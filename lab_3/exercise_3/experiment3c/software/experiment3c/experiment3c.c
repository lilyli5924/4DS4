// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "sys/alt_alarm.h"
#include "alt_types.h"
#include "system.h"
#include "altera_avalon_performance_counter.h"

#define ARRAY_SIZE 500

// For performance counter
void *performance_name = PERFORMANCE_COUNTER_0_BASE;

void swap(int *x_ptr, int *y_ptr) {
	int temp = *x_ptr;
	*x_ptr = *y_ptr;
	*y_ptr = temp;
}

void bubble_sort(int *data_array, int size)
{	
	// Bubble sort: compare the adjacent elements values and swap them in the correct order.
	// i.e.
	// 5 1 4 2 8 --> 1 5 4 2 8 --> 1 4 5 2 8 --> 1 4 2 5 8 --> 1 2 4 5 8
	// *array: value of array (?)
	// array: pointer to the first element in the arra
	// &array: pointer to the whole array of size elements
	// array = &array[0]
	int i,j;
	for (i=0; i<size-1; i++) {
		for (j=0; j<size-i-1; j++) {
			if (data_array[j] > data_array[j+1]) {
				swap(&data_array[j], &data_array[j+1]);
			}
		}
	}
}

int main()
{ 
	int data_set[ARRAY_SIZE];
	int i;
	
	printf("Generating random data...\n");
	for (i = 0; i < ARRAY_SIZE; i++) {
		data_set[i] = rand() % 65536;
	}
	
	// Reset the performance counter
	PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
	
	// Start the performance counter
	PERF_START_MEASURING(performance_name);
	
	printf("Start sorting...\n");
	
	// Start measuring code section
	PERF_BEGIN(performance_name, 1);
	
	bubble_sort(data_set, ARRAY_SIZE);
	
	// Stop measuring code section
	PERF_END(performance_name, 1);

	// Stop the performance counter
	PERF_STOP_MEASURING(performance_name);
	
	printf("PC: %d\n", perf_get_section_time(performance_name, 1));
	
  /* Event loop never exits. */
  while (1);

  return 0;
}
