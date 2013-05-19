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

/******************************************************************************/
/*                            Definitions                                     */
/******************************************************************************/
#define MIN_PWM 5000
#define MAX_PWM 10000

/******************************************************************************/
/*                         Global variables                                   */
/******************************************************************************/
extern volatile float throttle_input;
extern volatile float roll_input;
extern volatile float pitch_input;
extern volatile float yaw_input;
/*
 * 
 */

void PID()
{
            // TARGET_ZRATE = ZRATE_RANGE*(yaw_input - YAW_MID)/(YAW_MAX - YAW_MIN);
            //if(TARGET_ZRATE < 1.0 && TARGET_ZRATE > -1.0) {TARGET_ZRATE = 0;}

            //	if(throttle_input <= THROTTLE_THRESHOLD)
            //    	{
            //            throttle = 0.0;
            //	}
            //        else
            //        {
            //            throttle = (float)(throttle_input - THROTTLE_MIN)/(THROTTLE_MAX-THROTTLE_MIN);
            //        }

            //    	TARGET_XANGLE = XANGLE_RANGE*(roll_input - ROLL_MID)/(ROLL_MAX - ROLL_MIN);
            //    	//if(TARGET_XANGLE < 1.0 && TARGET_XANGLE > -1.0) {TARGET_XANGLE = 0;}

            //	TARGET_YANGLE = YANGLE_RANGE*(float)((float)(pitch_input - PITCH_MID)/(PITCH_MAX - PITCH_MIN));
            //	//if(TARGET_YANGLE < 1.0 && TARGET_YANGLE > -1.0) {TARGET_YANGLE = 0;}
    
                               //Pour l'asserv, on convertit les valeurs de
                                //5000 à 10000 de l'entree en angle avec une regle de 3
                                // c'est ANGLE_RANGE qui fixe l'angle max.
                                // l'angle max choisi est 30deg, donc de -15 a +15
                                // je pense quon peut aller bien au dessus.
                                // il est peut etre habile de deporter les calculs
                                // dans le mm ficier qe le PID pour limiter les
                                // variables globales gitanes entres fichiers.


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
	if(throttle_input==0.0)
	{
		OC1RS = 1000;
		OC2RS = 1000;
		OC3RS = 1000;
		OC4RS = 1000;
	}
	else
	{
		OC1RS = 0.5*PID_XOUTPUT + -0.5*PID_YOUTPUT + MIN_PWM + MOTOR_RANGE*throttle_input;
		OC4RS = -0.5*PID_XOUTPUT + -0.5*PID_YOUTPUT + MIN_PWM + MOTOR_RANGE*throttle_input;
		OC2RS = 0.5*PID_XOUTPUT + 0.5*PID_YOUTPUT + MIN_PWM + MOTOR_RANGE*throttle_input;
		OC3RS = -0.5*PID_XOUTPUT + 0.5*PID_YOUTPUT + MIN_PWM + MOTOR_RANGE*throttle_input;

		if(OC1RS > MAX_PWM) {OC1R = MAX_PWM;}
		if(OC2RS > MAX_PWM) {OC2R = MAX_PWM;}
		if(OC3RS > MAX_PWM) {OC3R = MAX_PWM;}
		if(OC4RS > MAX_PWM) {OC4R = MAX_PWM;}

		if(OC1RS < MIN_PWM) {OC1R = MIN_PWM;}
		if(OC2RS < MIN_PWM) {OC2R = MIN_PWM;}
		if(OC3RS < MIN_PWM) {OC3R = MIN_PWM;}
		if(OC4RS < MIN_PWM) {OC4R = MIN_PWM;}
	}
}

