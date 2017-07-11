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

typedef union{
	struct{
		int16_t rpm, map, air, lambda,
		tps, engtemp, vbat, oilp,
		oilt, gear, fuel, speed,
		bse, tps2, tpd1, tpd2;
	};
	struct {
		uint8_t p2[4];
		uint8_t p3[4];
		uint8_t p4[4];
		uint8_t p5[4];
	};
	struct {
		uint8_t id2[8];
		uint8_t id3[8];
		uint8_t id4[8];
		uint8_t id5[8];
	};
} ECUData;

typedef struct{
	union{
		uint16_t A[6];
		struct {
			uint16_t A0,fr,fl,rr,rl,rearbrake;
		};
	};
	int32_t millis;
} ADCData;

typedef union {
	struct {
		int16_t a[3];
		int16_t w[3];
	};
	struct{
		int16_t ax,ay,az,gx,gy,gz;
	};
} IMUData;

typedef struct {
	int16_t rpm, 		map, 	lambda,	tps, 	engtemp,
			vbat, 		oilp, 	oilt, 	gear, 	bse,
			rearbrake, 	fr,		fl,		rr,		rl,
			speed,		steer, 	a5,		a6,		a7,
			az,			ax,		ay;
} teleData;

typedef struct  {
	uint8_t header;
	uint8_t rpm, gear, speed, engtemp, oilp, vbat, lambda, oilt;
} dashData;

osPoolDef(adcPool, 8, ADCData);                    // Define memory pool
osPoolId  adcPool;
osMessageQDef(adcMsgBox, 8, ADCData*); //Define message queue
osMessageQId adcMsgBox;

osPoolDef(imuPool, 8, IMUData);                    // Define memory pool
osPoolId  imuPool;
osMessageQDef(imuMsgBox, 8, IMUData*); //Define message queue
osMessageQId imuMsgBox;

osPoolDef(ecuPool, 8, ECUData);                    // Define memory pool
osPoolId  ecuPool;
osMessageQDef(ecuMsgBox, 8, ECUData*); //Define message queue
osMessageQId ecuMsgBox;

osPoolDef(telePool, 8, teleData);                    // Define memory pool
osPoolId  telePool;
osMessageQDef(teleMsgBox, 8, teleData*); //Define message queue
osMessageQId teleMsgBox;

osPoolDef(dashPool, 8, dashData);                    // Define memory pool
osPoolId  dashPool;
osMessageQDef(dashMsgBox, 8, dashData*); //Define message queue
osMessageQId dashMsgBox;

uint8_t teleFlag = 0;
uint8_t dashFlag = 0;
uint8_t IPready = 0;
uint8_t loggingReady = 0;


void setupBuffers(){
	adcPool = osPoolCreate(osPool(adcPool));
	imuPool = osPoolCreate(osPool(imuPool));
	ecuPool = osPoolCreate(osPool(ecuPool));
	telePool = osPoolCreate(osPool(telePool));
	dashPool = osPoolCreate(osPool(dashPool));
	adcMsgBox = osMessageCreate(osMessageQ(adcMsgBox), NULL);
	imuMsgBox = osMessageCreate(osMessageQ(imuMsgBox), NULL);
	ecuMsgBox = osMessageCreate(osMessageQ(ecuMsgBox), NULL);
	teleMsgBox = osMessageCreate(osMessageQ(teleMsgBox), NULL);
	dashMsgBox = osMessageCreate(osMessageQ(dashMsgBox), NULL);
	printf("buffers ready\n");
}

// copia len byte da from a to, scambiandoli a due a due
// (per cambiare da big endian della motec a little endian della nostra RAM)
void format(uint8_t* to, uint8_t* from, int len){
	for ( int i = 0; i < len - 1; i += 2){
		to[i] = from[i+1];
		to[i+1] = from[i];
	}
}

#endif /* DATABUFFER_H_ */
