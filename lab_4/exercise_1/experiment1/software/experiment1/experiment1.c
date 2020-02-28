// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

#include "define.h"

int main()
{
	alt_printf("Start main...\n");

	IOWR(LED_GREEN_O_BASE, 0, 0x0);
	IOWR(LED_RED_O_BASE, 0, 0x0);

	init_button_irq();
	alt_printf("PB initialized...\n");
	
	init_counter_irq();
	alt_printf("Counter IRQ initialized...\n");
	
	alt_printf("System is up!\n");

	IOWR(LED_RED_O_BASE, 0, 0xF);
		
	while (1);
	
	return 0;
}
