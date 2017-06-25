/*
 * databuffer.h
 *
 *  Created on: Jun 6, 2017
 *      Author: Seb
 */

#ifndef DATABUFFER_H_
#define DATABUFFER_H_

#include <string.h>
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

void fakeDataThread(void const *argument) ;
void dispDataThread(void const *argument) ;

void setupBuffers(){
	ecuPool = osPoolCreate(osPool(ecuPool));
	daqPool = osPoolCreate(osPool(daqPool));
	imuPool = osPoolCreate(osPool(imuPool));
	ECUMsgBox = osMessageCreate(osMessageQ(ECUMsgBox), NULL);
	DAQMsgBox = osMessageCreate(osMessageQ(DAQMsgBox), NULL);
	IMUMsgBox = osMessageCreate(osMessageQ(IMUMsgBox), NULL);
}



void fakeDataThread(void const *argument) {

	ECUData *e,ce = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	DAQData *d,cd = {0,1,2,3,4,5,6,7,8,9};
	IMUData *i,ci = {10,11,12,13,14,15};

	while(1){
		printf("putting\n\n");

		d = osPoolAlloc(daqPool);       // Allocate memory for the message
		memcpy(d, &cd, sizeof(DAQData));
		osMessagePut(DAQMsgBox, (uint32_t)d, osWaitForever);  // Send Message
/*
		e = osPoolAlloc(ecuPool);       // Allocate memory for the message
		memcpy(e, &ce, sizeof(ECUData));
		osMessagePut(ECUMsgBox, (uint32_t)d, osWaitForever);  // Send Message
		i = osPoolAlloc(imuPool);       // Allocate memory for the message
		memcpy(i, &ci, sizeof(IMUData));
		osMessagePut(IMUMsgBox, (uint32_t)d, osWaitForever);  // Send Message
*/
		osDelay(10);


	}

}

void dispDataThread(void const *argument) {

	DAQData *d;
	osEvent  evt;

	while(1) {
		osDelay(100);
		printf("getting\n\n");
		while(1){

			evt = osMessageGet(DAQMsgBox, 0);  // don't wait for message

			if (evt.status == osEventMessage) {
				d = evt.value.p;
				printf("got: %d\t%d\t%d\n", d->a10, d ->a11, d->a12);
				osPoolFree(daqPool, d);        // free memory allocated for message
			} else {
				break;
			}
		}
	}


}



#endif /* DATABUFFER_H_ */
