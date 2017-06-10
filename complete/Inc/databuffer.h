/*
 * databuffer.h
 *
 *  Created on: Jun 6, 2017
 *      Author: Seb
 */

#ifndef DATABUFFER_H_
#define DATABUFFER_H_

/* Riferirsi a :
https://www.keil.com/pack/doc/CMSIS/RTOS/html/group__CMSIS__RTOS__Message.html
 */

#include "cmsis_os.h"

typedef struct{
  int16_t rpm, map, air, lambda, tps, engtemp, vbat, oilp, oilt, gear, fuel,
  speed, bse, tps2, tpd1, tpd2;
} ECUData;

typedef struct {
  uint32_t t;
  uint16_t a5,a6,a7,a9,a10,a11,a12,a13,a14;
} DAQData;

typedef struct {
	int16_t ax,ay,az,gx,gy,gz;
} IMUData;


osPoolDef(ecuPool, 16, ECUData);                    // Define memory pool
osPoolId ecuPool;

osMessageQDef(ECUMsgBox, 16, ECUData*); //Define message queue
osMessageQId ECUMsgBox;

osPoolDef(daqPool, 16, DAQData);                    // Define memory pool
osPoolId  daqPool;
osMessageQDef(DAQMsgBox, 16, DAQData*); //Define message queue
osMessageQId DAQMsgBox;

osPoolDef(imuPool, 16, IMUData);                    // Define memory pool
osPoolId  imuPool;
osMessageQDef(IMUMsgBox, 16, IMUData*); //Define message queue
osMessageQId IMUMsgBox;

void setupBuffers(){
	ecuPool = osPoolCreate(osPool(ecuPool));
	daqPool = osPoolCreate(osPool(daqPool));
	imuPool = osPoolCreate(osPool(imuPool));
	ECUMsgBox = osMessageCreate(osMessageQ(ECUMsgBox), NULL);
	DAQMsgBox = osMessageCreate(osMessageQ(DAQMsgBox), NULL);
	IMUMsgBox = osMessageCreate(osMessageQ(IMUMsgBox), NULL);
}

void fakeDataThread(void const *argument) {

	DAQData d = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	ECUData e = {0,1,2,3,4,5,6,7,8,9};
	IMUData i = {10,11,12,13,14,15};

	while(1){

		//osMessagePut(ECUData, e, 0);
		osDelay(10);
	}

}

void dispDataThread(void const *argument) {



}



#endif /* DATABUFFER_H_ */
