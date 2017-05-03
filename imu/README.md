 comunicazione i2c con imu

 il progetto i2c-console contiene codice elementare per comunicare con la IMU:

 nei file i2c.h / i2c.c ci sono delle funzioni per configurare e accedere ad
 alcuni registri

 il main.c configura la periferica i2c, il sensore, la uart e attende che venga
 premuto il pulsante blu per ottenere e stampare i valori delle accelerazioni

bisogna implementare le funzioni che incapsulino tutto quello di cui avremo
bisogno

c'è da fare del benchmarking, impostare il BUS in fast-mode e forse
valutare l'uso di DMA
