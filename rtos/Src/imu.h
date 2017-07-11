/*
 * imu.h
 *
 *  Created on: Jun 10, 2017
 *      Author: Seb
 */

#ifndef IMU_H_
#define IMU_H_
#include "i2c.h"
#include "inv_mpu.h"

int I2C_WriteRegister(  unsigned char slave_addr,
						unsigned char reg_addr,
						unsigned short len,
						unsigned char *data_ptr)
{
	return HAL_I2C_Mem_Write(&hi2c4, slave_addr<<1, reg_addr, I2C_MEMADD_SIZE_8BIT,
		  data_ptr, len, HAL_MAX_DELAY);
}

int I2C_ReadRegister(  unsigned char slave_addr,
					   unsigned char reg_addr,
					   unsigned short len,
					   unsigned char *data_ptr)
{

  return HAL_I2C_Mem_Read(&hi2c4, slave_addr<<1, reg_addr, I2C_MEMADD_SIZE_8BIT,
		  data_ptr, len, HAL_MAX_DELAY);

}

void get_ms(unsigned long *timestamp){
	*timestamp = HAL_GetTick();
}

void imuInit(){
	int res = 0;
	res = mpu_init();
	if(res) printf("IMU not available\n");
	else {
		if(mpu_set_sensors(INV_XYZ_ACCEL))
			printf("accelerometer failed\n");
		else
			printf("accelerometer ready\n");
	}

}



#endif /* IMU_H_ */
