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
#define MIN_PWM 6500
#define MAX_PWM 10000
#define MES_MIN 5000
#define MES_MAX 10000
#define MES_MID 7500
#define PLAGE_MOTOR 3500
#define PLAGE_XANGLE 30
#define PLAGE_YANGLE 30
#define PLAGE_ZRATE 90

/******************************************************************************/
/*                         Global variables                                   */
/******************************************************************************/
extern volatile float throttle_input;
extern volatile float roll_input;
extern volatile float pitch_input;
extern volatile float yaw_input;

extern volatile float filtered_angles[2];
extern volatile float dataA[2];
extern volatile float dataG[3];
/*
 * c'est la fête du global dans le PID !!
 */
volatile float PID_XOUTPUT;
volatile float PID_YOUTPUT;
volatile float PID_ZOUTPUT;
volatile float PREVIOUS_ERROR[3]; //X, Y , Z
volatile float ERROR[3]; //X, Y , Z
volatile float DIFFERENTIAL[3];
volatile float INTEGRAL[2];

float KP = 26.0; //25 27/6/12
float KI = 200.0; //85 5/6/12
float KD = 7; //7 27/6/12

float ZKP = 40.0; //40 8/6/12
float ZKD = 25.0; //25 8/6/12



void PID()
{
    float TZrate;
    float TXangle;
    float TYangle;

    TZrate = PLAGE_ZRATE*(float)((float)yaw_input - MES_MID)/(MES_MAX - MES_MIN);
    if(TZrate > 1.0 || TZrate < -1.0) {TZrate = 0;}

    TXangle = PLAGE_XANGLE*(float)((float)roll_input - MES_MID)/(MES_MAX - MES_MIN);
    if(TXangle > 1.0 || TXangle < -1.0) {TXangle = 0;}

    TYangle = PLAGE_YANGLE*(float)((float)(pitch_input - MES_MID)/(MES_MAX - MES_MIN));
    if(TYangle > 1.0 || TYangle < -1.0) {TYangle = 0;}
    
                               //Pour l'asserv, on convertit les valeurs de
                                //5000 à 10000 de l'entree en angle avec une regle de 3
                                // c'est ANGLE_RANGE qui fixe l'angle max.
                                // l'angle max choisi est 30deg, donc de -15 a +15
                                // je pense quon peut aller bien au dessus.
                                // il est peut etre habile de deporter les calculs
                                // dans le mm ficier qe le PID pour limiter les
                                // variables globales gitanes entres fichiers.


   PREVIOUS_ERROR[0] = ERROR[0];
   PREVIOUS_ERROR[1] = ERROR[1];
   PREVIOUS_ERROR[2] = ERROR[2];

    ERROR[0] = TXangle - filtered_angles[0];
    ERROR[1] = TYangle - filtered_angles[1];
    ERROR[2] = TZrate - dataG[2]/dt;

//    DIFFERENTIAL[0] = (ERROR[0] - PREVIOUS_ERROR[0])/dt;
//    DIFFERENTIAL[1] = (ERROR[1] - PREVIOUS_ERROR[1])/dt;
//    DIFFERENTIAL[2] = (ERROR[2] - PREVIOUS_ERROR[2])/dt;

    DIFFERENTIAL[0] = -dataG[0]/dt;
    DIFFERENTIAL[1] = -dataG[1]/dt;
    DIFFERENTIAL[2] = -dataG[2]/dt;

    INTEGRAL[0] += ERROR[0]*dt;
    INTEGRAL[1] += ERROR[1]*dt;

//    if(XINTEGRAL > 0.5) {XINTEGRAL = 0.5;}
//    else if(XINTEGRAL < -0.5) {XINTEGRAL = -0.5;}
//    if(YINTEGRAL > 0.5) {YINTEGRAL = 0.5;}
//    else if(YINTEGRAL < -0.5) {YINTEGRAL = -0.5;}

    PID_XOUTPUT = ERROR[0]*KP + DIFFERENTIAL[0]*KD + INTEGRAL[0]*KI;
    PID_YOUTPUT = ERROR[1]*KP + DIFFERENTIAL[1]*KD + INTEGRAL[1]*KI;
    PID_ZOUTPUT = ERROR[2]*ZKP + DIFFERENTIAL[2]*ZKD;

    if(PID_ZOUTPUT > 1000){PID_ZOUTPUT = 1000;}
    else if (PID_ZOUTPUT < -1000){PID_ZOUTPUT = -1000;}
}

void Update_PWM()
{
    float throttle;
    if(throttle_input <= 5500)
    {
        throttle = 0;
    }
    else
    {
        throttle = (float)(throttle_input - MES_MIN)/(MES_MAX-MES_MIN);
    }
    if (throttle > 1.0 || throttle < 0.0) throttle=0;

    if(throttle==0.0)
    {
	OC1RS = 5000;
	OC2RS = 5000;
	OC3RS = 5000;
	OC4RS = 5000;
    }
    else
    {
        int OC1_output;
        int OC2_output;
        int OC3_output;
        int OC4_output;
        OC1_output = 0.7071*PID_XOUTPUT + -0.7071*PID_YOUTPUT + PID_ZOUTPUT + MIN_PWM + PLAGE_MOTOR*throttle;
	OC2_output = 0.7071*PID_XOUTPUT + 0.7071*PID_YOUTPUT - PID_ZOUTPUT + MIN_PWM + PLAGE_MOTOR*throttle;
	OC3_output = -0.7071*PID_XOUTPUT + 0.7071*PID_YOUTPUT + PID_ZOUTPUT + MIN_PWM + PLAGE_MOTOR*throttle;
	OC4_output = -0.7071*PID_XOUTPUT + -0.7071*PID_YOUTPUT - PID_ZOUTPUT + MIN_PWM + PLAGE_MOTOR*throttle;

	if(OC1_output > MAX_PWM) {OC1_output = MAX_PWM;}
	if(OC2_output > MAX_PWM) {OC2_output = MAX_PWM;}
	if(OC3_output > MAX_PWM) {OC3_output = MAX_PWM;}
	if(OC4_output > MAX_PWM) {OC4_output = MAX_PWM;}

	if(OC1_output < MIN_PWM) {OC1_output = MIN_PWM;}
	if(OC2_output < MIN_PWM) {OC2_output = MIN_PWM;}
	if(OC3_output < MIN_PWM) {OC3_output = MIN_PWM;}
	if(OC4_output < MIN_PWM) {OC4_output = MIN_PWM;}

	OC1RS = OC1_output;
	OC2RS = OC2_output;
	OC3RS = OC3_output;
	OC4RS = OC4_output;

    }
}

