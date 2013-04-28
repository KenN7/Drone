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

//------------------------- initAccel() -------------------
// Inicializa o Aceletormetro, retorna um erro caso não consiga
unsigned char initAccel(void)
{
        unsigned char error = 0;
        error += LDByteWriteI2C(i2c_ADXL345,ADXL345_DATA_FORMAT,0x0A);
        error += LDByteWriteI2C(i2c_ADXL345,ADXL345_POWER_CTL,0x08);
        error += LDByteWriteI2C(i2c_ADXL345,ADXL345_BW_RATE,0x0A);
        return error;
}

//------------------------- readAccel() -------------------
// Le os valores em uma array de 3inteiros, accel_X, accel_Y,accel_Z.
// Retorna um erro caso não consiga
unsigned char readAccel(int *data)
{
        unsigned char error;
    error = LDByteReadI2C(i2c_ADXL345,ADXL345_OUTPUTS,(unsigned char *)data,6);
        return error;
}

//------------------------- initGyro() -------------------
// Inicializa o gyro, retorna um erro caso não consiga
unsigned char initGyro(void)
{
        unsigned char error = 0;
        error += LDByteWriteI2C(i2c_ITG3200,ITG3200_DLPF_FS,0x1b);
        error += LDByteWriteI2C(i2c_ITG3200,ITG3200_SMPLRT_DIV,0x09);
        return error;
}

//------------------------- readGyro() -------------------
// Le os valores em uma array de 4 inteiros, temp, gyro_X, gyro_Y,gyro_Z.
// Retorna um erro caso não consiga
unsigned char readGyro(int *data)
{
        unsigned char error, buffer[8];

    error = LDByteReadI2C(i2c_ITG3200,ITG3200_OUTPUTS,buffer,8);

        *data++ = (buffer[0]<<8)|buffer[1];
        *data++ = (buffer[2]<<8)|buffer[3];
        *data++ = (buffer[4]<<8)|buffer[5];
        *data++ = (buffer[6]<<8)|buffer[7];

        return error;
}