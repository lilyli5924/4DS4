// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

`timescale 1ns/100ps
`default_nettype none

// This is the top module
// It interfaces to the LCD display and touch panel
module exercise1 (
	/////// board clocks                      ////////////
	input logic CLOCK_50_I,                   // 50 MHz clock
	
	/////// pushbuttons/switches              ////////////
	input logic[3:0] PUSH_BUTTON_I,           // pushbuttons
	input logic[17:0] SWITCH_I,               // toggle switches
	
	/////// 7 segment displays/LEDs           ////////////
	output logic[6:0] SEVEN_SEGMENT_N_O[7:0], // 8 seven segment displays
	output logic[8:0] LED_GREEN_O,            // 9 green LEDs
	output logic[17:0] LED_RED_O,             // 18 red LEDs
	
	/////// GPIO connections                  ////////////
	inout wire[35:0] GPIO_0                   // GPIO Connection 0 (LTM)
);

// Signals for LCD Touch Module (LTM)
// LCD display interface
logic 	[7:0]	LTM_R, LTM_G, LTM_B;
logic 			LTM_HD, LTM_VD;
logic 			LTM_NCLK, LTM_DEN, LTM_GRST;

// LCD configuration interface
wire 			LTM_SDA;
logic 			LTM_SCLK, LTM_SCEN;

// LCD touch panel interface
logic 			TP_DCLK, TP_CS, TP_DIN, TP_DOUT;
logic 			TP_PENIRQ_N, TP_BUSY;

// Internal signals
logic 			Clock, Resetn;
logic 	[2:0] 	Top_state;

// For LCD display / touch screen
logic 			LCD_TPn_sel, LCD_TPn_sclk;
logic 			LCD_config_start, LCD_config_done;
logic 			LCD_enable, TP_enable;
logic 			TP_touch_en, TP_coord_en;
logic 	[11:0]	TP_X_coord, TP_Y_coord;

logic 	[9:0] 	Colourbar_X, Colourbar_Y;
logic 	[7:0]	Colourbar_Red, Colourbar_Green, Colourbar_Blue;

logic 	[4:0] 	TP_position[7:0];

assign Clock = CLOCK_50_I;
assign Resetn = SWITCH_I[17];

assign LCD_TPn_sclk = (LCD_TPn_sel) ? LTM_SCLK : TP_DCLK;
assign LTM_SCEN = (LCD_TPn_sel) ? 1'b0 : ~TP_CS;
assign LTM_GRST = Resetn;

// Connections to GPIO for LTM
assign TP_PENIRQ_N   = GPIO_0[0];
assign TP_DOUT       = GPIO_0[1];
assign TP_BUSY       = GPIO_0[2];
assign GPIO_0[3]	 = TP_DIN;

assign GPIO_0[4]	 = LCD_TPn_sclk;

assign GPIO_0[35]    = LTM_SDA;
assign GPIO_0[34]    = LTM_SCEN;
assign GPIO_0[33]    = LTM_GRST;

assign GPIO_0[9]	 = LTM_NCLK;
assign GPIO_0[10]    = LTM_DEN;
assign GPIO_0[11]    = LTM_HD;
assign GPIO_0[12]    = LTM_VD;

assign GPIO_0[5]     = LTM_B[3];
assign GPIO_0[6]     = LTM_B[2];
assign GPIO_0[7]     = LTM_B[1];
assign GPIO_0[8]     = LTM_B[0];
assign GPIO_0[16:13] = LTM_B[7:4];
assign GPIO_0[24:17] = LTM_G[7:0];
assign GPIO_0[32:25] = LTM_R[7:0];

// Top state machine for controlling resets
always_ff @(posedge Clock or negedge Resetn) begin
	if (~Resetn) begin
		Top_state <= 3'h0;
		TP_enable <= 1'b0;
		LCD_enable <= 1'b0;
		LCD_config_start <= 1'b0;
		LCD_TPn_sel <= 1'b1;
	end else begin
		case (Top_state)
			3'h0 : begin
				LCD_config_start <= 1'b1;
				LCD_TPn_sel <= 1'b1;
				Top_state <= 3'h1;
			end			
			3'h1 : begin
				LCD_config_start <= 1'b0;
				if (LCD_config_done & ~LCD_config_start) begin
					TP_enable <= 1'b1;
					LCD_enable <= 1'b1;
					LCD_TPn_sel <= 1'b0;
					Top_state <= 3'h2;
				end
			end			
			3'h2 : begin
				Top_state <= 3'h2;
			end
		endcase
	end
