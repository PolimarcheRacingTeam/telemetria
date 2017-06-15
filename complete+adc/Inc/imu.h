/*
 * imu.h
 *
 *  Created on: Jun 10, 2017
 *      Author: Seb
 */

#ifndef IMU_H_
#define IMU_H_

void imuInit(){
	int res = 0;
	res = mpu_init();
	if(res) printf("not yet...\n");
	else printf("woohoo");

}

#endif /* IMU_H_ */
