// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

#include "define.h"


int main()
{	
	volatile counter PB_timer;
	volatile floor_P plan;
	int floor_list[ARRAY_SIZE_MAX] = {13,13,13,13,13,13,13,13,13,13,13,13};

	plan.cur_floor = 0;
	plan.des_floor = 0; // Initially at lobby
	plan.direction = 1; // Initially going up
	plan.initial = 0;
	plan.hold_1 = 0;
	plan.state = 1;
	plan.led = 0;

	PB_timer.door_open = 0;
	PB_timer.elevator_move = 0;
	PB_timer.hold = 0;
	int load = 1;
	printf("Start main...\n");


	init_button_irq(&PB_timer);
	printf("PB initialized...\n");

	//initial switch interrupt
	init_switch_irq(&floor_list);
	printf("Switch IRQ initialized...\n");

	IOWR(LED_GREEN_O_BASE, 0, 0x0);
	IOWR(LED_RED_O_BASE, 0, 0x0);
	
	printf("Switch value: %X\n", IORD(SWITCH_I_BASE, 0));
	IOWR(SEVEN_SEGMENT_N_O_0_BASE, 0, 0x40);
		
	while (1){
		plan.hold_1 = PB_timer.hold;
		if((plan.initial == 0) && (((floor_list[0] != 13)) ||((floor_list[0] == 13) && (load == 0)))) {
			if(load == 1) {
				load_counter_config(&PB_timer.elevator_move);
				load = 0;
				plan.des_floor = floor_list[0];
				floor_list[0] = 13;
			}

			if ((plan.state == 1) && (read_move_interrupt() == 1) && (PB_timer.hold == 0)&& (read_door_interrupt() == 1)) {
				reset_move_counter();
				elevator(&plan);
			}
			else if(plan.cur_floor == plan.des_floor){
				if (load == 0){
					load = 1;
					load_door_config(&PB_timer.door_open);
				}
				printf("Door open \n");
				//plan.state = 0;
				plan.led = IORD(LED_RED_O_BASE, 0);
				plan.led = plan.led & (~(1 << plan.des_floor));
				IOWR(LED_RED_O_BASE, 0, plan.led);

				reset_door_counter();
				opendoor(&plan, &PB_timer);

				//printf("current floor = %d, destination floor= %d, direction = %d \n", plan.cur_floor, plan.des_floor, plan.direction);
				FindNext(&floor_list, &plan);
			}
		}
		else if (plan.initial == 1) {
			if(load == 1) {
				load = 0;
			}
			if ((plan.state == 1) && (read_move_interrupt() == 1) && (PB_timer.hold == 0) && (read_door_interrupt() == 1)) {
				reset_move_counter();
				elevator(&plan);
				//printf("current floor = %d, destination floor= %d, direction = %d \n", plan.cur_floor, plan.des_floor, plan.direction);
			}
			else if (plan.cur_floor == plan.des_floor){
				if (load == 0){
					load = 1;
				}
				//plan.state = 0;
				printf("Door open \n");
				plan.led = IORD(LED_RED_O_BASE, 0);
				plan.led = plan.led & (~(1 << plan.des_floor));
				IOWR(LED_RED_O_BASE, 0, plan.led);

				reset_door_counter();
				opendoor(&plan, &PB_timer);

				//printf("current floor = %d, destination floor = %d, direction = %d \n", plan.cur_floor, plan.des_floor, plan.direction);
				FindNext(&floor_list, &plan);
			}
		}
	}
	return 0;
}
