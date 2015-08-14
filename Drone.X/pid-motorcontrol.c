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
#define MIN_PWM 5500
#define MAX_PWM 9000 // TODO tweakage necessaire !! les controlleurs se mettent en BO pour des raisons inconnues
#define MES_MIN 6000
#define MES_MAX 9000
//#define MES_MID (MES_MIN+((MES_MAX-MES_MIN)/2))
#define MES_MID 7500
#define PLAGE_MOTOR (MAX_PWM-MIN_PWM)
#define PLAGE_XANGLE 20
#define PLAGE_YANGLE 20
#define PLAGE_ZRATE 50

/******************************************************************************/
/*                         Global variables                                   */
/******************************************************************************/
extern volatile float throttle_input;
extern volatile float roll_input;
extern volatile float pitch_input;
extern volatile float yaw_input;

extern volatile float filtered_angles[2];
extern volatile float filtered_angles2[2];
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

//float KP = 4.0; //25 27/6/12
//float KI = 0.3; //85 5/6/12
//float KD = 0.8; //7 27/6/12
//float ZKP = 5.0; //40 8/6/12
//float ZKD = 0.0; //25 8/6/12

//TODO teweaker les coefs
static float XKP = 10; //25 27/6/12 //0.2  // BAISSEERRRR   //20
static float XKI = 0; //85 5/6/12  //TESERRR               //10
static float XKD = 0; //7 27/6/12 // TESTERRRR              //5

static float YKP = 10;     //Ku=5 ==> Kp=0.6*Ku=3         //10 ==> 6
static float YKI = 0;     //Tu=1sec ==> Ki=2Kp/Tu=6       //1  ==> 12
static float YKD = 0;     //Kd=Kp*Tu/8=0.375              //0.75

static float ZKP = 20; //40 8/6/12
static float ZKD = 0; //25 8/6/12

static int Kdir = 1;


static volatile float TZrate = 0;
static volatile float TXangle = 0;
static volatile float TYangle = 0;

void PID()
{
    TZrate = PLAGE_ZRATE*(float)((float)yaw_input - MES_MID)/(MES_MAX - MES_MIN);
    if(TZrate < 1 && TZrate > -1) {TZrate = 0;}
    TZrate *= Kdir;

    TXangle = PLAGE_XANGLE*(float)((float)roll_input - MES_MID)/(MES_MAX - MES_MIN);
    if(TXangle < 1 && TXangle > -1) {TXangle = 0;}
    TXangle *= Kdir;

    TYangle = PLAGE_YANGLE*(float)((float)(pitch_input - MES_MID)/(MES_MAX - MES_MIN));
    if(TYangle < 1 && TYangle > -1) {TYangle = 0;}
    TYangle *= Kdir;
    
                               //Pour l'asserv, on convertit les valeurs de
                                //5000 à 10000 de l'entree en angle avec une regle de 3
                                // c'est ANGLE_RANGE qui fixe l'angle max.
                                // l'angle max choisi est 30deg, donc de -15 a +15
                                // je pense quon peut aller bien au dessus.
                                // il est peut etre habile de deporter les calculs
                                // dans le mm fichier que le PID pour limiter les
                                // variables globales gitanes entres fichiers.


   PREVIOUS_ERROR[0] = ERROR[0];
   PREVIOUS_ERROR[1] = ERROR[1];
   PREVIOUS_ERROR[2] = ERROR[2];

// avant bullshit MP6050
    ERROR[0] = TXangle - filtered_angles[0]; //roll
    ERROR[1] = TYangle - filtered_angles[1]; //pitch
    ERROR[2] = TZrate - dataG[2]; //speed asserv for yaw
// TODO refactorer et remettre ci dessus et enlever bullshit ci dessous
    //ERROR[0] = TXangle - COMPLEMENTARY_XANGLE; //roll
    //ERROR[1] = TYangle - COMPLEMENTARY_YANGLE; //pitch
    //ERROR[2] = TZrate - GYRO_ZRATE; //speed asserv for yaw

    //DIFFERENTIAL[0] = (ERROR[0] - PREVIOUS_ERROR[0])/dt;
    //DIFFERENTIAL[1] = (ERROR[1] - PREVIOUS_ERROR[1])/dt;

    // de même ici
    DIFFERENTIAL[0] = -dataG[0];
    DIFFERENTIAL[1] = -dataG[1];
    DIFFERENTIAL[2] = -(ERROR[2] - PREVIOUS_ERROR[2])/dt;
    //TODO de même virer code ci dessous et remettre ci dessus
    //DIFFERENTIAL[0] = -GYRO_XRATE;
    //DIFFERENTIAL[1] = -GYRO_YRATE;
    //DIFFERENTIAL[2] = -GYRO_ZRATE;

    INTEGRAL[0] += ERROR[0]*dt;
    INTEGRAL[1] += ERROR[1]*dt;

    if(INTEGRAL[0]> 0.5) {INTEGRAL[0] = 0.5;}
    else if(INTEGRAL[0] < -0.5) {INTEGRAL[0] = -0.5;}
    if(INTEGRAL[1] > 0.5) {INTEGRAL[1] = 0.5;}
    else if(INTEGRAL[1] < -0.5) {INTEGRAL[1] = -0.5;}

    PID_XOUTPUT = ERROR[0]*XKP + DIFFERENTIAL[0]*XKD + INTEGRAL[0]*XKI;
    PID_YOUTPUT = ERROR[1]*YKP + DIFFERENTIAL[1]*YKD + INTEGRAL[1]*YKI;
    PID_ZOUTPUT = ERROR[2]*ZKP + DIFFERENTIAL[2]*ZKD;

    if(PID_ZOUTPUT > 1000){PID_ZOUTPUT = 1000;}
    else if (PID_ZOUTPUT < -1000){PID_ZOUTPUT = -1000;}
}

