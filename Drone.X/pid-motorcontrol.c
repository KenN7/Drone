/* 
 * File:   pid-motorcontrol.c
 * Author: Ken
 *
 * Created on 28 avril 2013, 17:34
 */

#include <stdio.h>
#include <stdlib.h>
#include <p33Fxxxx.h>      /* Includes device header file                     */
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include "header.h"        /* Function / Parameters                           */
#include <libpic30.h>

extern volatile float throttle_input;
extern volatile float roll_input;
extern volatile float pitch_input;
extern volatile float yaw_input;
/*
 * 
 */
void PID()
{
//    PREVIOUS_XERROR = XERROR;
//    PREVIOUS_YERROR = YERROR;
//    PREVIOUS_ZERROR = ZERROR;
//
//    XERROR = TARGET_XANGLE - COMPLEMENTARY_XANGLE;
//    YERROR = TARGET_YANGLE - COMPLEMENTARY_YANGLE;
//    ZERROR = TARGET_ZRATE - GYRO_ZRATE;
//
//    //XDIFFERENTIAL = (XERROR - PREVIOUS_XERROR)/dt;
//    //YDIFFERENTIAL = (YERROR - PREVIOUS_YERROR)/dt;
//    //ZDIFFERENTIAL = (ZERROR - PREVIOUS_ZERROR)/dt;
//
//    XDIFFERENTIAL = -GYRO_XRATE;
//    YDIFFERENTIAL = -GYRO_YRATE;
//    ZDIFFERENTIAL = -GYRO_ZRATE;
//
//    XINTEGRAL += XERROR*dt;
//    YINTEGRAL += YERROR*dt;
//
//    if(XINTEGRAL > 0.5) {XINTEGRAL = 0.5;}
//    else if(XINTEGRAL < -0.5) {XINTEGRAL = -0.5;}
//    if(YINTEGRAL > 0.5) {YINTEGRAL = 0.5;}
//    else if(YINTEGRAL < -0.5) {YINTEGRAL = -0.5;}
//
//    PID_XOUTPUT = XERROR*KP + XDIFFERENTIAL*KD + XINTEGRAL*KI;
//    PID_YOUTPUT = YERROR*KP + YDIFFERENTIAL*KD + YINTEGRAL*KI;
//    PID_ZOUTPUT = ZERROR*ZKP; + ZDIFFERENTIAL*ZKD;
//    if(PID_ZOUTPUT > 1000){PID_ZOUTPUT = 1000;}
//    else if (PID_ZOUTPUT < -1000){PID_ZOUTPUT = -1000;}
}

void Update_PWM()
{
//	if(throttle_input==0.0)
//	{
//		OC1RS = 700;
//		OC2RS = 700;
//		OC3RS = 700;
//		OC4RS = 700;
//	}
//	else
//	{
//		OC1RS = 0.5*PID_XOUTPUT + -0.5*PID_YOUTPUT + MOTOR_MIN + MOTOR_RANGE*throttle_input;
//		OC4RS = -0.5*PID_XOUTPUT + -0.5*PID_YOUTPUT + MOTOR_MIN + MOTOR_RANGE*throttle_input;
//		OC2RS = 0.5*PID_XOUTPUT + 0.5*PID_YOUTPUT + MOTOR_MIN + MOTOR_RANGE*throttle_input;
//		OC3RS = -0.5*PID_XOUTPUT + 0.5*PID_YOUTPUT + MOTOR_MIN + MOTOR_RANGE*throttle_input;
//
//		if(OC1RS > MOTOR_MAX) {OC1R = MOTOR_MAX;}
//		if(OC2RS > MOTOR_MAX) {OC2R = MOTOR_MAX;}
//		if(OC3RS > MOTOR_MAX) {OC3R = MOTOR_MAX;}
//		if(OC4RS > MOTOR_MAX) {OC4R = MOTOR_MAX;}
//
//		if(OC1RS < MOTOR_MIN) {OC1R = MOTOR_MIN;}
//		if(OC2RS < MOTOR_MIN) {OC2R = MOTOR_MIN;}
//		if(OC3RS < MOTOR_MIN) {OC3R = MOTOR_MIN;}
//		if(OC4RS < MOTOR_MIN) {OC4R = MOTOR_MIN;}
//	}
}

