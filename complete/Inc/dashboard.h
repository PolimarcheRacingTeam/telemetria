/*
 * dashboard.c
 *
 *  Created on: Jun 6, 2017
 *      Author: Seb
 */
#ifndef DASHBOARD_H_
#define DASHBOARD_H_

#include "usart.h"
#include "cmsis_os.h"

#define DASH_UPDATE_PERIOD 100 //ms

struct datiCrusc {
  uint8_t id, rpm, gear, speed, engtemp, oilp, vbat;
} dc;

void dashboardThread(void const *argument) {

	dc.id = 204;
	while(1){
  //preparo il pacchetto per il cruscotto
/*
  dc.rpm      = dm.rpm;
  dc.gear     = dm.gear;
  dc.speed    = dm.speed;
  dc.engtemp  = dm.engtemp;
  dc.oilp     = dm.oilp;
  dc.vbat     = dm.vbat;

  //Cruscotto accetta pacchetti da 6 preceduti dal byte 204
  Serial3.write((char*)&dc, sizeof(dc)); //vs cruscotto
*/
		HAL_UART_Transmit(&huart2, (uint8_t *) &dc, sizeof(dc), HAL_MAX_DELAY);
		osDelay(DASH_UPDATE_PERIOD);
	}
	//non dovremmo mai arrivare qui
	osThreadTerminate(NULL);

}

#endif
