////////////////////////
// CODE SECTION BEGIN //
////////////////////////

#include "define.h"

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


// ISR when the counter is expired
void handle_move_expire_interrupts(floor_P* current_floor)
{
	if(current_floor->cur_floor < current_floor->des_floor) {
		current_floor->cur_floor = current_floor->cur_floor + 1;
	}

	if (current_floor->cur_floor > current_floor->des_floor) {
		current_floor->cur_floor = current_floor->cur_floor - 1;
	}
	if (current_floor->cur_floor == current_floor->des_floor)
		current_floor->state = 0;

	IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0, disp_seven_seg((current_floor->cur_floor)));
	printf("Moving!, current floor is %d \n",current_floor->cur_floor);

	IOWR(CUSTOM_COUNTER_COMPONENT_0_BASE, 2, 0);
}

// ISR when the door counter is expired
void handle_door_expire_interrupts(floor_P* current_floor)
{
	if(current_floor->hold_1 == 0) {
		printf("Door closed. \n");
	}
	current_floor->state = 1;


	IOWR(CUSTOM_COUNTER_COMPONENT_1_BASE, 2, 0);
}

void reset_move_counter() {
	IOWR(CUSTOM_COUNTER_COMPONENT_0_BASE, 1, 1);
	IOWR(CUSTOM_COUNTER_COMPONENT_0_BASE, 1, 0);

	IOWR(CUSTOM_COUNTER_COMPONENT_0_BASE, 2, 0);

}

void reset_door_counter() {
	IOWR(CUSTOM_COUNTER_COMPONENT_1_BASE, 1, 1);
	IOWR(CUSTOM_COUNTER_COMPONENT_1_BASE, 1, 0);

	IOWR(CUSTOM_COUNTER_COMPONENT_1_BASE, 2, 0);

}

int read_counter() {
	return IORD(CUSTOM_COUNTER_COMPONENT_0_BASE, 0);
}

int read_move_interrupt() {
	return IORD(CUSTOM_COUNTER_COMPONENT_0_BASE, 2);
}

int read_door_interrupt() {
	return IORD(CUSTOM_COUNTER_COMPONENT_1_BASE, 2);
}

void load_counter_config(int* config) {
	printf("Loading counter config %d\n", *config);

	IOWR(CUSTOM_COUNTER_COMPONENT_0_BASE, 3, *config);
}

void load_door_config(int* config) {
	printf("Loading counter config %d\n", *config);

	IOWR(CUSTOM_COUNTER_COMPONENT_1_BASE, 3, *config);
}

// Function for initializing the ISR of the Counter
//void init_counter_irq(floor_P *current_floor) {
//	IOWR(CUSTOM_COUNTER_COMPONENT_0_BASE, 2, 0);

//	alt_irq_register(CUSTOM_COUNTER_COMPONENT_0_IRQ, (void*)current_floor, (void*)handle_counter_expire_interrupts );
//}

////////////////////////
// CODE SECTION END   //
////////////////////////
