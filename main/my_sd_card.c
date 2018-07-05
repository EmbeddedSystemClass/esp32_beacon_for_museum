/*
 * my_sd_card.c
 *
 *  Created on: Jun 14, 2018
 *      Author: hai_dotcom
 */

#include "my_sd_card.h"
#include "project_main.h"

extern xQueueHandle Http_Queue_Handle;

void sd_card_init()
{
	sdmmc_host_t host = SDMMC_HOST_DEFAULT();
	sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

	// GPIOs 15, 2, 4, 12, 13 should have external 10k pull-ups.
	// Internal pull-ups are not sufficient. However, enabling internal pull-ups
	// does make a difference some boards, so we do that here.

	gpio_set_pull_mode(15, GPIO_PULLUP_ONLY);   // CMD, needed in 4- and 1- line modes
	gpio_set_pull_mode(2, GPIO_PULLUP_ONLY);    // D0, needed in 4- and 1-line modes
	gpio_set_pull_mode(4, GPIO_PULLUP_ONLY);    // D1, needed in 4-line mode only
	gpio_set_pull_mode(12, GPIO_PULLUP_ONLY);   // D2, needed in 4-line mode only
	gpio_set_pull_mode(13, GPIO_PULLUP_ONLY);   // D3, needed in 4- and 1-line modes
    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
	esp_vfs_fat_sdmmc_mount_config_t mount_config = {
	        .format_if_mount_failed = false,
	        .max_files = 5,
	        .allocation_unit_size = 16 * 1024
	    };
	// Use settings defined above to initialize SD card and mount FAT filesystem.
	    // Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
	    // Please check its source code and implement error recovery when developing
	    // production applications.
	    sdmmc_card_t* card;
	    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

	    if (ret != ESP_OK) {
	        if (ret == ESP_FAIL) {
	            printf( "Failed to mount filesystem.\n");
	        } else {
	            printf("Failed to initialize the card (%d).\n",ret);
	        }
	    }
	    // Card has been initialized, print its properties
	    sdmmc_card_print_info(stdout, card);
}

/*Get object's name based on ID*/
char * get_name(char line[128])
{
	char * name = (char * )calloc(64,sizeof(char));
	bool flag = false;
	uint8_t j = 0;
	printf("get name \n");
	for (uint8_t i = 0;i<strlen(line)-2;i++){
		//#: begin of name in a received string - ex: ..ort.mp3__name#Mona Lisa
		if(line[i] != '#' && flag == false){
			continue;
		}
		else{
			if(line[i] == '#')
				flag = true;
			*(name+j) = line[i+1];
			j++;
		}
	}
	return name;
}

/*Get object's file based on ID*/
char * get_file(char line[128])
{
	char * file = (char*)calloc(16,sizeof(char));
	uint8_t start = strstr(line,"/") - line+1;
	uint8_t end = strstr(line,"!") - line;
	for(uint8_t i = start; i < end;i++){
		*(file+i) = line[i];
	}
	//printf("Play: %s \n",file);
	return file;
}

data check_database(char id[9])
{
	char line[128];
	data data;
	data.check = false;
	data.name ="";
	data.file="";
	data.url="";
	FILE* database= fopen("/sdcard/database.txt", "r");
	if (database==NULL){
		printf("Failed to open file for reading\n");
		fclose(database);
		return data;
	}
	else{
		printf("open file \n");
		while(fgets(line,sizeof(line),database)!=NULL){
			printf(line);
			/*Check ID in file*/
			if(strstr(line,id)!=NULL){
				data.check = true;
				data.name = get_name(line);
				//data.file = get_file(line);
				data.file = "/sdcard/MON.mp3";
				data.url = "http://www.hubharp.com/web_sound/BachGavotteShort.mp3";
				printf("name: %s\n",data.name);
				break;
			}
		}
		FILE *file = fopen(data.file,"r");
		if(data.check == true && file == NULL){
			if( ! xQueueSend(Http_Queue_Handle,&data,portMAX_DELAY)){
				printf("Failed to send \n");
			}
			fclose(file);
		}

	}
	fclose(database);
	return data;
}