static volatile float throttle;
static volatile int OC1_output;
static volatile int OC2_output;
static volatile int OC3_output; //x roll, y pitch
static volatile int OC4_output;

void Update_PWM()
{   
    if(throttle_input <= 5700)
    {
        throttle = 0;
    }
    else
    {
        throttle = (float)(throttle_input - MES_MIN)/(MES_MAX-MES_MIN);
    }
    if (throttle > 1.0) throttle=1.0;
    if(throttle < 0.0) throttle=0.0;

    if(throttle==0.0)
    {
	OC1RS = 5000;
	OC2RS = 5000;
	OC3RS = 5000;
	OC4RS = 5000;
    }
    else //X config
    {
        OC1_output = -0.7071*PID_XOUTPUT + -0.7071*PID_YOUTPUT + PID_ZOUTPUT + MIN_PWM + PLAGE_MOTOR*throttle; // right front (Psend1)
	OC2_output = -0.7071*PID_XOUTPUT + 0.7071*PID_YOUTPUT - PID_ZOUTPUT + MIN_PWM + PLAGE_MOTOR*throttle;  //right back (Psend2)
        
	OC3_output = 0.7071*PID_XOUTPUT + 0.7071*PID_YOUTPUT + PID_ZOUTPUT + MIN_PWM + PLAGE_MOTOR*throttle; //left back (Psend3)
	OC4_output = 0.7071*PID_XOUTPUT + -0.7071*PID_YOUTPUT - PID_ZOUTPUT + MIN_PWM + PLAGE_MOTOR*throttle; //left front (Psend4)

	if(OC1_output > MAX_PWM) {OC1_output = MAX_PWM;}
	if(OC2_output > MAX_PWM) {OC2_output = MAX_PWM;}
	if(OC3_output > MAX_PWM) {OC3_output = MAX_PWM;}
	if(OC4_output > MAX_PWM) {OC4_output = MAX_PWM;}

	if(OC1_output < MIN_PWM) {OC1_output = MIN_PWM;}
	if(OC2_output < MIN_PWM) {OC2_output = MIN_PWM;}
	if(OC3_output < MIN_PWM) {OC3_output = MIN_PWM;}
	if(OC4_output < MIN_PWM) {OC4_output = MIN_PWM;}

	//OC1RS = OC1_output;
	OC2RS = OC2_output;
	//OC3RS = OC3_output;
	OC4RS = OC4_output;

    }

//    static int y=0;
//    if (y%100 == 1) {
//        printf("%i,%i\n",OC2_output,OC4_output);
//        //printf("%g,%g,%g,%g\n",(double)PID_XOUTPUT,(double)PID_YOUTPUT,(double)PID_ZOUTPUT,(double)throttle);
//    }
//    y+=1;
}

void testmoteurs()
{
    OC1RS = throttle_input;
    OC2RS = throttle_input;
    OC3RS = throttle_input;
    OC4RS = throttle_input;
}