end		

logic [3:0] color_code;
logic [2:0] counter_p_0, counter_p_1, counter_m_0, counter_m_1; 
logic [25:0] counter_1s;
logic [15:0] counter_1ms, check;
logic [3:0] area_code_buf;

//Second counter
always_ff @ (posedge Clock or negedge Resetn) begin
	if(~Resetn) begin
		counter_1s <= 26'd0;
	end else begin
		area_code_buf <= area_code;
		if(TP_touch_en) begin
			if(area_code_buf == area_code) begin
				if(counter_1s < 26'd49999999) begin
					counter_1s <= counter_1s + 26'd1;
				end else begin
					counter_1s <= 26'd0;
				end
			end else begin
				counter_1s <= 26'd0;
			end
		end else begin
			counter_1s <= 26'd0;
		end
	end	
end

always_ff @(posedge Clock or negedge Resetn) begin
	if (~Resetn) begin
		counter_p_0 <= 3'b000;
		counter_m_0 <= 3'b111;
		counter_p_1 <= 3'b000;
		counter_m_1 <= 3'b111;
	end else begin
		if(counter_1s == 26'd49999999) begin
			if(area_code == 4'd0) begin
				// left top
				if (counter_p_0 < 3'b111) begin
					counter_p_0 <= counter_p_0 + 3'd1;					
				end else begin
					counter_p_0 <= 3'b000;
				end
			end
			if(area_code == 4'd1) begin
				// right top
				if (counter_m_0 > 3'b000) begin
					counter_m_0 <= counter_m_0 - 3'd1;
				end else begin
					counter_m_0 <= 3'b111;				
				end
			end
			if(area_code == 4'd2) begin
				if (counter_m_1 > 3'b000) begin
					counter_m_1 <= counter_m_1 - 3'd1;					
				end else begin
					counter_m_1 <= 3'b111;				
				end
			end
			if(area_code == 4'd3) begin
				if (counter_p_1 < 3'b111) begin
					counter_p_1 <= counter_p_1 + 3'd1;						
				end else begin
					counter_p_1 <= 3'b000;
				end
			end
		end
	end
end

always_comb begin
	color_code = 3'b111;
	if (counter_p_0 == counter_p_1) begin
		if (counter_m_0 == counter_m_1) begin
			color_code = (counter_p_0 > counter_m_0) ? counter_p_0 : counter_m_0;
		end else begin
			color_code = counter_p_0;
		end
	end else if (counter_m_0 == counter_p_0) begin
		if (counter_m_1 == counter_p_1)begin
			color_code = (counter_m_0 > counter_m_1) ? counter_m_0 : counter_m_1;
		end else begin
			color_code = counter_m_0;
		end
	end else if (counter_p_0 == counter_m_1) begin
		if (counter_m_0 == counter_p_1) begin
			color_code = (counter_p_0 > counter_m_0) ? counter_p_0 : counter_m_0;
		end else begin
			color_code = counter_p_0;
		end
	end else if (counter_p_1 == counter_m_0) begin
		if (counter_m_1 == counter_p_0) begin
			color_code = (counter_p_1 > counter_m_1) ? counter_p_1 : counter_m_1;
		end else begin
			color_code = counter_p_1;
		end
	end else if (counter_p_1 == counter_m_1) begin
		if (counter_m_0 == counter_p_0) begin
			color_code = (counter_p_1 > counter_m_0) ? counter_p_1 : counter_m_0;
		end else begin
			color_code = counter_p_1;
		end
	end else if (counter_m_0 == counter_m_1) begin
		if (counter_m_1 == counter_p_0) begin
			color_code = (counter_m_0 > counter_m_1) ? counter_m_0 : counter_m_1;
		end else begin
			color_code = counter_m_0;
		end
	end else
		if (counter_p_0 > counter_m_0 && counter_p_0 > counter_m_1 && counter_p_0 > counter_p_1)
			color_code = counter_p_0;
		else if (counter_p_1 > counter_m_0 && counter_p_1 > counter_m_1 && counter_p_1 > counter_p_0)
			color_code = counter_p_1;
		else if (counter_m_0 > counter_p_1 && counter_m_0 > counter_m_1 && counter_m_0 > counter_p_0)
			color_code = counter_m_0;
		else if (counter_m_1 > counter_p_1 && counter_m_1 > counter_m_0 && counter_m_1 > counter_p_0)
			color_code = counter_m_1;
end

//BCD 
logic [3:0] bcd3,bcd2,bcd1,bcd0;
always_ff @ (posedge CLOCK_50_I or negedge Resetn) begin
	if (~Resetn) begin
		{bcd3,bcd2,bcd1,bcd0} <= 16'd0;
		counter_1ms <= 16'd0;
		check <= 16'd0;
	end else begin
		if (TP_touch_en) begin	
			if(counter_1ms < 16'd49999) begin
				counter_1ms <= counter_1ms + 16'd1;
			end else begin
				counter_1ms <= 16'd0;
				check <= check + 16'd1;
				bcd0 <= bcd0 + 4'd1;					
				if(bcd0 == 4'd9) begin
					bcd1 <= bcd1 + 4'd1;
					bcd0 <= 4'd0;
					if(bcd1 == 4'd9) begin
						bcd1 <= 4'd0;
						bcd2 <= bcd2 + 4'd1;
						if (bcd2 == 4'd9) begin
							bcd2 <= 4'd0; 
							if (bcd3 != 4'd9)
							bcd3 <= bcd3 + 4'd1;
						end	
					end
				end
			end				
		end else begin
			counter_1ms <= 16'd0;
			check <= 16'd0;
			{bcd0,bcd1,bcd2,bcd3} <= 16'd0;
		end
	end
end

// LCD Configuration
LCD_Config_Controller LCD_Config_unit(
	.Clock(Clock),
	.Resetn(Resetn),
	.Start(LCD_config_start),
	.Done(LCD_config_done),
	.LCD_I2C_sclk(LTM_SCLK),
 	.LCD_I2C_sdat(LTM_SDA),
	.LCD_I2C_scen()
);

// LCD Image
LCD_Data_Controller LCD_Data_unit (
	.Clock(Clock),
	.oClock_en(),
	.Resetn(Resetn),
	.Enable(LCD_enable),
	.iRed(Colourbar_Red),
	.iGreen(Colourbar_Green),
	.iBlue(Colourbar_Blue),
	.oCoord_X(Colourbar_X),
	.oCoord_Y(Colourbar_Y),
	.H_Count(), // not used in this experiment
	.V_Count(), // not used in this experiment
	.LTM_NCLK(LTM_NCLK),
	.LTM_HD(LTM_HD),
	.LTM_VD(LTM_VD),
	.LTM_DEN(LTM_DEN),
	.LTM_R(LTM_R),
	.LTM_G(LTM_G),
	.LTM_B(LTM_B)
);

// State machine for generating the colour bars
always_ff @(posedge Clock or negedge Resetn) begin
	if (~Resetn) begin
		Colourbar_Red <= 8'h00; 
		Colourbar_Green <= 8'h00;
		Colourbar_Blue <= 8'h00;
	end else begin

		if (Colourbar_X < 10'd400) begin
			if (Colourbar_Y < 10'd240) begin
				//left top
				Colourbar_Red <= {8{counter_p_0[2]}};
				Colourbar_Green <= {8{counter_p_0[1]}};
				Colourbar_Blue <= {8{counter_p_0[0]}};
			end else begin
				//right top
				Colourbar_Red <= {8{counter_m_1[2]}};
				Colourbar_Green <= {8{counter_m_1[1]}};
				Colourbar_Blue <= {8{counter_m_1[0]}};		
			end
		end else begin
			if (Colourbar_Y < 10'd240) begin
				//left bottom
				Colourbar_Red <= {8{counter_m_0[2]}};
				Colourbar_Green <= {8{counter_m_0[1]}};
				Colourbar_Blue <= {8{counter_m_0[0]}};
			end else begin
				//right bottom
				Colourbar_Red <= {8{counter_p_1[2]}};
				Colourbar_Green <= {8{counter_p_1[1]}};
				Colourbar_Blue <= {8{counter_p_1[0]}};
			end
		end
	end
end

// Controller for the TP on the LTM
Touch_Panel_Controller Touch_Panel_unit(
	.Clock_50MHz(Clock),
	.Resetn(Resetn),
	.Enable(~LTM_VD),	
	.Touch_En(TP_touch_en),
	.Coord_En(TP_coord_en),
	.X_Coord(TP_X_coord),
	.Y_Coord(TP_Y_coord),
	.TP_PENIRQ_N_I(TP_PENIRQ_N),
	.TP_BUSY_I(TP_BUSY),
	.TP_SCLK_O(TP_DCLK),
	.TP_MOSI_O(TP_DIN),
	.TP_MISO_I(TP_DOUT),
	.TP_SS_N_O(TP_CS)
);

// State machine for capturing the touch panel coordinates
// and displaying them on the seven segment displays
always_ff @(posedge Clock or negedge Resetn) begin
	if (~Resetn) begin
		TP_position[0] <= 5'h00;
		TP_position[1] <= 5'h00;
		TP_position[2] <= 5'h00;
		TP_position[3] <= 5'h00;
		TP_position[4] <= 5'h00;
		TP_position[5] <= 5'h00;
		TP_position[6] <= 5'h00;
		TP_position[7] <= 5'h00;
	end else begin
		if (TP_coord_en) begin
			TP_position[0][3:0] <= TP_Y_coord[3:0];
			TP_position[1][3:0] <= TP_Y_coord[7:4];
			TP_position[2][3:0] <= TP_Y_coord[11:8];
			TP_position[4][3:0] <= TP_X_coord[3:0];
			TP_position[5][3:0] <= TP_X_coord[7:4];
			TP_position[6][3:0] <= TP_X_coord[11:8];
		end
		TP_position[0][4] <= TP_touch_en;
		TP_position[1][4] <= TP_touch_en;
		TP_position[2][4] <= TP_touch_en;
		TP_position[4][4] <= TP_touch_en;
		TP_position[5][4] <= TP_touch_en;
		TP_position[6][4] <= TP_touch_en;
	end
end

// Seven segment displays
//seven_seg_displays display_unit (
//	.hex_values(TP_position),
//	.SEVEN_SEGMENT_N_O(SEVEN_SEGMENT_N_O)
//);

logic [6:0] converted_values [5:0] ;
logic [3:0] area_code;

always_comb begin
	if (TP_X_coord < 12'd2048) begin
		if (TP_Y_coord < 12'd2048) begin
			//left top
			area_code = 4'd0;
		end else begin
			//left bottom
			area_code = 4'd2;
		end
	end else begin
		if (TP_Y_coord < 12'd2048) begin
			//right top
			area_code = 4'd1;
		end else begin
			//right bottom
			area_code = 4'd3;
		end
	end
end

convert_hex_to_seven_segment seg5 (
	.hex_value(color_code),
	.converted_value(converted_values[5])
);
convert_hex_to_seven_segment seg4 (
	.hex_value(area_code),
	.converted_value(converted_values[4])
);
convert_hex_to_seven_segment seg3 (
	.hex_value(bcd3),
	.converted_value(converted_values[3])
);
convert_hex_to_seven_segment seg2 (
	.hex_value(bcd2),
	.converted_value(converted_values[2])
);
convert_hex_to_seven_segment seg1 (
	.hex_value(bcd1),
	.converted_value(converted_values[1])
);
convert_hex_to_seven_segment seg0 (
	.hex_value(bcd0),
	.converted_value(converted_values[0])
);
assign SEVEN_SEGMENT_N_O[7] = (TP_touch_en) ? converted_values[4] : 7'h7F;
assign SEVEN_SEGMENT_N_O[0] = (TP_touch_en) ? converted_values[0] : converted_values[5];
assign SEVEN_SEGMENT_N_O[1] = (TP_touch_en) ? converted_values[1] : 7'h7F;
assign SEVEN_SEGMENT_N_O[2] = (TP_touch_en) ? converted_values[2] : 7'h7F;
assign SEVEN_SEGMENT_N_O[3] = (TP_touch_en) ? converted_values[3] : 7'h7F;

assign SEVEN_SEGMENT_N_O[6] = 7'h7F;
assign SEVEN_SEGMENT_N_O[5] = 7'h7F;
assign SEVEN_SEGMENT_N_O[4] = 7'h7F;
endmodule
