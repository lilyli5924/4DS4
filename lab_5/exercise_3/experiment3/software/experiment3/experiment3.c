// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

// This is the embedded software for the
// LCD / Camera design

#include "define.h"

int main() 
{
    // alt_up_sd_card_dev *sd_card_dev = NULL;
    PB_irq_data_struct PB_irq_data;
    short int file_handle;
    int i, j;
    int connected = 0;
    int status = 0;
    unsigned short int data;
    char filename[13];
    int file_number = 0;
    
    PB_irq_data.load_img_flag = 0;
    PB_irq_data.save_img_flag = 0;
    
    // sd_card_dev = alt_up_sd_card_open_dev("/dev/SD_card_0");
    sd_card_open_dev();
    
    alt_irq_register(NIOS_LCD_CAMERA_COMPONENT_0_TOUCHPANEL_IRQ, NULL, (void *)LTC_TouchPanel_int);
    init_button_irq(&PB_irq_data);
    
    printf("Experiment 3!\n");

    // initialize the touch panel
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_TOUCHPANEL_BASE, 2, 0x0);
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_TOUCHPANEL_BASE, 1, 0x400000);

    // initialize the camera
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 0, 0x0400);
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 1, 0x2);
    while ((IORD(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 1) & 0x1) == 0);
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 1, 0x4);

    // initialize the buttons
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CONSOLE_BASE, 1, 0x0);
    
    // initialize the filter pipe
    IOWR(NIOS_LCD_CAMERA_COMPONENT_0_IMAGELINE_BASE, 4, 0);

        while(1) { 
            if ((connected == 0) && (sd_card_is_Present())) {
                printf("Card connected.\n"); 
                connected = 1; 
                if (sd_card_is_FAT16()) {
                    printf("Valid file system detected.\n"); 
                } else {
                    printf("Unknown file system.\n");
                }
                status = sd_card_find_first(".", PB_irq_data.filename);
                
                switch (status) {
                    case 0: printf("Success\n"); break;
                    case 1: printf("Invalid directory\n"); break;
                    case 2: printf("No card or incorrect FS\n"); break;
                }
            } else if ((connected == 1) && (sd_card_is_Present() == false)) {
                printf("Card disconnected.\n"); connected = 0;
            }
            
            if (PB_irq_data.load_img_flag == 1) {
                // stop the camera
                IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 1, 0x8);
                
                file_handle = sd_card_fopen(PB_irq_data.filename, false);
                
                if (status < 0) printf("Error opening file \"%s\"\n", PB_irq_data.filename);
                else {
                    printf("Loading image data from file \"%s\"\n", PB_irq_data.filename);

                    // take out header from BMP (54 bytes)
                    for (i = 0; i < 54; i++) {
                        data = sd_card_read(file_handle);
                        if ((data >> 8) != 0) printf("eof reached in header\n");
                    }
                    LTC_Switch_Nios_Mode(1);

                    for (i = 0; i < NUM_LINES; i++) {
                    	if (i < 120 || i > 359) {
                    		for (j = 0; j < LINE_LEN; j++) {
                    			IOWR(LED_RED_O_BASE, 0, i*640+j);
                    			R_vals[j] = 0x0 & 0xFF;
                    			G_vals[j] = 0x0 & 0xFF;
                    			B_vals[j] = 0x0 & 0xFF;
                    			if (j % 100 == 160) printf("R eof reached: %d, %d\n", i, j);
                    		}
                    		LTC_Write_Image_Line(R_vals, G_vals, B_vals);
                    	}
                    	else {
                    		for (j = 0; j < LINE_LEN; j++) {
                    			if (j < 160 || j >= 480) {
                    				IOWR(LED_RED_O_BASE, 0, i*640+j);
                    				R_vals[j] = 0x0 & 0xFF;
                    				G_vals[j] = 0x0 & 0xFF;
                    				B_vals[j] = 0x0 & 0xFF;
                    			}
                    			else {
                    				IOWR(LED_RED_O_BASE, 0, i*640+j);
                    				data = sd_card_read(file_handle);
                    				// Change the read endian from big to little
                    				//if ((data >> 8) != 0) printf("B eof reached: %d, %d\n", i, j);
                    				B_vals[j] = data & 0xFF;

                    				data = sd_card_read(file_handle);
                    				//if ((data >> 8) != 0) printf("G eof reached: %d, %d\n", i, j);
                    				G_vals[j] = data & 0xFF;

                    				data = sd_card_read(file_handle);
                    				//if ((i == 120 || i == 121)) printf("R eof reached: %d, %d, data is %d \n", i, j, data);
                    				R_vals[j] = data & 0xFF;
                    			}
                    		}

                    		LTC_Write_Image_Line(R_vals, G_vals, B_vals);
                    	}
                    }
                    LTC_Switch_HW_Mode();
                    
                    sd_card_fclose(file_handle);
                }
                
                PB_irq_data.load_img_flag = 0;
            }
            
            if (PB_irq_data.save_img_flag == 1) {
                // stop the camera
                IOWR(NIOS_LCD_CAMERA_COMPONENT_0_CAMERA_BASE, 1, 0x8);
                
                sprintf(filename, "file%d.bmp", file_number++);
                
                while ((file_handle = sd_card_fopen(filename, 1)) < 0) {
                    sprintf(filename, "file%d.bmp", file_number++);
                }
                
                printf("Saving image to file (%s)...\n", filename);
                LTC_Switch_Nios_Mode(0);

                // BMP header
                status = sd_card_write(file_handle, 0x42); //B
                status = sd_card_write(file_handle, 0x4D); //M

                status = sd_card_write(file_handle, 0x36); //size
                status = sd_card_write(file_handle, 0x10);
                status = sd_card_write(file_handle, 0xE);
                status = sd_card_write(file_handle, 0x0);

                status = sd_card_write(file_handle, 0x0); //Reserved
                status = sd_card_write(file_handle, 0x0);

                status = sd_card_write(file_handle, 0x0); //Reserved
                status = sd_card_write(file_handle, 0x0);

                status = sd_card_write(file_handle, 0x36); //Offset
                status = sd_card_write(file_handle, 0x0);
                status = sd_card_write(file_handle, 0x0);
                status = sd_card_write(file_handle, 0x0);

                status = sd_card_write(file_handle, 0x28); //# of bytes in header
                status = sd_card_write(file_handle, 0x0);
                status = sd_card_write(file_handle, 0x0);
                status = sd_card_write(file_handle, 0x0);

                status = sd_card_write(file_handle, 0x40); //320 40
                status = sd_card_write(file_handle, 0x1); // 1
                status = sd_card_write(file_handle, 0x0); // 0
                status = sd_card_write(file_handle, 0x0); // 0

                status = sd_card_write(file_handle, 0x10); //-240
                status = sd_card_write(file_handle, 0xFF);
                status = sd_card_write(file_handle, 0xFF);
                status = sd_card_write(file_handle, 0xFF);

                status = sd_card_write(file_handle, 0x1); //Number of color plane
                status = sd_card_write(file_handle, 0x0);

                status = sd_card_write(file_handle, 0x18); //# of bits/pixel
                status = sd_card_write(file_handle, 0x0);

                status = sd_card_write(file_handle, 0x0); //BI_RGB
                status = sd_card_write(file_handle, 0x0);
                status = sd_card_write(file_handle, 0x0);
                status = sd_card_write(file_handle, 0x0);

                status = sd_card_write(file_handle, 0x0); //Size of raw BMP data
                status = sd_card_write(file_handle, 0x10);
                status = sd_card_write(file_handle, 0xE);
                status = sd_card_write(file_handle, 0x0);

                status = sd_card_write(file_handle, 0x13); //horizontal resolution
                status = sd_card_write(file_handle, 0xB);
                status = sd_card_write(file_handle, 0x0);
                status = sd_card_write(file_handle, 0x0);

                status = sd_card_write(file_handle, 0x13); //vertical resolution
                status = sd_card_write(file_handle, 0xB);
                status = sd_card_write(file_handle, 0x0);
                status = sd_card_write(file_handle, 0x0);

                status = sd_card_write(file_handle, 0x0); //# of colors in the palette
                status = sd_card_write(file_handle, 0x0);
                status = sd_card_write(file_handle, 0x0);
                status = sd_card_write(file_handle, 0x0);

                status = sd_card_write(file_handle, 0x0); //Means all colors are important
                status = sd_card_write(file_handle, 0x0);
                status = sd_card_write(file_handle, 0x0);
                status = sd_card_write(file_handle, 0x0);

                for (i = 1; i <= NUM_LINES; i++) {
                	LTC_Read_Image_Line(R_vals, G_vals, B_vals);
                	if (i % 2 == 1) {
                		continue;
                	}
                    for (j = 1; j <= LINE_LEN; j+=2) {
                    	IOWR(LED_RED_O_BASE, 0, i*640+j);
                    	// Change the endian from big to little
                    	status = sd_card_write(file_handle, B_vals[j]);
                    	if (!status) printf("Write fail: B %d %d\n", i, j);
                    	status = sd_card_write(file_handle, G_vals[j]);
                    	if (!status) printf("Write fail: G %d %d\n", i, j);
                    	status = sd_card_write(file_handle, R_vals[j]);
                    	if (!status) printf("Write fail: R %d %d\n", i, j);
                    }
                }
                LTC_Switch_HW_Mode();
                
                sd_card_fclose(file_handle);

                PB_irq_data.save_img_flag = 0;
            }
        }
    
    return 0;
}
