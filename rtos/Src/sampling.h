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
uint32_t now, last;

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
		if (now - last > 10) {
			printf("late\n");
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
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

		while(HAL_CAN_Receive(&hcan1,CAN_FIFO0, 0) != HAL_TIMEOUT){
			switch (hcan1.pRxMsg->StdId){
			case 0x002: format(ecumsg->id2, hcan1.pRxMsg->Data, 8);
			break;
			case 0x003: format(ecumsg->id3, hcan1.pRxMsg->Data, 8);
			break;
			case 0x004: format(ecumsg->id4, hcan1.pRxMsg->Data, 8);
			break;
			case 0x005: format(ecumsg->id5, hcan1.pRxMsg->Data, 8);
			break;
			}
		}

		if(mpu_get_accel_reg(imumsg->a, NULL)){
			imumsg->ax = 0;
			imumsg->ay = 0;
			imumsg->az = 0;
		}
		osMessagePut(imuMsgBox, (uint32_t)imumsg, osWaitForever);
		osMessagePut(ecuMsgBox, (uint32_t)ecumsg, osWaitForever);
		osMessagePut(adcMsgBox, (uint32_t)adcmsg, osWaitForever);

		if(teleFlag){
			teleFlag = 0;

			telemsg = osPoolAlloc(telePool);

			telemsg->rpm = ecumsg->rpm;
			telemsg->map = ecumsg->map;
			telemsg->lambda = ecumsg->lambda;
			telemsg->tps = ecumsg->tps;
			telemsg->engtemp = ecumsg->engtemp;

			telemsg->vbat = ecumsg->vbat;
			telemsg->oilp = ecumsg->oilp;
			telemsg->oilt = ecumsg->oilt;
			telemsg->gear = ecumsg->gear;
			telemsg->bse = ecumsg->bse;

			telemsg->rearbrake = adcmsg->rearbrake;
			telemsg->fr = adcmsg->fr;
			telemsg->fl = adcmsg->fl;
			telemsg->rr = adcmsg->rr;
			telemsg->rl = adcmsg->rl;

			telemsg->speed = ecumsg->speed;
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

			dashmsg->rpm = ecumsg->rpm;
			dashmsg->gear = ecumsg->gear;
			dashmsg->lambda = ecumsg->lambda;
			dashmsg->speed = ecumsg->speed;
			dashmsg->engtemp = ecumsg->engtemp;
			dashmsg->vbat = ecumsg->vbat;
			dashmsg->oilp = ecumsg->oilp;
			dashmsg->oilt = ecumsg->oilt;

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
