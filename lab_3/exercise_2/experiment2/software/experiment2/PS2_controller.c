// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

#include "define.h"
#include "stdbool.h"

void translate_PS2_code(buffer_ISR* buffer, int PS2_code, int cap_lock) {
	if (buffer->cur_buf_length >= MAX_STRING_LENGTH-2) {
		printf("buffer overflow\n");
	} else {
		//cap lock 0x58
		if(cap_lock == 0){
			switch (PS2_code) {
				case 0x0E: buffer->string_buffer[buffer->cur_buf_length++] = '`'; break;
				case 0x16: buffer->string_buffer[buffer->cur_buf_length++] = '1'; break;
				case 0x1E: buffer->string_buffer[buffer->cur_buf_length++] = '2'; break;
				case 0x26: buffer->string_buffer[buffer->cur_buf_length++] = '3'; break;
				case 0x25: buffer->string_buffer[buffer->cur_buf_length++] = '4'; break;
				case 0x2E: buffer->string_buffer[buffer->cur_buf_length++] = '5'; break;
				case 0x36: buffer->string_buffer[buffer->cur_buf_length++] = '6'; break;
				case 0x3D: buffer->string_buffer[buffer->cur_buf_length++] = '7'; break;
				case 0x3E: buffer->string_buffer[buffer->cur_buf_length++] = '8'; break;
				case 0x46: buffer->string_buffer[buffer->cur_buf_length++] = '9'; break;
				case 0x45: buffer->string_buffer[buffer->cur_buf_length++] = '0'; break;
				case 0x4E: buffer->string_buffer[buffer->cur_buf_length++] = '-'; break;
				case 0x55: buffer->string_buffer[buffer->cur_buf_length++] = '='; break;

				case 0x5D: buffer->string_buffer[buffer->cur_buf_length++] = '\\'; break;
				case 0x15: buffer->string_buffer[buffer->cur_buf_length++] = 'q'; break;
				case 0x1D: buffer->string_buffer[buffer->cur_buf_length++] = 'w'; break;
				case 0x24: buffer->string_buffer[buffer->cur_buf_length++] = 'e'; break;
				case 0x2D: buffer->string_buffer[buffer->cur_buf_length++] = 'r'; break;
				case 0x2C: buffer->string_buffer[buffer->cur_buf_length++] = 't'; break;
				case 0x35: buffer->string_buffer[buffer->cur_buf_length++] = 'y'; break;
				case 0x3C: buffer->string_buffer[buffer->cur_buf_length++] = 'u'; break;
				case 0x43: buffer->string_buffer[buffer->cur_buf_length++] = 'i'; break;
				case 0x44: buffer->string_buffer[buffer->cur_buf_length++] = 'o'; break;
				case 0x4D: buffer->string_buffer[buffer->cur_buf_length++] = 'p'; break;
				case 0x54: buffer->string_buffer[buffer->cur_buf_length++] = '['; break;
				case 0x5B: buffer->string_buffer[buffer->cur_buf_length++] = ']'; break;

				case 0x1C: buffer->string_buffer[buffer->cur_buf_length++] = 'a'; break;
				case 0x1B: buffer->string_buffer[buffer->cur_buf_length++] = 's'; break;
				case 0x23: buffer->string_buffer[buffer->cur_buf_length++] = 'd'; break;
				case 0x2B: buffer->string_buffer[buffer->cur_buf_length++] = 'f'; break;
				case 0x34: buffer->string_buffer[buffer->cur_buf_length++] = 'g'; break;
				case 0x33: buffer->string_buffer[buffer->cur_buf_length++] = 'h'; break;
				case 0x3B: buffer->string_buffer[buffer->cur_buf_length++] = 'j'; break;
				case 0x42: buffer->string_buffer[buffer->cur_buf_length++] = 'k'; break;
				case 0x4B: buffer->string_buffer[buffer->cur_buf_length++] = 'l'; break;
				case 0x4C: buffer->string_buffer[buffer->cur_buf_length++] = ';'; break;
				case 0x52: buffer->string_buffer[buffer->cur_buf_length++] = '\''; break;
				case 0x1A: buffer->string_buffer[buffer->cur_buf_length++] = 'z'; break;
				case 0x22: buffer->string_buffer[buffer->cur_buf_length++] = 'x'; break;

				case 0x21: buffer->string_buffer[buffer->cur_buf_length++] = 'c'; break;
				case 0x2A: buffer->string_buffer[buffer->cur_buf_length++] = 'v'; break;
				case 0x32: buffer->string_buffer[buffer->cur_buf_length++] = 'b'; break;
				case 0x31: buffer->string_buffer[buffer->cur_buf_length++] = 'n'; break;
				case 0x3A: buffer->string_buffer[buffer->cur_buf_length++] = 'm'; break;
				case 0x41: buffer->string_buffer[buffer->cur_buf_length++] = ','; break;
				case 0x49: buffer->string_buffer[buffer->cur_buf_length++] = '.'; break;
				case 0x4A: buffer->string_buffer[buffer->cur_buf_length++] = '/'; break;
				case 0x29: buffer->string_buffer[buffer->cur_buf_length++] = ' '; break;
				case 0x5A: buffer->string_buffer[buffer->cur_buf_length++] = '\n'; break;
				// case 0x12: buffer[cur_buf_length++] = ''; break;
				// case 0x59: buffer[cur_buf_length++] = ''; break;
			}
		}
		else {
			switch (PS2_code) {
				case 0x0E: buffer->string_buffer[buffer->cur_buf_length++] = '`'; break;
				case 0x16: buffer->string_buffer[buffer->cur_buf_length++] = '1'; break;
				case 0x1E: buffer->string_buffer[buffer->cur_buf_length++] = '2'; break;
				case 0x26: buffer->string_buffer[buffer->cur_buf_length++] = '3'; break;
				case 0x25: buffer->string_buffer[buffer->cur_buf_length++] = '4'; break;
				case 0x2E: buffer->string_buffer[buffer->cur_buf_length++] = '5'; break;
				case 0x36: buffer->string_buffer[buffer->cur_buf_length++] = '6'; break;
				case 0x3D: buffer->string_buffer[buffer->cur_buf_length++] = '7'; break;
				case 0x3E: buffer->string_buffer[buffer->cur_buf_length++] = '8'; break;
				case 0x46: buffer->string_buffer[buffer->cur_buf_length++] = '9'; break;
				case 0x45: buffer->string_buffer[buffer->cur_buf_length++] = '0'; break;
				case 0x4E: buffer->string_buffer[buffer->cur_buf_length++] = '-'; break;
				case 0x55: buffer->string_buffer[buffer->cur_buf_length++] = '='; break;
								
				case 0x5D: buffer->string_buffer[buffer->cur_buf_length++] = '\\'; break;
				case 0x15: buffer->string_buffer[buffer->cur_buf_length++] = 'Q'; break;
				case 0x1D: buffer->string_buffer[buffer->cur_buf_length++] = 'W'; break;
				case 0x24: buffer->string_buffer[buffer->cur_buf_length++] = 'E'; break;
				case 0x2D: buffer->string_buffer[buffer->cur_buf_length++] = 'R'; break;
				case 0x2C: buffer->string_buffer[buffer->cur_buf_length++] = 'T'; break;
				case 0x35: buffer->string_buffer[buffer->cur_buf_length++] = 'Y'; break;
				case 0x3C: buffer->string_buffer[buffer->cur_buf_length++] = 'U'; break;
				case 0x43: buffer->string_buffer[buffer->cur_buf_length++] = 'I'; break;
				case 0x44: buffer->string_buffer[buffer->cur_buf_length++] = 'O'; break;
				case 0x4D: buffer->string_buffer[buffer->cur_buf_length++] = 'P'; break;
				case 0x54: buffer->string_buffer[buffer->cur_buf_length++] = '['; break;
				case 0x5B: buffer->string_buffer[buffer->cur_buf_length++] = ']'; break;
							
				case 0x1C: buffer->string_buffer[buffer->cur_buf_length++] = 'A'; break;
				case 0x1B: buffer->string_buffer[buffer->cur_buf_length++] = 'S'; break;
				case 0x23: buffer->string_buffer[buffer->cur_buf_length++] = 'D'; break;
				case 0x2B: buffer->string_buffer[buffer->cur_buf_length++] = 'F'; break;
				case 0x34: buffer->string_buffer[buffer->cur_buf_length++] = 'G'; break;
				case 0x33: buffer->string_buffer[buffer->cur_buf_length++] = 'H'; break;
				case 0x3B: buffer->string_buffer[buffer->cur_buf_length++] = 'J'; break;
				case 0x42: buffer->string_buffer[buffer->cur_buf_length++] = 'K'; break;
				case 0x4B: buffer->string_buffer[buffer->cur_buf_length++] = 'L'; break;
				case 0x4C: buffer->string_buffer[buffer->cur_buf_length++] = ';'; break;
				case 0x52: buffer->string_buffer[buffer->cur_buf_length++] = '\''; break;
				case 0x1A: buffer->string_buffer[buffer->cur_buf_length++] = 'Z'; break;
				case 0x22: buffer->string_buffer[buffer->cur_buf_length++] = 'X'; break;
							
				case 0x21: buffer->string_buffer[buffer->cur_buf_length++] = 'C'; break;
				case 0x2A: buffer->string_buffer[buffer->cur_buf_length++] = 'V'; break;
				case 0x32: buffer->string_buffer[buffer->cur_buf_length++] = 'B'; break;
				case 0x31: buffer->string_buffer[buffer->cur_buf_length++] = 'N'; break;
				case 0x3A: buffer->string_buffer[buffer->cur_buf_length++] = 'M'; break;
				case 0x41: buffer->string_buffer[buffer->cur_buf_length++] = ','; break;
				case 0x49: buffer->string_buffer[buffer->cur_buf_length++] = '.'; break;
				case 0x4A: buffer->string_buffer[buffer->cur_buf_length++] = '/'; break;
				case 0x29: buffer->string_buffer[buffer->cur_buf_length++] = ' '; break;
				case 0x5A: buffer->string_buffer[buffer->cur_buf_length++] = '\n'; break;
				// case 0x12: buffer[cur_buf_length++] = ''; break;
				// case 0x59: buffer[cur_buf_length++] = ''; break;
			}
		}
	
		buffer->string_buffer[buffer->cur_buf_length] = '\0';
	}
}

