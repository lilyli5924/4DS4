#include "define.h"

void FindNext(int* array,floor_P *floor){
	int i,j;
	if (floor->direction == 1) {
		// sorting to find up
		for (i=0; i<ARRAY_SIZE_MAX-1; i++){
			if ((array[i] > floor->cur_floor) && (array[i] != 13)){
				floor->des_floor = array[i];
				IOWR(LED_RED_O_BASE, 0, (floor->led | (1<<floor->des_floor)));
			    // Deleting element
			    for (j = i; j < ARRAY_SIZE_MAX - 1; j++) {
			    	array[j] = array[j + 1];
			    	array[ARRAY_SIZE_MAX-1] = 13;
				}
			    floor->initial = 1;
				break;
			}

			floor->initial = 0;
		}
		// Not coexisting situation
		if (floor->initial == 0) {
			floor->direction = 0;
			for (i=ARRAY_SIZE_MAX-1; i>=0; i--){
				if ((array[i] < floor->cur_floor) && (array[i] != 13)){
					floor->des_floor = array[i];
					IOWR(LED_RED_O_BASE, 0, (floor->led | (1<<floor->des_floor)));
					// Deleting element
					for (j = i; j < ARRAY_SIZE_MAX - 1; j++) {
						array[j] = array[j+1];
					 	 array[ARRAY_SIZE_MAX-1] = 13;
					}
					floor->initial = 1;
					break;
				}
				floor->initial = 0;
			}
		}
	}
	else {
		// sorting to find down
		for (i=ARRAY_SIZE_MAX-1; i>=0; i--){
			if ((array[i] < floor->cur_floor) && (array[i] != 13)){
				floor->des_floor = array[i];
				IOWR(LED_RED_O_BASE, 0, (floor->led | (1<<floor->des_floor)));
				// Deleting element
			    for (j = i; j < ARRAY_SIZE_MAX - 1; j++) {
			    	array[j] = array[j+1];
			    	array[ARRAY_SIZE_MAX-1] = 13;
				}
			    floor->initial = 1;
				break;
			}
			floor->initial = 0;
		}
		if(floor->initial == 0) {
			floor->direction = 1;

			for (i=0; i<ARRAY_SIZE_MAX-1; i++){
				if ((array[i] > floor->cur_floor) && (array[i] != 13)){
					floor->des_floor = array[i];
					IOWR(LED_RED_O_BASE, 0, (floor->led | (1<<floor->des_floor)));
					// Deleting element
					for (j = i; j < ARRAY_SIZE_MAX - 1; j++) {
						array[j] = array[j + 1];
						array[ARRAY_SIZE_MAX-1] = 13;
					}
					floor->initial = 1;
					break;
				}
				floor->initial = 0;
			}
		}
	}
}

void elevator(floor_P * floor){
	IOWR(CUSTOM_COUNTER_COMPONENT_0_BASE, 2, 0);

	alt_irq_register(CUSTOM_COUNTER_COMPONENT_0_IRQ, (void*)floor, (void*)handle_move_expire_interrupts );
}

void opendoor(floor_P* floor, counter *PB_timer){
	//floor->state = 0;
	IOWR(CUSTOM_COUNTER_COMPONENT_1_BASE, 2, 0);
	alt_irq_register(CUSTOM_COUNTER_COMPONENT_1_IRQ, (void*)floor, (void*)handle_door_expire_interrupts );
	//floor->state = 1;

}
