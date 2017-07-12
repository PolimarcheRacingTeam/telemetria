/*
 * telemetry.h
 *
 *  Created on: Jun 6, 2017
 *      Author: Seb
 */

#ifndef TELEMETRY_H_
#define TELEMETRY_H_
#include "lwip/api.h"
#include "retarget.h"
#include "string.h"
#include "databuffer.h"

#define TELEMETRY_PORT 2017
#define TELEMETRY_PERIOD 100 //milliseconds

struct udp_pcb* pcb;
err_t err;

osEvent  evt;

char * packet;
uint16_t packet_length;

static struct netconn *conn;
static struct netbuf *buf;
uint8_t telecnt = 0;

static void telemetryThread(void const *arg){

	MX_LWIP_Init();
	printf("IP ready\n");
	IPready = 1;

	conn = netconn_new( NETCONN_UDP );
	//netconn_bind(conn, IP_ADDR_ANY, TELEMETRY_PORT ); //local port

	err = netconn_connect(conn, IP_ADDR_BROADCAST, TELEMETRY_PORT );
	if (err != ERR_OK){
		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin,1);
		osThreadTerminate(NULL);
	}

	teleData* telemsg;

	while(1){
		teleFlag = 1;
		evt = osMessageGet(teleMsgBox, osWaitForever);  // wait for message
		if(!(telecnt%5))
			HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		telecnt ++;
		if (evt.status == osEventMessage) {
			telemsg = evt.value.p;
			buf = netbuf_new();
			packet = netbuf_alloc(buf, sizeof(teleData));
			memcpy (packet, telemsg, sizeof (teleData));
			osPoolFree(telePool, telemsg);
			netconn_send(conn, buf);
			netbuf_delete(buf); // De-allocate packet buffer
		}
		osDelay(TELEMETRY_PERIOD);
	}

	osThreadTerminate(NULL);

}

#endif /* TELEMETRY_H_ */