void read_PS2_data(int *make_code_flag, int *PS2_code) {
	int data;
	int static data_buf = 0;
	
	data = IORD(PS2_CONTROLLER_COMPONENT_0_BASE, 0);

	if(data_buf != data) {
		*make_code_flag = (data >> 8) & 0x1;
		*PS2_code = data & 0xFF;
	}
	data_buf = data;
}

// ISR when the a PS2 code is acquired
void handle_PS2_interrupts(buffer_ISR* buffer)
{
	int make_code_flag, PS2_code;
	// make sure it cap_on does not reset to 0 every time a function is called
	int static cap_on = 0;
	
	read_PS2_data(&make_code_flag, &PS2_code);

	if(make_code_flag == 0 && PS2_code == 0x58) {
		cap_on = ~cap_on;
		//alt_printf("cap lock check: %x \n", cap_on);
	}

	// make_code_flag == 1: pressed, make_code_flag == 0: released
	if (make_code_flag == 1) {
		translate_PS2_code(buffer->string_buffer, PS2_code, cap_on);
		buffer->buffer_flush = 1;
	}

	IOWR(PS2_CONTROLLER_COMPONENT_0_BASE, 0, 0);
}

// Function for initializing the ISR of the PS2 controller
// Get the struct buf_ptr address of the local variable in experiment2.c so it can be used in this file
// Pass the pointer pointing to the address of buf_ptr
void init_PS2_irq(buffer_ISR* buffer) {
	IOWR(PS2_CONTROLLER_COMPONENT_0_BASE, 0, 0);

	// give it a void pointer
	alt_irq_register(PS2_CONTROLLER_COMPONENT_0_IRQ, (void*)buffer, (void*)handle_PS2_interrupts );
}
