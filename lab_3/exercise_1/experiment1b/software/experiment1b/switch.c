#include "define.h"

void insertSort(int *floor_buf, int key){
		int i;
	    for (i = ARRAY_SIZE_MAX - 1; (i >= 0 && floor_buf[i] > key); i--){
	    	floor_buf[i+1] = floor_buf[i];
	    }
	    floor_buf[i + 1] = key;
}

void handle_switch_interrupt(int *floor_buf){
	int sw_grp;
	int led = IORD(LED_RED_O_BASE, 0);
	sw_grp = IORD(SWITCH_I_BASE, 3);
	int i;
	for (i=0; i<12; i++){
		if (((sw_grp >> i) & 0x1) == 1){
			insertSort(floor_buf,i);
			break;
		}
	}
	printf("ARRAY is: \n");
	for(i = 0; i < 12; i++){
		printf("%d, ", floor_buf[i]);
	}
	printf("\n");

	IOWR(SWITCH_I_BASE, 3, 0x0);

	for (i=0;i<ARRAY_SIZE_MAX;i++){
		if (floor_buf[i] != 13) {
			led = led | (0x1<< floor_buf[i]);
		}
	}
	IOWR(LED_RED_O_BASE, 0, led);

}


void init_switch_irq(int *floor_buf) {
	IOWR(SWITCH_I_BASE, 3, 0x0); // edge capture register
	IOWR(SWITCH_I_BASE, 2, 0xFFF); // IRQ mask
	alt_irq_register(SWITCH_I_IRQ, (void*)floor_buf, (void*)handle_switch_interrupt);
	/*for (i=0;i<ARRAY_SIZE_MAX;i++){
		if (floor_buf[i] != 13) {
			led = led | (0x1<< floor_buf[i]);
		}
	}
	IOWR(LED_RED_O_BASE, 0, led);*/

}
