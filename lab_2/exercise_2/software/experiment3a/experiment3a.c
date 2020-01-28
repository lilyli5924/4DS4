#include "system.h"
#include <io.h>
#include "sys/alt_stdio.h"
#include "altera_up_avalon_character_lcd.h"

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

    	case 13 : return 0x47; //L
    	case 14 : return 0x02; //G
    	case 15 : return 0x06; //E
    	default : return 0x7f;
	}
}


int main()
{ 
	alt_u16 a = 0x1;
	alt_u32 b = 0;
	alt_8 i;
	alt_8 sw_17 = 0;
	alt_8 sw_16 = 0;
	alt_16 sw_15 = 0;
	alt_16 sw_15_buf = 0;
	alt_16 min = 0;
	alt_16 max = 0;
	alt_32 sum = 0;
	alt_16 avg = 0;
	alt_u32 switch_val = 0;
	alt_u8 sw_16_buf = 0;
	alt_up_character_lcd_dev *lcd_0;
	alt_16 record_val[16];
	char lcd_string[17];

	lcd_string[0] = ' ';
	lcd_string[1] = ' ';
	lcd_string[2] = ' ';
	lcd_string[3] = ' ';
	lcd_string[4] = ' ';
	lcd_string[5] = ' ';
	lcd_string[6] = ' ';
	lcd_string[7] = ' ';
	lcd_string[8] = ' ';
	lcd_string[9] = ' ';
	lcd_string[10] = ' ';
	lcd_string[11] = ' ';
	lcd_string[12] = ' ';
	lcd_string[13] = ' ';
	lcd_string[14] = ' ';
	lcd_string[15] = ' ';
	lcd_string[16] = '\0';

	char term_string [17];
	term_string[0] = ' ';
	term_string[1] = ' ';
	term_string[2] = ' ';
	term_string[3] = ' ';
	term_string[4] = ' ';
	term_string[5] = ' ';
	term_string[6] = ' ';
	term_string[7] = ' ';
	term_string[8] = ' ';
	term_string[9] = ' ';
	term_string[10] = ' ';
	term_string[11] = ' ';
	term_string[12] = ' ';
	term_string[13] = ' ';
	term_string[14] = ' ';
	term_string[15] = ' ';
	term_string[16] = '\0';

	record_val[0] = 0;
	record_val[1] = 0;
	record_val[2] = 0;
	record_val[3] = 0;
	record_val[4] = 0;
	record_val[5] = 0;
	record_val[6] = 0;
	record_val[7] = 0;
	record_val[8] = 0;
	record_val[9] = 0;
	record_val[10] = 0;
	record_val[11] = 0;
	record_val[12] = 0;
	record_val[13] = 0;
	record_val[14] = 0;
	record_val[15] = 0;

	alt_printf("Exercise 2:\n");
	lcd_0 = alt_up_character_lcd_open_dev(CHARACTER_LCD_0_NAME);

	if (lcd_0 == NULL) alt_printf("Error opening LCD device\n");
	else alt_printf("LCD device opened.\n");

	alt_up_character_lcd_init(lcd_0);
	alt_up_character_lcd_string(lcd_0, "exercise 2");

	/* Event loop never exits. */
	while (1) {
		b = b + 1;
		sw_16_buf = sw_16; //assign old switch 16
		sw_15_buf = sw_15; //assign old switch [15:0]
		switch_val = IORD(SWITCH_I_BASE, 0);

		//value of switch[17]
		sw_17 = (switch_val >> 17) & 0x1;
		//value of switch[16]
		sw_16 = (switch_val >> 16) & 0x1;
		//value of switch [15:0]
		sw_15 = switch_val & 0xFFFF;

		if (sw_15 != sw_15_buf){

			// Only sum up the last 16 values
			sum = sum - record_val[0] + sw_15;

			// Divided by 16
			avg = sum/16;

			record_val[0] = record_val[1];
	    	record_val[1] = record_val[2];
	    	record_val[2] = record_val[3];
	    	record_val[3] = record_val[4];
	    	record_val[4] = record_val[5];
	    	record_val[5] = record_val[6];
	    	record_val[6] = record_val[7];
	    	record_val[7] = record_val[8];
	    	record_val[8] = record_val[9];
	    	record_val[9] = record_val[10];
	    	record_val[10] = record_val[11];
	    	record_val[11] = record_val[12];
	    	record_val[12] = record_val[13];
	    	record_val[13] = record_val[14];
	    	record_val[14] = record_val[15];
	    	record_val[15] = sw_15;
	    	alt_printf("sw_15 in: %x\n",sw_15);
	    	alt_printf("sum is: %x, avg is %x \n",sum,avg);
	    	min = record_val[0];
			max = record_val[0];
		
			// Compare each record value in the register and find the min and max
			for (i=1;i<16;i++){
				// max
				if (record_val[i] < 0){
			  		if (max < 0) {
				  		if (((~record_val[i]) + 1) < ((~max) + 1)) {
					  		max = record_val[i];
				  		}
			  		}
				}else {
			  		if (max < 0){
						max = record_val[i];
			  		} else {
				  		if (record_val[i] > max){
							max = record_val[i];
				  		}
			  		}
		  		}
				// min
				if (record_val[i] < 0){
			  		if (min < 0){
				  		if (((~record_val[i]) + 1) > ((~min) + 1)) {
							min = record_val[i];
						}
					}
					else {
						min = record_val[i];//min = positive; record = negative
					}
				}else {
					if (min > 0){
						if (record_val[i] < min){
							min = record_val[i];
						}
					}
				}
		  
			}
			//alt_printf("min value is: %x , max is =  %x\n", min, max);
		}

		if (sw_17){
			if (sw_16 == 0 && sw_16_buf == 1){    // switch 16 from high to low
				IOWR(LED_RED_O_BASE, 0, switch_val);
				alt_printf("%x, %x \n",avg,sw_15);
				if (avg < sw_15){
					IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0, disp_seven_seg(13));}
				else if (avg > sw_15){
					IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0, disp_seven_seg(14));}
				else {
					IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0, disp_seven_seg(15));}
			}
			else if (sw_16 == 1 && sw_16_buf == 0) { // switch 16 from low to high
					IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0, 0x7f);
					IOWR(LED_GREEN_O_BASE,0,0x0);
					IOWR(LED_RED_O_BASE,0,0x0);
			}
		} else {
			IOWR(LED_RED_O_BASE, 0, switch_val);
			if (sw_16 == 0 && sw_16_buf == 1){ // switch 16 from high to low
				// Finding out the number on each bit in decimal
				if(min < 0) {
					alt_u16 min_val = 0;
					min_val = (~min) + 1;
					alt_printf("min = %x, min_val = %x \n", min, min_val);
					lcd_string[1] = '0' + (min_val/10000);
					lcd_string[2] = '0' + ((min_val%10000)/1000);
					lcd_string[3] = '0' + ((min_val%1000)/100);
					lcd_string[4] = '0' + ((min_val%100)/10);
					lcd_string[5] = '0' + (min_val%10);

					lcd_string [0] = '-';
				}
				else {
					lcd_string[0] = '0' + (min/10000);
					lcd_string[1] = '0' + ((min%10000)/1000);
					lcd_string[2] = '0' + ((min%1000)/100);
					lcd_string[3] = '0' + ((min%100)/10);
					lcd_string[4] = '0' + (min%10);

					for (i=1; i<=4; i++){
						if (lcd_string[0] == '0'){
							lcd_string[0] = lcd_string[1];
							lcd_string[1] = lcd_string[2];
							lcd_string[2] = lcd_string[3];
							lcd_string[3] = lcd_string[4];
							lcd_string[4] = ' ';
						}else
							break;
					}
				}

				alt_up_character_lcd_init(lcd_0);
				alt_up_character_lcd_set_cursor_pos(lcd_0, 0, 0);
		    	alt_up_character_lcd_string(lcd_0, lcd_string);

			} else if (sw_16 == 1 && sw_16_buf == 0) { // switch 16 from low to high
				if (max < 0) {
					alt_u16 max_val;
					max_val = (~max) + 1;

					term_string[1] = '0' + (max_val/10000);
					term_string[2] = '0' + ((max_val%10000)/1000);
					term_string[3] = '0' + ((max_val%1000)/100);
					term_string[4] = '0' + ((max_val%100)/10);
					term_string[5] = '0' + (max_val%10);

					term_string [0] = '-';
				}
				else {
					term_string[0] = '0' + (max/10000);
					term_string[1] = '0' + ((max%10000)/1000);
					term_string[2] = '0' + ((max%1000)/100);
					term_string[3] = '0' + ((max%100)/10);
					term_string[4] = '0' + (max%10);

					for (i=1; i<=4; i++){
						if (term_string[0] == '0'){
							term_string[0] = term_string[1];
							term_string[1] = term_string[2];
							term_string[2] = term_string[3];
							term_string[3] = term_string[4];
							term_string[4] = ' ';
						} else
							break;
					}
				}
		    alt_printf("Max is %s \n",term_string);
			}
		}
	  	
	  	if (b == 10000000) {
			b = 0;
			if (a == 0x100) a = 0x1;
			else a = a << 1;
			IOWR(LED_GREEN_O_BASE,0,a);
		}
	}

	return 0;
}
