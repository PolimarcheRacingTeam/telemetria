/*
 * sampling.h
 *
 *  Created on: Jul 10, 2017
 *      Author: Seb
 */

#ifndef SAMPLING_H_
#define SAMPLING_H_

#include "adc.h"
#include "imu.h"
#include "databuffer.h"

#define ADC_PERIOD 10 // ms between samples

__IO uint16_t rawADC[6];
ECUData ed;
uint32_t now, last;
HAL_StatusTypeDef canret;

/* thread per campionare i canali analogici */
static void samplingThread(void const *arg){

	if (HAL_ADC_Start_DMA(&hadc3, (uint32_t*)rawADC, 6) != HAL_OK){
		printf("ADC Start failed\n");
		Error_Handler();
	}
	printf("ADC ready\n");
	imuInit();

	while(!loggingReady)
		osDelay(100);

	ADCData* adcmsg;
	IMUData* imumsg;
	ECUData* ecumsg;
	teleData* telemsg;
	dashData* dashmsg;

	last = HAL_GetTick();
	while(1){
		now = HAL_GetTick();
		if (now%10) {
			printf("late\n");
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
			do{
				osDelay(1);
				now = HAL_GetTick();
			}while(now%10);
		} else {
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
		}
		last = now;

		imumsg = osPoolAlloc(imuPool);
		ecumsg = osPoolAlloc(ecuPool);
		adcmsg = osPoolAlloc(adcPool);

		for(int i = 0; i < 6; i++){
			adcmsg->A[i] = rawADC[i];
		}

		adcmsg->millis = now;
		uint32_t id;
		uint8_t flags = 0;

		while(1){
			canret = HAL_CAN_Receive(&hcan1,CAN_FIFO0, 0) ;
			if (canret == HAL_TIMEOUT) break;
			//if (flags == 15) break;
			id = hcan1.pRxMsg->StdId;
			if(id==0x002){
				format(ed.id2, hcan1.pRxMsg->Data, 8);
				//printf("a");
				flags |= 1;
			} else if (id == 0x003){
				format(ed.id3, hcan1.pRxMsg->Data, 8);
				flags |= 2;

				//printf("b");
			} else if (id == 0x004){
				format(ed.id4, hcan1.pRxMsg->Data, 8);
				flags |= 4;

				//printf("c");
			} else if(id == 0x005){
				format(ed.id5, hcan1.pRxMsg->Data, 8);
				flags |= 8;
				//printf("d");
			} else {
				//printf("z");
			}

		}
		//printf("\n");


		if(mpu_get_accel_reg(imumsg->a, NULL)){
			imumsg->ax = 0;
			imumsg->ay = 0;
			imumsg->az = 0;
		}
		osMessagePut(imuMsgBox, (uint32_t)imumsg, osWaitForever);
		memcpy(ecumsg, &ed, sizeof(ECUData));
		//if(ecumsg->gear == 0) while(1);
		osMessagePut(ecuMsgBox, (uint32_t)ecumsg, osWaitForever);
		osMessagePut(adcMsgBox, (uint32_t)adcmsg, osWaitForever);

		if(teleFlag){
			teleFlag = 0;

			telemsg = osPoolAlloc(telePool);

			telemsg->rpm = ed.rpm / 100;
			telemsg->map = ed.map;
			telemsg->lambda = ed.lambda;
			telemsg->tps = ed.tps;
			telemsg->engtemp = ed.engtemp / 10;

			telemsg->vbat = ed.vbat / 10;
			telemsg->oilp = ed.oilp / 100;
			telemsg->oilt = ed.oilt;
			telemsg->gear = ed.gear;
			telemsg->bse = ed.bse;

			telemsg->rearbrake = adcmsg->rearbrake;
			telemsg->fr = adcmsg->fr;
			telemsg->fl = adcmsg->fl;
			telemsg->rr = adcmsg->rr;
			telemsg->rl = adcmsg->rl;

			telemsg->speed = ed.speed / 10;
			telemsg->steer = adcmsg->A[0];
			telemsg->a5 = 0;
			telemsg->a6 = 0;
			telemsg->a7 = 0;

			telemsg->az = imumsg->az;
			telemsg->ax = imumsg->ax;
			telemsg->ay = imumsg->ay;

			osMessagePut(teleMsgBox, (uint32_t)telemsg, osWaitForever);
		}

		if(dashFlag){
			dashFlag = 0;

			dashmsg = osPoolAlloc(dashPool);

			dashmsg->rpm = ed.rpm / 100;
			dashmsg->gear = ed.gear;
			dashmsg->lambda = ed.lambda >> 5;
			dashmsg->speed = ed.speed / 10;
			dashmsg->engtemp = ed.engtemp / 10;
			dashmsg->vbat = ed.vbat/ 10;
			dashmsg->oilp = ed.oilp  / 100;
			dashmsg->oilt = ed.oilt/ 10;

			osMessagePut(dashMsgBox, (uint32_t)dashmsg, osWaitForever);
		}

		osDelay(10);
	}
	osThreadTerminate(NULL);
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc){

	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	printf("overrun error\n");

}

#endif /* SAMPLING_H_ */
