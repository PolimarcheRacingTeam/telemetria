/*
 * telemetry.h
 *
 *  Created on: Jun 6, 2017
 *      Author: Seb
 */

#ifndef ADCSAMPLING_H_
#define ADCSAMPLING_H_

#include "adc.h"

#define PERIOD 1000 //milliseconds 


static void samplingThread(void const *arg){

    uint8_t adcValue=0; 

    while(1){
    		
			HAL_ADC_Start(&hadc1);	
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); 
			
			adcValue = HAL_ADC_GetValue(&hadc1);
			HAL_UART_Transmit(&huart3, &adcValue, sizeof(adcValue), HAL_MAX_DELAY);
		
			HAL_ADC_Stop(&hadc1);	
		     
            osDelay(PERIOD); 
    }

}

#endif /* ADCSAMPLING_H_ */