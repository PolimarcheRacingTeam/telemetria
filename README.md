Sheda di acquisizione: STM32 Nucleo-144 Board F746ZG 1M Flash
- Microcontrollore core Cortex-M7 da 216 MHz STM32F746ZGT6 con memoria flash da 1 MB, SRAM da 320 KB
- Acceleratore adattivo in tempo reale (ART Accelerator™) che consente l'esecuzione con stato di attesa 0 dalla memoria Flash
- Controller LCD-TFT fino a risoluzione XGA e Chrom-ART Accelerator™ dedicato
- Interfaccia macchina fotografica in parallelo da 8 a 14 bit fino a 54 MB/s
- Accesso completo a tutti i GPIO con connettore ST Zio (supporto connettività Arduino Uno v3)
- Connettori maschio di espansione ST morpho per l'accesso a tutti i GPIO
- Debugger/programmatore ST-LINK/V2-1 con connettore SWD
- Fino a 25 interfacce di comunicazione seriale: USART, IrDA, I²C, SPI, LIN, CAN, USB, I²S, SDIO, HDMI-CEC, S/PDIF-Rx, Ethernet
- Alimentatore scheda flessibile
- Dispositivo USB OTG o FS con connettore micro-AB
- Generatore di numeri casuali
- Unità di calcolo CRC
- RTC con precisione in frazioni di secondo e calendario hardware
- ID univoco a 96 bit
- 3 LED: LED di alimentazione, comunicazione USB, LED utente
- Pulsanti operatore e di reset
- Oscillatore al quarzo a 32,768 KHz
- Predisposizione ARM mbed (mbed.org)

Il modo più semplice di compilare e caricare un programma sulla scheda è di usare mbed. strumento poco professionale e limitato. è tipo il gradino sopra Arduino. (Gratis) Non dovrebbe essere troppo impegnativo quindi  vale la pena provarlo.

Possibile workflow serio per programmare la scheda ARM (tutto in C):

Si inizializza un progetto con STM32CubeMX, questo predispone le librerie per interfacciarsi all'hardware specificato (librerie "HAL").
POI
Il progetto può essere esportato per programmare/flashare tramite una scelta di IDE:
1. Keil uVision (MDK-ARM v5) - soluzione professionale di ARM stessa, lots of $$$
2. IAR Embedded Workbench (EWARM) - soluzione professionale lots of $$$
3. Atollic Truestudio - IDE gratuito in versione limitata, closed source, (la versione Mac deve arrivare..)
4. System Workbench for STM32 (SW4STM32) - IDE open source basato su Eclipse, compatibilità alle schede stm32F7 aggiunta da ST stessa (https://goo.gl/rKpHgf)
Altre opzioni:
https://goo.gl/fgkwXq
tra cui 
5. IDE (tipo Eclipse) + compiler ARM + driver ST-link , massima libertà ma ci perdi più tempo per configurare. 

Leggendo in giro tipo https://goo.gl/vybkfy o https://goo.gl/o3R6Ob sono abbastanza convinto che la 4 sia la migliore per noi. Sostanzialmente è la 5 già impacchetta e cross-platform, supporta i progetti inzializzati con Cube, è gratis, open source e anche ST ha contribuito alla sua esistenza. I migliori indiscussi sono IAR e KEIL ma costano, mentre di truestudio ne parla quasi nessuno.
