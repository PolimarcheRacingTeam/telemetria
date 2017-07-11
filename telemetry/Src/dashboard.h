/*
 * dashboard.c
 *
 *  Created on: Jun 6, 2017
 *      Author: Seb
 *
 *      https://community.st.com/thread/14092
 *      se torna il bug per cui il dma invia una volta sola.
 */
#ifndef DASHBOARD_H_
#define DASHBOARD_H_

#include "usart.h"
#include "cmsis_os.h"
#include "stm32f7xx_hal_uart.h"
#include "databuffer.h"

#define DASH_UPDATE_PERIOD 100 //ms

uint8_t DASH_HEADER = 204;
HAL_StatusTypeDef ret;
osEvent  dashevt;

int txcplt = 1;

dashData dc;

void dashThread(void const *argument) {
	dc.header = DASH_HEADER;
	dashData* dashmsg;
	while(1){
		dashFlag = 1;
		dashevt = osMessageGet(dashMsgBox, osWaitForever);  // wait for message
		if (dashevt.status == osEventMessage) {

			dashmsg = dashevt.value.p;

			dc.rpm = dashmsg->rpm;
			dc.gear = dashmsg->gear;
			dc.speed = dashmsg->speed;
			dc.engtemp = dashmsg->engtemp;
			dc.oilp = dashmsg->oilp;
			dc.vbat = dashmsg->vbat;
			dc.lambda = dashmsg->lambda;
			dc.oilt = dashmsg->oilt;

			HAL_UART_Transmit_DMA(&huart7, (uint8_t *) &dc, sizeof(dc));
			osDelay(DASH_UPDATE_PERIOD);
		}

	}
	osThreadTerminate(NULL);
}

#endif
