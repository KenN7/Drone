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
        error += LDByteWriteI2C(i2c_ADXL345,ADXL345_DATA_FORMAT,0x03); //(0x0B=full res +-16g) 0x03=not fullres 16g
        error += LDByteWriteI2C(i2c_ADXL345,ADXL345_POWER_CTL,0x08);
        error += LDByteWriteI2C(i2c_ADXL345,ADXL345_BW_RATE,0x08); //200Hz bw 100Hz



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

    /* This code shows an easy way to smooth readings from a sensor subject to
 high frequency noise.
It uses a low pass filter on a circular buffer.
This circular buffer always contains the last BUFFER_SIZE-1 readings from
the sensor.
The new reading is then added to this buffer, from which wecompute the
mean value by simply dividing the sum of the readings in the buffer by the
number of readings in the buffer.
*/

int indexBuffer = 0;
int BUFFER_SIZE = 5; // Number of samples you want to filter on.

float circularBufferX[5];
float circularBufferY[5];
float circularBufferZ[5];
float sensorDataCircularSum[3];
float filteredOutput[3];
extern volatile int raw_dataA[3]; // typically the value you read from your sensor

void smoothSensorReadings(){
 // We remove the oldest value from the buffer
 sensorDataCircularSum[0] = sensorDataCircularSum[0] - circularBufferX[indexBuffer];
 sensorDataCircularSum[1] = sensorDataCircularSum[1] - circularBufferY[indexBuffer];
 sensorDataCircularSum[2] = sensorDataCircularSum[2] - circularBufferZ[indexBuffer];
  // The new input from the sensor is placed in the buffer
 circularBufferX[indexBuffer] = raw_dataA[0];
 circularBufferY[indexBuffer] = raw_dataA[1];
 circularBufferZ[indexBuffer] = raw_dataA[2];
// It is also added to the total sum of the last  BUFFER_SIZE readings
// This method avoids to sum all the elements every time this function is called.
 sensorDataCircularSum[0] += raw_dataA[0];
 sensorDataCircularSum[1] += raw_dataA[1];
 sensorDataCircularSum[2] += raw_dataA[2];
// We increment the cursor
 indexBuffer++;

 if (indexBuffer>=BUFFER_SIZE) indexBuffer=0;// We test if we arrived to the end
//of the buffer, in which case we start again from index 0
 filteredOutput[0] =(sensorDataCircularSum[0]/BUFFER_SIZE); // The output is the the mean
 filteredOutput[1] =(sensorDataCircularSum[1]/BUFFER_SIZE);
 filteredOutput[2] =(sensorDataCircularSum[2]/BUFFER_SIZE);
//value of the circular buffer.
}


void Process_Accel(volatile int * raw_data,volatile float * data) //data[0] = X angle, data[1] = Y angle
{
    //http://www.analog.com/static/imported-files/application_notes/AN-1057.pdf
    //http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
    //raw_data[X,Y,Z] accel, currently in radians, multiply by 57.295 to get degrees
    
    data[0] = atan2(raw_data[1],raw_data[2])*57.295; //57.295; //Those 2 formulas can be found on the internet, dunno which to choose

    //data[0] = atan2(raw_data[1],sqrt(raw_data[0]*raw_data[0]+raw_data[2]*raw_data[2]));
    //data[0] is roll
    //data[1] = atan2(raw_data[0],sqrt(raw_data[1]*raw_data[1]+raw_data[2]*raw_data[2]));
    
    data[1] = atan2(raw_data[0],raw_data[2])*57.295; //57.295;
    
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
        error += LDByteWriteI2C(i2c_ITG3200,ITG3200_SMPLRT_DIV,0x01);
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
        __delay_ms(100);
        led1 = !led2;
        led3 = !led1;
        led2 = !led3;
    }

    GyroOffset[0] = GyroOffset[0]/50 ; //approx -55
    GyroOffset[1] = GyroOffset[1]/50 ; // approx 49
    GyroOffset[2] = GyroOffset[2]/50 ; //approx -52
}

void Process_Gyro(volatile int * raw_data, volatile float * data) //return the calculated gyro angles
// data[0] = Xangle, data[1] = Yangle, data[2] = Zrate.
{
    int gyro_xsens = 1; //must tweak those !!
    int gyro_ysens = -1; //14.375 according to datasheet // 14.375
    int gyro_zsens = 1;
    data[0] += (((float)raw_data[1]-GyroOffset[0])/gyro_xsens)*dt; //data[0] represents the roll angle
    data[1] += (((float)raw_data[2]-GyroOffset[1])/gyro_ysens)*dt; //some use the rate into the filter
    data[2] = (((float)raw_data[3]-GyroOffset[2])/gyro_zsens); //its a rate
}