/* 
 * File:   accelgyro.h
 * Author: Ken
 *
 * Created on 27 avril 2013, 21:33
 */

//---------------- Constantes ------------------
// ADXL345  Registers
#define ADXL345_DEVID                   0x00    // R     Device ID
#define ADXL345_THRESH_TAP              0x1D    // R/W   Tap threshold
#define ADXL345_OFSX                    0x1E    // R/W   X-axis offset
#define ADXL345_OFSY                    0x1F    // R/W   Y-axis offset
#define ADXL345_OFSZ                    0x20    // R/W   Z-axis offset
#define ADXL345_DUR                     0x21    // R/W   Tap duration
#define ADXL345_Latent                  0x22    // R/W   Tap latency
#define ADXL345_Window                  0x23    // R/W   Tap window
#define ADXL345_THRESH_ACT              0x24    // R/W   Activity threshold
#define ADXL345_THRESH_INACT            0x25    // R/W   Inactivity threshold
#define ADXL345_TIME_INACT              0x26    // R/W   Inactivity time
#define ADXL345_ACT_INACT_CTL           0x27    // R/W   Axis enable control for activity and inactivity detection
#define ADXL345_THRESH_FF               0x28    // R/W   Free-fall threshold
#define ADXL345_TIME_FF                 0x29    // R/W   Free-fall time
#define ADXL345_TAP_AXES                0x2A    // R/W   Axis control for single tap/double tap
#define ADXL345_ACT_TAP_STATUS          0x2B    // R     Source of single tap/double tap
#define ADXL345_BW_RATE                 0x2C    // R/W   Data Rate and power mode control
#define ADXL345_POWER_CTL               0x2D    // R/W   Power-saving features control
#define ADXL345_INT_ENABLE              0x2E    // R/W   Interrupt enable control
#define ADXL345_INT_MAP                 0x2F    // R/W   Interrupt mapping control
#define ADXL345_INT_SOURCE              0x30    // R     Source of interrupts
#define ADXL345_DATA_FORMAT             0x31    // R/W   Data format control
#define ADXL345_DATAX0                  0x32    // R     X-Axis Data 0
#define ADXL345_DATAX1                  0x33    // R     X-Axis Data 1
#define ADXL345_DATAY0                  0x34    // R     Y-Axis Data 0
#define ADXL345_DATAY1                  0x35    // R     Y-Axis Data 1
#define ADXL345_DATAZ0                  0x36    // R     Z-Axis Data 0
#define ADXL345_DATAZ1                  0x37    // R     Z-Axis Data 1
#define ADXL345_FIFO_CTL                0x38    // R/W   FIFO control
#define ADXL345_FIFO_STATUS             0x39    // R     FIFO status
#define ADXL345_OUTPUTS                 0x32

// ITG-3200  Registers
#define ITG3200_WHO_AM_I                0x00
#define ITG3200_SMPLRT_DIV              0x15
#define ITG3200_DLPF_FS                 0x16
#define ITG3200_INT_CFG                 0x22
#define ITG3200_INT_STATUS              0x22
#define ITG3200_OUTPUTS                 0x1B
#define ITG3200_TEMP_OUT_H              0x1B
#define ITG3200_TEMP_OUT_L              0x1C
#define ITG3200_GYRO_XOUT_H             0x1D
#define ITG3200_GYRO_XOUT_L             0x1E
#define ITG3200_GYRO_YOUT_H             0x1F
#define ITG3200_GYRO_YOUT_L             0x20
#define ITG3200_GYRO_ZOUT_H             0x21
#define ITG3200_GYRO_ZOUT_L             0x22
#define ITG3200_PWR_MGM                 0x3E

#define i2c_ADXL345                     0xA6 //accelerometer
#define i2c_ITG3200                     0xD0 //gyroscope
