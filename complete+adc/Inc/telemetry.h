/*
 * telemetry.h
 *
 *  Created on: Jun 6, 2017
 *      Author: Seb
 */

#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include "lwip/api.h"
#include "retarget.h"
#include "string.h"


#define TELEMETRY_PORT 2017
#define LEASE_LIFE 5000 //milliseconds
#define TELEMETRY_PERIOD 100 //milliseconds

long last_lease = 0;
static struct netconn *conn;
static struct netbuf *buf;

char * packet;
uint16_t packet_length;

static void telemetryThread(void const *arg){

    const char msg[]="testing\n" ;

    conn = netconn_new( NETCONN_UDP );
    netconn_bind(conn, IP_ADDR_ANY, TELEMETRY_PORT ); //local port

    netconn_connect(conn, IP_ADDR_BROADCAST, TELEMETRY_PORT );
    while(1){
    		printf("sending\n");
            buf = netbuf_new();
            packet = netbuf_alloc(buf, sizeof(msg));
            memcpy (packet, msg, sizeof (msg));
            netconn_send(conn, buf);
            netbuf_delete(buf); // De-allocate packet buffer
            osDelay(TELEMETRY_PERIOD); //some delay!
    }

}



/*

{
  err_t err, recv_err;
  conn = netconn_new(NETCONN_UDP);
  if (conn != NULL) {
    err = netconn_bind(conn, IP_ADDR_ANY, LISTEN_PORT);
    if (err == ERR_OK) {
    	while (1){
    		recv_err = netconn_recv(conn, &buf);

    		if (recv_err == ERR_OK) {
    			printf("received over UDP\n");
    			last_lease = HAL_GetTick();
    			client = netbuf_fromaddr(buf);
    			port = netbuf_fromport(buf);
    			netconn_connect(conn, client, port);
    			buf->addr.addr = 0;
    			while ( HAL_GetTick() - last_lease < LEASE_LIFE){
    				netbuf_data(buf, (void *)&packet, &packet_length);
    				printf("\n packet length = %d\n", packet_length);
    				for (int i = 0; i < packet_length; i++){
    					printf("%c", packet[i]);
    					packet[i] = 'a' + i;
    				}
    				printf("\n");
    				//costruisci buffer da inviare alla telemetria
    				netconn_send(conn,buf);
    				osDelay(TELEMETRY_PERIOD);
    			}
    			netbuf_delete(buf);

    		} else {
    			printf("no client\n");
    			osDelay(500);
    		}
    	}
    }
  }
  else{
	  netconn_delete(conn);
  }

}*/

#endif /* TELEMETRY_H_ */
