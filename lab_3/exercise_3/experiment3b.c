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

#define ARRAY_SIZE 120
#define ARRAY_SIZE_2 135

// For performance counter
void *performance_name = PERFORMANCE_COUNTER_0_BASE;

void swap(short int *x_ptr, short int *y_ptr) {
	short int temp = *x_ptr;
	*x_ptr = *y_ptr;
	*y_ptr = temp;
}
// *data_array == data_array[]
void bubble_sort_long(short int *data_array, int size)
{
	int i,j;
	for (i=0; i<size-1; i++) {
		for (j=0; j<size-i-1; j++) {
			if (data_array[j] > data_array[j+1]) {
				// &data_array[j] is the address of data_array[j]
				// the addresses pointers pointing to now is swapped
				swap(&data_array[j], &data_array[j+1]);
				///////////////////////////////////////
				// OR WRITE LIKE THIS:
				//int temp = data_array[j];
				//data_array[j] = data_array[j+1];
				//data_array[j+1] = temp;
			}
		}
	}
}

int mask (int size) {
	int result = (1 << size) - 1;
	return result;
}


void bubble_sort_short(char *data_array, int size)
{
	int i,j;
	char arr_var_1, arr_var_2,arr_var_3, arr_var_4;
	short int sec_1, sec_2, sec_3, sec_4;
	short int var1, var2;
	short int var1_ext, var2_ext;
	int lead = 8;
	int end = 1;

	for (j=0; j<size-1; j++){
		lead = 8;
		end = 1;
	for (i=0; i<size-j-1; i++) {
		arr_var_1 = data_array[i];
		arr_var_2 = data_array[i+1];
		arr_var_3 = data_array[i+2];

		sec_1 = arr_var_1 & mask(lead);
		sec_2 = (arr_var_2 >> (lead - 1)) & mask(end);

		if (lead == 1){
			arr_var_4 = data_array[i+3];
			sec_3 = arr_var_3;
			sec_4 = (arr_var_4 >> 7) & mask(1);
		}
		else {
			sec_3 = arr_var_2 & mask(lead - 1);
			sec_4 = (arr_var_3 >> (lead - 2)) & mask(end + 1);
		}

		var1 = ((sec_1 << end) + sec_2);
		if (lead != 1){
			var2 = ((sec_3 << (end + 1)) + sec_4);
		}
		else {
			var2 = ((sec_3 << 1) + sec_4);
		}
		//sign extension
		var1_ext = (var1 & 0x1FF) | ((var1 & 0x100) ? 0xFE00 : 0);
		var2_ext = (var2 & 0x1FF) | ((var2 & 0x100) ? 0xFE00 : 0);

		if ((var1_ext > var2_ext) && (lead != 1)){
			data_array[i] = (((data_array[i] >> lead) & mask(8 - lead)) << lead) | ((var2_ext >> (9 - lead)) & mask(lead));
			data_array[i+1] = ((var2_ext & mask(end)) << (lead - 1)) | (var1_ext >> (end + 1)) & mask(lead - 1);
			data_array[i+2] = (data_array[i+2] & mask(lead-2)) | ((var1_ext & mask(end + 1)) << (lead - 2));
			}
		if ((var1_ext > var2_ext) && (lead == 1)){
			data_array[i] = ( var2_ext >> 8 )& 0x1 | ((data_array[i] >> 1) & mask(7)) << 1;
			data_array[i+1] = (var2_ext >> 1) & mask(8);
			data_array[i+2] = (var1_ext >> 1) & mask(8);
			data_array[i+3] = ((var1_ext & 0x1) << 8) | (data_array[i+3] & mask(7));
		}

		if (lead == 1){
			i += 1;
			lead = 8;
			end = 1;
		}
		else {
			lead -= 1;
			end += 1;
		}
	}
}
}

int main()
{ 
	short int data_set_long[ARRAY_SIZE] = {};   // size: 120
	char data_set_short[ARRAY_SIZE] = {};  // size: 135
	int i,j;

	for (j=0; j<10; j++) {
		printf("Generating random data for uncompacted array...\n");
		for (i = 0; i < ARRAY_SIZE; i++) {
			data_set_long[i] = (rand() % 512) - 256;
		}

		printf("Generating random data for compacted array...\n");
		for (i = 0; i < ARRAY_SIZE_2; i++) {
			data_set_short[i] = (rand() % 512) - 256;
		}

		// Reset the performance counter
		PERF_RESET(PERFORMANCE_COUNTER_0_BASE);

		// Start the performance counter
		PERF_START_MEASURING(performance_name);
	
		printf("Start sorting uncompacted array...\n");
	
		// Start measuring code section
		PERF_BEGIN(performance_name, 1);
	
		bubble_sort_long(data_set_long, ARRAY_SIZE);
	
		// Stop measuring code section
		PERF_END(performance_name, 1);
	
		// Stop the performance counter
		PERF_STOP_MEASURING(performance_name);
	
		printf("PC for uncompacted: %d\n", perf_get_section_time(performance_name, 1));
	
		// Reset the performance counter
		PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
	
		// Start the performance counter
		PERF_START_MEASURING(performance_name);
	
		printf("Start sorting compacted array...\n");
	
		// Start measuring code section
		PERF_BEGIN(performance_name, 1);
	
		bubble_sort_short(data_set_short, ARRAY_SIZE_2);
	
		// Stop measuring code section
		PERF_END(performance_name, 1);
	
		// Stop the performance counter
		PERF_STOP_MEASURING(performance_name);

		printf("PC for compacted: %d\n", perf_get_section_time(performance_name, 1));
	}
  /* Event loop never exits. */
  while (1);

  return 0;
}
