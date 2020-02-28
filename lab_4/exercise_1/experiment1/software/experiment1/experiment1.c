// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

#include <stdio.h>
#include "io.h"
#include "system.h"
#include "sys/alt_irq.h"
#include "sys/alt_stdio.h"
#include "priv/alt_busy_sleep.h"

const M = 10;
const N = 10;



int RGB_colour(int colour) {
    switch (colour & 0x7) {
        case 0 : return 0x00000000; // black 
        case 1 : return 0x03FF0000; // blue
        case 2 : return 0x7C007C00; // green 
        case 3 : return 0x7FFF7C00; // cyan
        case 4 : return 0x000003FF; // red
        case 5 : return 0x03FF03FF; // magenta
        case 6 : return 0x7C007FFF; // yellow
        case 7 : return 0x7FFF7FFF; // white
    }
    return 0x00000000;
}    

//void draw_horizontal_bars(int width);
void draw_initial_square(int width, int length);
void square_paint(int rb, int cb,int arr[M][N]);
int col_check(int arr[M][N],int col);
int row_check(int arr[M][N],int row);

void TouchPanel_int(int arr[M][N]) {
    static int width = 32;
    int TP_val, x_val, y_val, key = 6;
	static int cursor_x = 0;
	static int cursor_y = 0;
	int i,j;
	int x = 0, y = 0;
	int width_counter = 0, length_counter = 0;
	int cb = 0;
	int rb = 0;
	int color_change = 0;

    TP_val = IORD(NIOS_LCD_COMPONENT_0_TOUCHPANEL_BASE, 0);
    x_val = (TP_val >> 20) & 0xFF; y_val = (TP_val >> 4) & 0xFF;
    if (((TP_val >> 31) & 0x1) && (x_val >= 0xC9) && (x_val <= 0xF1)) {
        if ((y_val >= 0x17) && (y_val <= 0x33)) { // Key 0
            key = 0;
            IOWR(NIOS_LCD_COMPONENT_0_CONSOLE_BASE, 0, 0x1);
        }
        if ((y_val >= 0x3D) && (y_val <= 0x58)) { // Key 1
            key = 1;
            IOWR(NIOS_LCD_COMPONENT_0_CONSOLE_BASE, 0, 0x2);
        }
        if ((y_val >= 0x62) && (y_val <= 0x7E)) { // Key 2
            key = 2;
			IOWR(NIOS_LCD_COMPONENT_0_CONSOLE_BASE, 0, 0x4);
        }
        if ((y_val >= 0x88) && (y_val <= 0xA4)) { // Key 3
            key = 3;
            IOWR(NIOS_LCD_COMPONENT_0_CONSOLE_BASE, 0, 0x8);
        }
        if ((y_val >= 0xAE) && (y_val <= 0xC9)) { // Key 4
            key = 4;
            IOWR(NIOS_LCD_COMPONENT_0_CONSOLE_BASE, 0, 0x10);
        }
        if ((y_val >= 0xD3) && (y_val <= 0xEF)) { // Key 5
            key = 5;
            IOWR(NIOS_LCD_COMPONENT_0_CONSOLE_BASE, 0, 0x20);
        }
    } else IOWR(NIOS_LCD_COMPONENT_0_CONSOLE_BASE, 0, 0x0);



    if (IORD(NIOS_LCD_COMPONENT_0_TOUCHPANEL_BASE, 2) & 0x2) { // posedge
        // Set pixel position to top-left corner
        IOWR(NIOS_LCD_COMPONENT_0_IMAGE_BASE, 2, 0x1);

        if (key == 0) {
        	//printf("key 0 is pressed, move up \n");
        	if (cursor_y == 0){
        		cursor_y = 9;
        	}
        	else {
        	cursor_y  = cursor_y - 1; //up
        	}

        }
        if (key == 1) {
        	//printf("key 1 is pressed, move down \n");
        	if (cursor_y == 9){
        		cursor_y = 0;
        	}
        	else {
        		cursor_y = cursor_y + 1; //down
        	}
        }
        if (key == 2) {
        	//printf("key 2 is pressed, move left \n");
        	if (cursor_x == 0){
        		cursor_x = 9;
        	}
        	else {
        		cursor_x  = cursor_x - 1;//left
        	}
        }
        if (key == 3) {
        	//printf("key 3 is pressed, move right \n");
			if (cursor_x == 9){
				cursor_x = 0;
			}
			else {
				cursor_x = cursor_x + 1;//right
            }
       }
        if (key == 4) {
        	if(arr[cursor_y][cursor_x] == 7) {
        		arr[cursor_y][cursor_x] = 0;
        	}else {
        		arr[cursor_y][cursor_x]  = arr[cursor_y][cursor_x] + 1;
        	}
        	color_change = 1;
        }
        if (key == 5) {
        	if(arr[cursor_y][cursor_x] == 0) {
        		arr[cursor_y][cursor_x] = 7;
        	}else{
        		arr[cursor_y][cursor_x]  = arr[cursor_y][cursor_x] - 1;
        	}
        	color_change = 1;
        }

        //display to LCD
        //where the cursor is in this square
         x = cursor_x*64;
         y = cursor_y*48;
         //printf("x = %d, y = %d \n", x, y);
         if (color_change){
        	 printf("colour changed!, pixel colour = %d \n", arr[cursor_y][cursor_x]);
         }
         for (i=0; i<480; i++){
        	 width_counter = 0;
        	 cb = 0;
        	 for (j=0; j<640; j++){
        		 if (cb == cursor_x && rb == cursor_y) {
        			 if((i >= 12+y) && (i <= 35+y)) {
        				 if((j > 19+x) && (j <= 43+x)) {
        			     	IOWR(NIOS_LCD_COMPONENT_0_IMAGE_BASE, 0, ~RGB_colour(arr[rb][cb]));//negative RGB??????
        			     }
        				 else {
        					 IOWR(NIOS_LCD_COMPONENT_0_IMAGE_BASE, 0, RGB_colour(arr[rb][cb]));
        				 }
        			 }
        			 else {
        				 IOWR(NIOS_LCD_COMPONENT_0_IMAGE_BASE, 0, RGB_colour(arr[rb][cb]));
        			 }
        		 }
        		 else {
        			 IOWR(NIOS_LCD_COMPONENT_0_IMAGE_BASE, 0, RGB_colour(arr[rb][cb]));
        		 }

        		 if (width_counter == 64) {
        		 	width_counter = 0;
        		 	cb++;
        		 }
        		 width_counter++;
        	 }
             if (length_counter == 48) {
             	 length_counter = 0;
             	 rb++;
             }
             length_counter++;
         }

    }


    //check row
    if(color_change){
    	if (row_check(arr, cursor_y)){
    		printf("All rectangles from row %d have color %d \n", cursor_y, arr[cursor_y][0]);
    	}
    }

    //check column
    if(color_change){
    if (col_check(arr, cursor_x)){
        	printf("All rectangles from col %d have color %d \n", cursor_x, arr[0][cursor_x]);
        }
    }

    IOWR(NIOS_LCD_COMPONENT_0_CONSOLE_BASE, 0, 0x0);
    TP_val = IORD(NIOS_LCD_COMPONENT_0_TOUCHPANEL_BASE, 2);
    IOWR(NIOS_LCD_COMPONENT_0_TOUCHPANEL_BASE, 2, TP_val & 0x30);

}

