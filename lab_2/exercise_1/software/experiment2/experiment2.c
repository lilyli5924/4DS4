// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

#include "io.h"
#include "system.h"
#include "alt_types.h"
#include "sys/alt_stdio.h"

alt_u16 disp_seven_seg(alt_u8 val) {
    switch (val) {
        case  0 : return 0x40;
        case  1 : return 0x79;
        case  2 : return 0x24;
        case  3 : return 0x30;
        case  4 : return 0x19;
        case  5 : return 0x12;
        case  6 : return 0x02;
        case  7 : return 0x78;
        case  8 : return 0x00;
        case  9 : return 0x18;
        case 10 : return 0x08;
        case 11 : return 0x03;
        case 12 : return 0x46;
        case 13 : return 0x21;
        case 14 : return 0x06;
        case 15 : return 0x0e;
        default : return 0x7f;
    }
}        

int main()
{
    alt_32 i;
    alt_u32 switch_val = 0;
    alt_u32 led_grn_out = 0;
    alt_u32 led_out = 0;
    alt_u32 counter = 0;
    alt_u32 counter_1 = 0;
    alt_u32 led_red_out = 0;
    alt_putstr("Exercise 2!\n");
    
    /* Event loop never exits. */
    while (1) {

		// Initialize counter back to 0
		counter = 0;
		counter_1 = 0;
	
		led_out = (switch_val & 0x1) || (switch_val & 0x2);
		led_out = led_out + (((switch_val >> 17) && ((switch_val >> 16) & 0x1)) << 1);
	
		// RED Position 17 - 13
		for (i=17; i>=0; i--) {
			if((switch_val >> i) != 0) {
				led_red_out = led_red_out | (i << 13);
				break;
			}
		}
	
		// RED Position 12 - 8
		for (i=0; i<= 17; i++) {
			if ((switch_val >> i) & 0x1) {
				led_red_out = led_red_out | (i << 8);
				break;
			}
		}
	
		// RED Position 7 - 3
		for (i=17; i>=0; i--) {
			if (((switch_val >> i) & 0x1) != 0) {
				counter = counter + 1;
			}
		}
		led_red_out = led_red_out | (counter << 3);
	
		// RED Position 2
		if (counter > (0x12 - counter)) {
			led_red_out = led_red_out | 0x4;
		}
	
		// RED Position 1
		if (counter < (0x12 - counter)) {
			led_red_out = led_red_out | 0x2;
		}
	
		// RED Position 0
		if (counter == (0x12 - counter)) {
			led_red_out = led_red_out | 0x1;
		}
	
		// GRN Position 8
		led_grn_out = led_grn_out | 0x100;
	
		// GRN Position 7 - 4, # of times that a switch is ON followed by a switch OFF
		// The two values has to bitwise AND with 11 == 0x2 in order to detect
		for (i=17; i>=0; i--) {
			if (((switch_val >> (i-1)) & 0x3) == 0x2) {
				counter_1++;
			}
		}
		led_grn_out = led_grn_out | (counter_1 << 4);
	
		// GRN Position 0
		if ((counter & 0x1) == 0x1) {
			led_grn_out = led_grn_out | 0x1;
		}
	
		switch_val = IORD(SWITCH_I_BASE, 0);
		IOWR(LED_RED_O_BASE, 0, led_red_out);
		IOWR(LED_GREEN_O_BASE, 0, led_grn_out);
	
		if (switch_val == 0) {
			IOWR(SEVEN_SEGMENT_N_O_1_BASE, 0,
				disp_seven_seg(16)); // no 7 segment display
			IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0,
				disp_seven_seg(16));
		} else {
			for (i = 0; i <= 17; i++) {
				if ((switch_val >> i) & 0x1) {
					IOWR(SEVEN_SEGMENT_N_O_1_BASE, 0,
						disp_seven_seg((i >> 4) & 0xF));
					IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0,
						disp_seven_seg(i & 0xF));
					break;
				}
			}
		}

		// Clean the red and green LED every clock cycle.
		led_red_out = 0x0;
		led_grn_out = 0x0;
	}
    
    return 0;
}
