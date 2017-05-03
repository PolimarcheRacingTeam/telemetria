 Acquisizione tramite ADC 
 
 
 sospensioni:
 - Potenziometro FL wheel
 - Potenziometro FR wheel
 - Potenziometro RF wheel
 - Potenziometro RR wheel
 
 sterzo:
 - Potenziometro sterzo
 
 freni:
 - sensore pressione circuito freni
 
 batteria:
- sensore corrente batteria
 
 temperatura:
 - sensore ntc temperatura aria dopo radiatore

 
 Totale 8 ingressi analogici
 
 
 -------------------------------------------------------------------------------------------
 Conversion speed:
 
  (minimum convesion time for resolutions)
  • 12 bits: 3 + 12 = 15 ADCCLK cycles
  • 10 bits: 3 + 10 = 13 ADCCLK cycles 
  • 8  bits: 3 + 8  = 11 ADCCLK cycles 
  • 6  bits: 3 + 6  = 9  ADCCLK cycles
  
  
  1/[(McuFrequency)/Prescaler]= Cycles per second (or μs if you use MHz)
  
  
  Number of selcted cycles (es.  sConfig.SamplingTime = ADC_SAMPLETIME_XXXCYCLES; ),
  divided for cycles per second gives sampling time:
  
  
  1/[(selected cicles)/(cycles per seconds)]= Samplig Frequency
  
 --------------------------------------------------------------------------------------------- 
  
  