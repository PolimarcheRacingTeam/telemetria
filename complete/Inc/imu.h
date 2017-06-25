/*
 * imu.h
 *
 *  Created on: Jun 10, 2017
 *      Author: Seb
 */

#ifndef IMU_H_
#define IMU_H_

#include "inv_mpu.h"
#include "math.h"

#define DEFAULT_MPU_HZ  (20)
#define COMPASS_READ_MS (100)

#define ACCEL_ON        (0x01)
#define GYRO_ON         (0x02)
#define COMPASS_ON      (0x04)

struct rx_s {
	unsigned char header[3];
	unsigned char cmd;
};

struct hal_s {
	unsigned char lp_accel_mode;
	unsigned char sensors;
	unsigned char dmp_on;
	unsigned char wait_for_tap;
	volatile unsigned char new_gyro;
	unsigned char motion_int_mode;
	unsigned long no_dmp_hz;
	unsigned long next_pedo_ms;
	unsigned long next_temp_ms;
	unsigned long next_compass_ms;
	unsigned int report;
	unsigned short dmp_features;
	struct rx_s rx;
};
static struct hal_s hal = {0};


static inline void run_self_test(void)
{
	int result;
	long gyro[3], accel[3];

	result = mpu_run_6500_self_test(gyro, accel, 0);
	if (result == 0x7) {
		printf("Passed!\n");
		printf("accel: %7.4f %7.4f %7.4f\n",
				accel[0]/65536.f,
				accel[1]/65536.f,
				accel[2]/65536.f);
		printf("gyro: %7.4f %7.4f %7.4f\n",
				gyro[0]/65536.f,
				gyro[1]/65536.f,
				gyro[2]/65536.f);
		/* Test passed. We can trust the gyro data here, so now we need to update calibrated data*/

		/*
		 * This portion of the code uses the HW offset registers that are in the MPUxxxx devices
		 * instead of pushing the cal data to the MPL software library
		 */
		unsigned char i = 0;

		for(i = 0; i<3; i++) {
			gyro[i] = (long)(gyro[i] * 32.8f); //convert to +-1000dps
			accel[i] *= 2048.f; //convert to +-16G
			accel[i] = accel[i] >> 16;
			gyro[i] = (long)(gyro[i] >> 16);
		}
		mpu_set_gyro_bias_reg(gyro);
		mpu_set_accel_bias_6500_reg(accel);
	}
	else {
		if (!(result & 0x1))
			printf("Gyro failed.\n");
		if (!(result & 0x2))
			printf("Accel failed.\n");
		if (!(result & 0x4))
			printf("Compass failed.\n");
	}

}

void q2rpy (const double q[4], double* roll, double* pitch, double* yaw)
{
	double ysqr = q[1] * q[1];

	// roll (x-axis rotation)
	double t0 = +2.0 * (q[3] * q[0] + q[1] * q[2]);
	double t1 = +1.0 - 2.0 * (q[0] * q[0] + ysqr);
	*roll = atan2(t0, t1);

	// pitch (y-axis rotation)
	double t2 = +2.0 * (q[3] * q[1] - q[2] * q[0]);
	t2 = t2 > 1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;
	*pitch = asin(t2);

	// yaw (z-axis rotation)
	double t3 = +2.0 * (q[3] * q[2] + q[0] * q[1]);
	double t4 = +1.0 - 2.0 * (ysqr + q[2] * q[2]);
	*yaw = atan2(t3, t4);
}


void imuInit(){
	int res = 0;
	unsigned char accel_fsr,  new_temp = 0;
	unsigned short gyro_rate, gyro_fsr;
	unsigned long timestamp;
	unsigned char new_compass = 0;
	unsigned short compass_fsr;


	res = mpu_init();
	if(res) {
		printf("reset failed...\n");
		while(1);
	}

	mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
	mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
	mpu_set_sample_rate(DEFAULT_MPU_HZ);
	mpu_set_compass_sample_rate(1000 / COMPASS_READ_MS);

	mpu_get_sample_rate(&gyro_rate);
	mpu_get_gyro_fsr(&gyro_fsr);
	mpu_get_accel_fsr(&accel_fsr);
	mpu_get_compass_fsr(&compass_fsr);

	hal.sensors = ACCEL_ON | GYRO_ON | COMPASS_ON;
	hal.dmp_on = 0;
	hal.report = 0;
	hal.rx.cmd = 0;
	hal.next_pedo_ms = 0;
	hal.next_compass_ms = 0;
	hal.next_temp_ms = 0;

	dmp_load_motion_driver_firmware();
	hal.dmp_features = DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
			DMP_FEATURE_GYRO_CAL;
	dmp_enable_feature(hal.dmp_features);
	dmp_set_fifo_rate(DEFAULT_MPU_HZ);
	mpu_set_dmp_state(1);
	hal.dmp_on = 1;

	short gyro[3], accel_short[3], sensors;
	unsigned char more;
	long accel[3], quat[4], temperature;
	unsigned long sensor_timestamp;
	double q[4];
	double roll, pitch, yaw;
	long lr, lp, ly;


	while(1){
		dmp_read_fifo(gyro, accel_short, quat, &sensor_timestamp, &sensors, &more);
		if(more) continue;
        mpu_get_temperature(&temperature, &sensor_timestamp);
		accel[0] = (long)accel_short[0];
		accel[1] = (long)accel_short[1];
		accel[2] = (long)accel_short[2];
		for (int i = 0; i < 4; i++){
			q[i] = quat[i];
		}
		q2rpy(q, &roll, &pitch, &yaw);
		lr = roll*1000.0/M_PI*180.0;
		lp = pitch*1000.0/M_PI*180.0;
		ly = yaw*1000.0/M_PI*180.0;
		printf("roll = %ld\tpitch = %ld\tyaw = %ld\t", lr, lp, ly);
		printf("ax = %ld\tay = %ld\taz = %ld\n", accel[0], accel[1], accel[2]);
		HAL_Delay(100);
	}

}




#endif /* IMU_H_ */
