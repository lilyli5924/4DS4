// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

#ifndef __CUSTOM_COUNTER_H__
#define __CUSTOM_COUNTER_H__

// Global functions
void handle_move_expire_interrupts();
void handle_door_expire_interrupts();
void reset_door_counter();
int read_move_counter();
void reset_move_counter();
int read_move_interrupt();
int read_door_interrupt();
void init_counter_irq();
void load_counter_config(int*);
void load_door_config (int*);
alt_u16 disp_seven_seg(alt_u8);

#endif
