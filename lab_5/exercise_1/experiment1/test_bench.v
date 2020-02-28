`timescale 1ns/1ps

module test_bench;

	reg CLOCK_50_I;
	reg RESET_N_I;
	wire [17:0] LED_RED_O;
	wire [8:0] LED_GREEN_O;
	reg [16:0] SWITCH_I;
	reg [3:0] PUSH_BUTTON_I;

	experiment1 DUT (
		.clock_50_i_clk_in_clk                    (CLOCK_50_I),
		.clock_50_i_clk_in_reset_reset_n          (RESET_N_I),
		.led_red_o_external_connection_export     (LED_RED_O),
		.push_button_i_external_connection_export (PUSH_BUTTON_I),
		.switch_i_external_connection_export      (SWITCH_I),
		.led_green_o_external_connection_export   (LED_GREEN_O)
	);

	initial begin
		$timeformat(-3, 3, " ms", 11);

		SWITCH_I = 0;
		PUSH_BUTTON_I = 0;

		CLOCK_50_I = 0;
		RESET_N_I = 0;

		#70 RESET_N_I = 1;

		// wait for system startup
		@ (posedge LED_RED_O[0]);
		$display("TBINFO: System startup done...\n");

		SWITCH_I = 'h3 << 15;
		PUSH_BUTTON_I = 2;
		$display("TBINFO: Loading config\n");
		@ (negedge LED_RED_O[1]);
		// Load config IRQ done in software writes 0xD (1101) to LED_RED_O reg
		PUSH_BUTTON_I = 0;
		$display("TBINFO: Config being loaded\n");		
		@ (posedge LED_RED_O[1]);
		// Reset counter IRQ in software writes 0xB (1011) to LED_RED_O reg
		#5000;
		PUSH_BUTTON_I = 1;
		$display("TBINFO: Reset counter\n");
		@ (negedge LED_RED_O[2]);
		PUSH_BUTTON_I = 0;
		$display("TBINFO: Counter counting\n");
		@ (posedge LED_RED_O[3]);
		// Counter expire IRQ in software writes 0x7 (0111) to LED_RED_O reg
		$display("TBINFO: Dortmund \n");

		#100000;
		$display("TBINFO: Simulation done\n");
		$stop;
	end


	always @ (posedge DUT.jtag_uart_0.av_chipselect) begin
		#1;	// Wait until signals settle
		if (DUT.jtag_uart_0.av_write_n == 1'b0) begin
			if(DUT.custom_counter_component_0.custom_counter_unit_inst.counter_expire == 'd1) begin
			//if (DUT.jtag_uart_0.av_address == 'd0) begin
			if(DUT.jtag_uart_0.av_writedata == 32'h44 || DUT.jtag_uart_0.av_writedata == 32'h64) begin
				$display("\n%X %t ", DUT.jtag_uart_0.av_writedata, $realtime);
			end
			end
			//end
		end
	end


	always begin
		CLOCK_50_I = #10 ~CLOCK_50_I;
	end

endmodule

