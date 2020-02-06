// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

#include "define.h"

void KEY0_Pressed() {
	//reset_counter();
}

void KEY1_Pressed(int *open) {//upload the time door open
	//load_counter_config((IORD(SWITCH_I_BASE, 0) & 0x18000) >> 15);
	*open = (IORD(SWITCH_I_BASE, 0) & 0x18000) >> 15;
}

void KEY2_Pressed(int *move) {//upload the time takes
	//load_counter_config((IORD(SWITCH_I_BASE, 0) & 0x18000) >> 15);
	//printf("Counter value = %d\n", read_counter());
	*move = (IORD(SWITCH_I_BASE, 0) & 0x18000) >> 15;

}

void KEY3_Pressed(int *hold) {
	int static button_3 = 0;
	button_3 = button_3 ^ 0x1;
	*hold = button_3;
	if(button_3 == 1){
		printf("You are holding the door \n");
	} else {
		reset_door_counter();
	}
}

// ISR when any PB is pressed
void handle_button_interrupts(counter* PB_timer)
{
	IOWR(LED_GREEN_O_BASE, 0, IORD(PUSH_BUTTON_I_BASE, 3)*IORD(PUSH_BUTTON_I_BASE, 3));

	switch(IORD(PUSH_BUTTON_I_BASE, 3)) {
	case 1: KEY0_Pressed(); break;
	case 2: KEY1_Pressed(&PB_timer->door_open); break;
	case 4: KEY2_Pressed(&PB_timer->elevator_move); break;
	case 8: KEY3_Pressed(&PB_timer->hold); break;
	}


	IOWR(PUSH_BUTTON_I_BASE, 3, 0x0);
}

// Function for initializing the ISR of the PBs
// The PBs are setup to generate interrupt on falling edge,
// and the interrupt is captured when the edge comes
void init_button_irq(counter* PB_timer) {
  // Enable all 4 button interrupts
  IOWR(PUSH_BUTTON_I_BASE, 2, BUTTON_INT_MASK);

  // Reset the edge capture register
  IOWR(PUSH_BUTTON_I_BASE, 3, 0x0);

  // Register the interrupt handler
  alt_irq_register(PUSH_BUTTON_I_IRQ, (void*)PB_timer, (void*)handle_button_interrupts );
}
