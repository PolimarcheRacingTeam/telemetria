/*
 * adcSamplig.h
 *
 *  Created on: 
 *      Author: Mich
 */
 
 /*
 ADC1 INIT SETTINGS:
 
- Enable 8 channels  IN0, IN3, IN4, IN5, IN6, IN9, IN10
- Configuration:   

		  hadc1.Instance = ADC1;
		  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
		  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
		  hadc1.Init.ScanConvMode = ENABLE;							<---------- CHECK THIS
		  hadc1.Init.ContinuousConvMode = DISABLE;
		  hadc1.Init.DiscontinuousConvMode = DISABLE;
		  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		  hadc1.Init.NbrOfConversion = 2;
		  hadc1.Init.DMAContinuousRequests = DISABLE;
		  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
		  if (HAL_ADC_Init(&hadc1) != HAL_OK)
		  {
			_Error_Handler(__FILE__, __LINE__);
		  }
 

 
  */
 

#ifndef ADCSAMPLING_H_
#define ADCSAMPLING_H_

#include "adc.h"

#define PERIOD 1000 //milliseconds 


static void samplingThread(void const *arg){

    uint8_t adcValue1=0; 
	uint8_t adcValue2=0; 
	uint8_t adcValue3=0; 
	uint8_t adcValue4=0; 
	uint8_t adcValue5=0; 
	uint8_t adcValue6=0; 
	uint8_t adcValue7=0; 
	uint8_t adcValue8=0; 
	

    while(1){
    		
			//Starting ADC1
			HAL_ADC_Start(&hadc1);	
			
			
			// Polling CHANNEL 01
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); 
			adcValue1 = HAL_ADC_GetValue(&hadc1);
			
			// Polling CHANNEL 02
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); 
			adcValue2 = HAL_ADC_GetValue(&hadc1);
		
			// Polling CHANNEL 03
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); 
			adcValue3 = HAL_ADC_GetValue(&hadc1);
			
			// Polling CHANNEL 04
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); 
			adcValue4 = HAL_ADC_GetValue(&hadc1);
			
			// Polling CHANNEL 05
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); 
			adcValue5 = HAL_ADC_GetValue(&hadc1);
			
			// Polling CHANNEL 06
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); 
			adcValue6 = HAL_ADC_GetValue(&hadc1);
			
			// Polling CHANNEL 07
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); 
			adcValue7 = HAL_ADC_GetValue(&hadc1);
			
			// Polling CHANNEL 08
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); 
			adcValue8 = HAL_ADC_GetValue(&hadc1);
		
		
			//Stopping ADC1
			HAL_ADC_Stop(&hadc1);	
			
			
		     
            osDelay(PERIOD); // some delay
    }

}

#endif /* ADCSAMPLING_H_ */