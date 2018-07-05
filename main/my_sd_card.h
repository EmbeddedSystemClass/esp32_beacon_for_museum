/*
 * my_sd_card.h
 *
 *  Created on: Jun 14, 2018
 *      Author: hai_dotcom
 */

#ifndef MAIN_MY_SD_CARD_H_
#define MAIN_MY_SD_CARD_H_

/*Declare library*/
#include "stdbool.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include <sys/unistd.h>
#include <sys/stat.h>
#include "project_main.h"

/*Prototype*/
void sd_card_init();
data check_database(char id[9]);
char *get_name(char name[64]);

#endif /* MAIN_MY_SD_CARD_H_ */
