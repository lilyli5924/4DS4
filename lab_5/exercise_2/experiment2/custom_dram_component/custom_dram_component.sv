/*
Copyright by Henry Ko and Nicola Nicolici
Developed for the Digital Systems Design course (COE3DQ4)
Department of Electrical and Computer Engineering
McMaster University
Ontario, Canada
*/

`timescale 1ns/100ps
`default_nettype none

module custom_dram_component (
		input logic clock,
		input logic resetn,

		input  logic [2:0] 	address, // Base address offset
		input  logic 		chipselect,
		input  logic 		read,     // read enable
		input  logic 		write,    // write_enable
		output logic [31:0]	readdata, // Software config input
		input  logic [31:0]	writedata,
		output logic dram_done_irq
		
);

logic [8:0] read_address, read_counter,write_address, compare_addr,counter;
logic[17:0] test,test_2,test_3;
logic [15:0] write_data_b;
logic write_enable_b;
logic [15:0] read_data_a;
logic [15:0] read_data_b;
logic [15:0] num_min, num_max,  num_distinct,data_buffer;
logic [15:0] min, max;
logic load, load_config,flag;
logic start,finish, finish_buf; //start research

enum logic [3:0] {
	S_compare,
	S_compare_last,
	S_find_num,
	S_find_num_last,
	S_dinstinct_0,
	S_dinstinct_1,
	S_dinstinct_2,
	S_dinstinct_3,
	S_dinstinct_4,
	S_dinstinct_5,
	S_finish,
	S_IDLE
} state;


//assign min = 16'd66;
//assign num_min = 16'd250;
//assign max = 16'd4;
//assign num_max = 16'd5;
//assign num_distinct = 16'd6;

// Instantiate RAM0
dual_port_RAM0 dual_port_RAM_inst0 (
	.address_a ( read_address ),//search
	.address_b ( write_address ),//NIOS
	.clock ( clock ),
	.data_a ( 16'h00 ),
	.data_b ( write_data_b),
	.wren_a ( 1'b0 ),
	.wren_b ( write_enable_b ),
	.q_a ( read_data_a ),
	.q_b ( read_data_b )
	);

// 1. searching logic
// 2. interrupt
// 3. how to connect software to dpram

// for getting the configuration from NIOS
always_ff @ (posedge clock or negedge resetn) begin
	if (!resetn) begin
		write_enable_b <= 'd0;
		load <= 1'b0;
		load_config <= 'd0;
	end else begin
		load <= 1'b0;
		if (chipselect & write) begin
			case (address)
			'd0: begin 
				write_address <= writedata[24:16];
				write_data_b <= writedata[15:0];
				write_enable_b <= writedata[25];
			end
			'd7: begin
				load <= writedata[31];
				load_config <= writedata[0];
			end
			endcase
		end
	end
end

// for sending information to NIOS
always_ff @ (posedge clock or negedge resetn) begin
	if (!resetn) begin
		readdata <= 'd0;
	end else begin
		if (chipselect & read) begin
			case (address)
			'd1: readdata <= {16'd0, read_data_b};
			'd2: readdata <= {14'd0, min};
			'd3: readdata <= {16'd0, num_min};
			'd4: readdata <= {16'd0, max};
			'd5: readdata <= {16'd0, num_max};
			'd6: readdata <= {16'd0, num_distinct};
			endcase
		end
	end
end

assign start = load;

always_ff @(negedge clock or negedge resetn) begin
	if (!resetn) begin
		finish <= 1'b0;
		read_address <= 9'd0;
		min <= 16'd0;
		max <= 16'd0;
		num_min <= 16'd0;
		num_max <= 16'd0;
		read_counter <= 9'd1;
		num_distinct <= 16'd1;
		data_buffer <= 16'd0;
		flag <= 1'd0;
		compare_addr <= 9'd0;
		counter <= 9'd0;
		state <= S_IDLE;
	end else begin
		case (state)
			S_IDLE:begin
				read_address <= 9'd0;
				if (start) begin
					max <= 16'h8000;
					min <= 16'h7FFF;
					flag <= 1'd0;
					compare_addr <= 9'd0;
					num_min <= 16'd0;
					num_max <= 16'd0;
					num_distinct <= 16'd1;
					data_buffer <= 16'd0;
					counter <= 9'd0;
					read_counter <= 9'd1;
					state <= S_compare;
				end
			end
			S_compare : begin
				if ($signed(min) > $signed(read_data_a)) begin
					min <= $signed(read_data_a);
				end
				if ($signed(read_data_a) > $signed(max)) begin
					max <= $signed(read_data_a);
				end
				read_address <= read_address + 9'd1;
				if (read_address == 9'd511) begin
					state <= S_find_num;	
					read_address <= 9'd0;
				end
			end
			S_find_num : begin
				if ($signed(read_data_a) == $signed(min)) begin
					num_min <= num_min + 9'd1;
				end
				if ($signed(read_data_a) == $signed(max)) begin
					num_max <= num_max + 9'd1;
				end
				read_address <= read_address + 9'd1;
				if (read_address == 9'd511)begin
					state <= S_dinstinct_0;	
					read_address <= 9'd1;
				end	
			end	
			
			S_dinstinct_0: begin
				data_buffer <= read_data_a; 
			    state <= S_dinstinct_1; 	
				read_address <= 9'd0;
				counter <= 9'd0;								
			end
			
			S_dinstinct_1: begin
				read_address <= read_address + 9'd1;
				if (read_address < read_counter)begin
					if ($signed(data_buffer) == $signed(read_data_a)) counter <= counter + 9'd1;					
				end
                ////////////////////////////////////////////////////////////////           
				if (read_counter == 9'd511 && read_address == 9'd510)begin				
					state <= S_finish;
					if (counter == 9'd0)begin 
						num_distinct <= num_distinct + 16'd1;
					end
				end				
				if (read_counter < 9'd511)begin
					if (read_address == read_counter - 9'd1)begin
	
						state <= S_dinstinct_0;
						read_address <= read_counter + 9'd1;
						read_counter <= read_counter + 9'd1;						
						if ((counter == 9'd0) && ($signed(data_buffer) != $signed(read_data_a)))   						
							num_distinct <= num_distinct + 16'd1;		
					end				
				end
			end
			
			S_finish: begin
				finish <= 1'b1;	
				if (start == 1'b1) begin
					finish <= 1'b0;
					state <= S_IDLE;
				end				
			end
			
		endcase
	end
end

// Offset address 7 is used to clear the interrupt
always_ff @ (posedge clock or negedge resetn) begin
	if (!resetn) begin
		finish_buf <= 1'b0;
	end else begin
		finish_buf <= finish;	
		if (finish & ~finish_buf) dram_done_irq <= 1'b1;
		if (chipselect & write & (address == 'd7) & (load_config == 'd0)) dram_done_irq <= 1'b0;
	end
end


endmodule