int col_check(int arr[M][N],int col){
	int i,flag = 0;
		for (i = 0; i<9; i++){
			if (arr[i][col] != arr[i+1][col]){
				flag = 1;
				break;
			}
		}
		if(flag) {
			return 0;
		}else {
			return 1;
		}
}

int row_check(int arr[M][N],int row){
	int i,flag = 0;
		for (i= 0; i<9; i++){
			if (arr[row][i] != arr[row][i+1]){
				flag = 1;
				break;
			}
		}
		if(flag) {
			return 0;
		}else {
			return 1;
	}
}

void draw_initial_square(int width, int length/*, int arr[M][N]*/) {
    int i, j;
    int colour_white = 7;
    int colour_black = 0;
    int RGB = RGB_colour(colour_white);
    int width_counter = 0;
    int length_counter = 0;
    // Set pixel position to top-left corner
    IOWR(NIOS_LCD_COMPONENT_0_IMAGE_BASE, 2, 0x1);

    for (i = 0; i < 480; i++) {
        for (j = 0; j < 640; j++) {
        	if((j<64 && i<48) || (j<64 && i>432) || (j>575 && i<48) || (j>575 && i>432)) {
        		if((i > 12) && (i <= 35)) {
        			if((j > 19) && (j <= 43)) {
        				IOWR(NIOS_LCD_COMPONENT_0_IMAGE_BASE, 0, RGB_colour(colour_black));//negative RGB??????
           	        }
        			else {
        				IOWR(NIOS_LCD_COMPONENT_0_IMAGE_BASE, 0, RGB_colour(colour_white));
        			}
        		}
           	    else {
           	    	IOWR(NIOS_LCD_COMPONENT_0_IMAGE_BASE, 0, RGB);
           	    }
        		if (width_counter == width) {
        		    width_counter = 0;
        		    RGB = RGB_colour(colour_white);
        		}
        		width_counter++;
        	}
        	else {
        		if (width_counter == width) {
        		    width_counter = 0;
        		    RGB = RGB_colour(colour_black);
        		}
        		width_counter++;
        		IOWR(NIOS_LCD_COMPONENT_0_IMAGE_BASE, 0, RGB);
        	}
        }
        if (length_counter == length) {
        	length_counter = 0;
        }
        length_counter++;
    }
}

int main()
{
	//store the RGB value for every square
	int square_colour[10][10]={{7,0,0,0,0,0,0,0,0,7},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{7,0,0,0,0,0,0,0,0,7},
	};//rb,cb
	//square_colour[0][0] = square_colour[0][9] =square_colour[9][0] =square_colour[9][9] = 7;

	printf("Experiment 1!\n");

    //initial state
    draw_initial_square(64,48);

    alt_irq_register(NIOS_LCD_COMPONENT_0_TOUCHPANEL_IRQ, square_colour, (void *)TouchPanel_int);

    // Turn button indicators off
    IOWR(NIOS_LCD_COMPONENT_0_CONSOLE_BASE, 0, 0x0);

    while (1);
    return 0;

}

