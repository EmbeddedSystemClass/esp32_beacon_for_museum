/*
 * http_download.h
 *
 *  Created on: Jul 3, 2018
 *      Author: hai_dotcom
 */

#ifndef MAIN_HTTP_DOWNLOAD_H_
#define MAIN_HTTP_DOWNLOAD_H_

/*Declare library*/
#include "project_main.h"
#include "my_sd_card.h"

//http request library
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

/*Define*/
#define WEB_SERVER "www.hubharp.com"
#define WEB_PORT 80
#define WEB_URL "http://www.hubharp.com/web_sound/BachGavotteShort.mp3"

#define WIFI_SSID	"Hai Dotcom"
#define WIFI_PASS 	"doremonnobita"

/*Prototype*/
void http_download_task(void *pvParameters);
void initialise_wifi(void);
esp_err_t event_handler(void *ctx, system_event_t *event);

#endif /* MAIN_HTTP_DOWNLOAD_H_ */
