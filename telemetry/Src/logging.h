/*
 * logging.c
 *
 *  Created on: Jun 6, 2017
 *      Author: Seb
 */

#ifndef __logging_H
#define __logging_H

#include "string.h"
#include "fatfs.h"
#include "sampling.h"
#include "databuffer.h"


FATFS SDFatFs;
FIL file;
char filename[] = "p____.csv";
uint16_t filecnt = 	0000;
FRESULT res;
char line[128];
osEvent  evt;

void attemptRecovery();
void writeSD(char line[]);

void loggingThread(const void* args){
	MX_FATFS_Init();

	while(!IPready) osDelay(100);

	do{
		res = f_mount(&SDFatFs, (TCHAR const*)SD_Path, 1);

		if(!res) break;
		else{
			osDelay(100);
			printf("res = %d ",res);
			printf("path = %c%c%c%c\n", SD_Path[0], SD_Path[1],
					SD_Path[2], SD_Path[3]);
		}
	} while(1);

	printf("SD ready\n");

	/* Choose adequate file name */
	do {
		sprintf(filename, "p%03d.csv",filecnt);
		res = f_stat(filename, NULL);

		if (filecnt >= 9999)
			break;
		if (res == FR_NO_FILE)
			break;
		if (res){ // != FR_OK && != FR_NO_FILE
			//printf("filename loop #%d err = %d\n",filecnt, res);
			osDelay(100);
		}
		else
			filecnt++;
	} while (1);
	if (filecnt >= 9999) {
		printf("too many files (9999) in SD card\n");
		while(1);
	}
	if (res == FR_NOT_READY) {
		printf("error: check SD card connections\n");
		Error_Handler();
	}
	if (res != FR_NO_FILE) {
		printf("failed getting file information, %d\n", res);
		Error_Handler();
	}

	/* Open new file */
	while(1) {
		res = f_open(&file, filename, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
		if(!res) {
			printf("filename: %s\n", filename);
			break;
		}
		//printf("failed to create new file, %d\n", res);
		f_close(&file);
		osDelay(100);

		//Error_Handler();
	}
	f_sync(&file);
	loggingReady = 1;

	ADCData* adcmsg;
	IMUData* imumsg;
	ECUData* ecumsg;

	while(1){

		evt = osMessageGet(adcMsgBox, osWaitForever);  // wait for message
		if (evt.status == osEventMessage) {
			adcmsg = evt.value.p;
			sprintf(line, "%ld;%d;%d;%d;%d;%d;%d;",
					adcmsg->millis,
					adcmsg->A[0], adcmsg->A[1], adcmsg->A[2],
					adcmsg->A[3], adcmsg->A[4], adcmsg->A[5]);
			osPoolFree(adcPool, adcmsg);
		}
		writeSD(line);
		//printf("%s\n", line); //per controllare gli adc su console

		evt = osMessageGet(imuMsgBox, osWaitForever);  // wait for message
		if (evt.status == osEventMessage) {
			imumsg = evt.value.p;
			sprintf(line, "%d;%d;%d;",
					imumsg->a[0], imumsg->a[1], imumsg->a[2]);
			osPoolFree(imuPool, imumsg);
		}
		writeSD(line);

		evt = osMessageGet(ecuMsgBox, osWaitForever);  // wait for message
		if (evt.status == osEventMessage) {
			ecumsg = evt.value.p;
			sprintf(line, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;\n",
					ecumsg->p2[0],ecumsg->p2[1],ecumsg->p2[2],ecumsg->p2[3],
					ecumsg->p3[0],ecumsg->p3[1],ecumsg->p3[2],ecumsg->p3[3],
					ecumsg->p4[0],ecumsg->p4[1],ecumsg->p4[2],ecumsg->p4[3],
					ecumsg->p5[0],ecumsg->p5[1],ecumsg->p5[2],ecumsg->p5[3]);
			printf("%d\n",ecumsg->tps);

			osPoolFree(ecuPool, ecumsg);
		}
		writeSD(line);

	}


	osThreadTerminate(NULL);

}

void attemptRecovery(){
	res = f_close(&file);
	if (res) printf("failed closing, ");
	res = f_open(&file, filename, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
	if (res) {
		printf("failed opening, resetting driver: ");
		res = FATFS_UnLinkDriver(SD_Path);
		if (res) {
			printf("unlink failed\n");
			//don't care
		}
		MX_FATFS_Init();
		if (retSD) {
			printf("relink failed, ");
			return;
		}
		res = f_mount(&SDFatFs, (TCHAR const*)SD_Path, 1);
		if (res) {
			printf("failed to mount: %d, ", res);
			return;
		}
		res = f_open(&file, filename, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
		if (res) {
			printf("failed reopening: %d, ", res);
		}

	}

	res = f_lseek(&file, f_size(&file));
	if (res) {
		//printf("failed to reset file pointer, ");
		return;
	} else {
		printf("fixd\n");
	}

}

UINT writecnt = 0;
int flag;

void writeSD(char line[]){
	flag = 0;
	while(1) {
		res = f_write(&file, (void*)line, strlen(line), &writecnt);

		if (res){
			printf("SDerr.."); 	// se non stampi almeno 6 char si impalla
								// lo so che è brutto, se non ci dormi fallo te
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin,1);
			taskENTER_CRITICAL();
			attemptRecovery();
			taskEXIT_CRITICAL();
		} else {
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin,0);
			break;
		}
		/*
			//printf("write error, ");
			if(flag > 5){
				taskENTER_CRITICAL();
				attemptRecovery();
				taskEXIT_CRITICAL();
			}else{
				printf(".");
				flag ++;
			}
		} else {
			if (flag) {
				printf("\n");
			}
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin,0);
			break;
		}*/
	}
}

#endif
