// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

#include "define.h"

int main()
{
	// Create a volatile local variable buf_ptr with struct of buffer_ISR
	volatile buffer_ISR buf_ptr;
	printf("Start main...\n");

	buf_ptr.buffer_flush = 0;
	buf_ptr.cur_buf_length = 0;

	// Give address to PS2_controller so they can use this variable in a pointer way
	init_PS2_irq(&buf_ptr);
	printf("PS2 IRQ initialized...\n");

	IOWR(LED_GREEN_O_BASE, 0, 0x0);
	IOWR(LED_RED_O_BASE, 0, 0x0);
	
	printf("Switch value: %X\n", IORD(SWITCH_I_BASE, 0));
		
	while (1) {
		if (buf_ptr.buffer_flush == 1) {
			printf("%s", buf_ptr.string_buffer);
			buf_ptr.buffer_flush = 0;
			buf_ptr.string_buffer[0] = '\0';
			buf_ptr.cur_buf_length = 0;
		}
	};
	
	return 0;
}
