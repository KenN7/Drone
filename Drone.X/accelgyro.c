/* 
 * File:   accelgyro.c
 * Author: Ken
 *
 * Created on 27 avril 2013, 21:31
 */

#include <p33Fxxxx.h>      /* Includes device header file                     */
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include "header.h"        /* Function / Parameters                           */
#include <libpic30.h>
#include <stdio.h>
#include <stdlib.h>
#include "accelgyro.h"
#include "i2c_func.h"
#include <math.h>

//------------------------- initAccel() -------------------
// Initialize accelerometer
unsigned char Initialize_Accel(void)
{
        unsigned char error = 0;
        error += LDByteWriteI2C(i2c_ADXL345,ADXL345_DATA_FORMAT,0x0A);
        error += LDByteWriteI2C(i2c_ADXL345,ADXL345_POWER_CTL,0x08);
        error += LDByteWriteI2C(i2c_ADXL345,ADXL345_BW_RATE,0x0A);
        return error;
}

//------------------------- readAccel() -------------------
// Reads the values of the accelerometer in an array of 3 integer :
// accel_X, accel_Y,accel_Z.
unsigned char Read_Accel(volatile int * raw_data)
{
        unsigned char error;
    error = LDByteReadI2C(i2c_ADXL345,ADXL345_OUTPUTS,(unsigned char *)raw_data,6);
        return error;
}

void Process_Accel(volatile int * raw_data,volatile float * data) //data[0] = X angle, data[1] = Y angle
{
    //http://www.analog.com/static/imported-files/application_notes/AN-1057.pdf
    //http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
    //raw_data[X,Y,Z] accel, currently in radians, multiply by 57.295 to get degrees
    
    data[0] = atan2(raw_data[1],raw_data[2])*57.295; //Those 2 formulas can be found on the internet, dunno which to choose

    //data[0] = atan2(raw_data[1],sqrt(raw_data[0]*raw_data[0]+raw_data[2]*raw_data[2]));
    //data[0] is roll
    //data[1] = atan2(raw_data[0],sqrt(raw_data[1]*raw_data[1]+raw_data[2]*raw_data[2]));
    
    data[1] = atan2(raw_data[0],raw_data[2])*57.295;
    
    //accelAngleX = atan2(x ,sqrt(ySquared + zSquared));
    //accelAngleY = atan2(y ,sqrt(xSquared + zSquared));
    //data[1] is pitch
}

//------------------------- initGyro() -------------------
// Initialize gyroscope
unsigned char Initialize_Gyro(void)
{
        unsigned char error = 0;
        error += LDByteWriteI2C(i2c_ITG3200,ITG3200_DLPF_FS,0x1b);
        error += LDByteWriteI2C(i2c_ITG3200,ITG3200_SMPLRT_DIV,0x09);
        return error;
}

//------------------------- readGyro() -------------------
// Reads the values of the gyroscope in an array of 4 integer :
// temp, gyro_X, gyro_Y,gyro_Z. (but we don't care about temp)
unsigned char Read_Gyro(volatile int * raw_data)
{
        unsigned char error, buffer[8];

    error = LDByteReadI2C(i2c_ITG3200,ITG3200_OUTPUTS,buffer,8);

        *raw_data++ = (buffer[0]<<8)|buffer[1];
        *raw_data++ = (buffer[2]<<8)|buffer[3];
        *raw_data++ = (buffer[4]<<8)|buffer[5];
        *raw_data++ = (buffer[6]<<8)|buffer[7];

        return error;
}

static float GyroOffset[3];
void Calibrate_Gyro(volatile int * raw_data)
{
    int i;
    for (i=0;i<50;i++)
    {
        Read_Gyro(raw_data);
        GyroOffset[0] += raw_data[1];
        GyroOffset[1] += raw_data[2];
        GyroOffset[2] += raw_data[3];
        __delay_ms(70);
        led1 = !led2;
        led3 = !led1;
        led2 = !led3;
    }
    GyroOffset[0] = GyroOffset[0]/50; //approx -55
    GyroOffset[1] = GyroOffset[1]/50; // approx 49
    GyroOffset[2] = GyroOffset[2]/50; //approx -52
}

void Process_Gyro(volatile int * raw_data, volatile float * data) //return the calculated gyro angles
// data[0] = Xangle, data[1] = Yangle, data[2] = Zangle.
{
    int gyro_xsens = 4; //must tweak those !!
    int gyro_ysens = -4; //14.375 according to datasheet // 14.375
    int gyro_zsens = 4;
    data[0] += (((float)raw_data[1]-GyroOffset[0])/gyro_xsens)*dt; //data[0] represents the roll angle
    data[1] += (((float)raw_data[2]-GyroOffset[1])/gyro_ysens)*dt; //some use the rate into the filter
    data[2] += (((float)raw_data[3]-GyroOffset[2])/gyro_zsens)*dt;
}