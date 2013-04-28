/* 
 * File:   inputcapt.c
 * Author: Ken
 *
 * Created on 27 avril 2013, 13:39
 */

#include <p33Fxxxx.h>      /* Includes device header file                     */
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include "header.h"        /* Function / Parameters                           */
#include <libpic30.h>
#include <stdio.h>
#include <stdlib.h>
#include <timer.h>
#include <incap.h>

extern volatile float throttle_input;
extern volatile float roll_input;
extern volatile float pitch_input;
extern volatile float yaw_input;

/*
 * 
 */
void Initialize_T2()
{
    OpenTimer2(T2_OFF & T2_GATE_OFF & T2_PS_1_1 & T2_SOURCE_INT, 0xFFFF);
    DisableIntT2;
}

void ReStart_T2()
{
    T2CONbits.TON = 0;
    WriteTimer2(0);
    T2CONbits.TON = 1;
}

void Initialize_IC()
{
    _IC1R = 5; //Tie IC1 on RP5
    _IC2R = 6; //Tie IC2 on RP6
    _IC7R = 7; //Tie IC7 on RP7
    _IC8R = 20; //Tie IC8 on RP20

    OpenCapture1(IC_IDLE_CON & IC_TIMER2_SRC & IC_INT_1CAPTURE & IC_EVERY_EDGE);
    OpenCapture2(IC_IDLE_CON & IC_TIMER2_SRC & IC_INT_1CAPTURE & IC_EVERY_EDGE);
    OpenCapture7(IC_IDLE_CON & IC_TIMER2_SRC & IC_INT_1CAPTURE & IC_EVERY_EDGE);
    OpenCapture8(IC_IDLE_CON & IC_TIMER2_SRC & IC_INT_1CAPTURE & IC_EVERY_EDGE);

    ConfigIntCapture1(IC_INT_ON & IC_INT_PRIOR_6);
    ConfigIntCapture2(IC_INT_ON & IC_INT_PRIOR_6);
    ConfigIntCapture7(IC_INT_ON & IC_INT_PRIOR_6);
    ConfigIntCapture8(IC_INT_ON & IC_INT_PRIOR_6);
}


void __attribute__((interrupt, auto_psv)) _IC1Interrupt(void)
{
    int useless;
    if(_RB5)
    {
    	ReStart_T2();
    	useless = IC1BUF;
    }
    else if(!_RB5)
    {
       // yaw_input = IC1BUF;
       // TARGET_ZRATE = ZRATE_RANGE*(yaw_input - YAW_MID)/(YAW_MAX - YAW_MIN);
        //if(TARGET_ZRATE < 1.0 && TARGET_ZRATE > -1.0) {TARGET_ZRATE = 0;}
    }
   // Reset_Timer4();
    _IC1IF = 0; //Clear IC1 interrupt flag
}

void __attribute__((interrupt, auto_psv)) _IC2Interrupt(void)
{
    int useless;
    if(_RB6)
    {
	ReStart_T2();
	useless = IC2BUF;
    }
    else if(!_RB6)
    {
//	throttle_input = IC2BUF;
//	if(throttle_input <= THROTTLE_THRESHOLD)
//    	{
//            throttle = 0.0;
//	}
//        else
//        {
//            throttle = (float)(throttle_input - THROTTLE_MIN)/(THROTTLE_MAX-THROTTLE_MIN);
//        }
    }
//  Reset_Timer4();
    _IC2IF = 0; //Clear IC2 interrupt flag
}

void __attribute__((interrupt, auto_psv)) _IC7Interrupt(void)
{
    int useless;
    if(_RB7)
    {
	ReStart_T2();
	useless = IC7BUF;
    }
    else if(!_RB7)
    {
//	roll_input = IC7BUF;
//    	TARGET_XANGLE = XANGLE_RANGE*(roll_input - ROLL_MID)/(ROLL_MAX - ROLL_MIN);
//    	//if(TARGET_XANGLE < 1.0 && TARGET_XANGLE > -1.0) {TARGET_XANGLE = 0;}
    }
//   Reset_Timer4();
    _IC7IF = 0; //Clear IC7 interrupt flag
}

void __attribute__((interrupt, auto_psv)) _IC8Interrupt(void)
{
    int useless;
    if(_RC4)
    {
	ReStart_T2();
	useless = IC8BUF;
    }
    else if(!_RC4)
    {
//	pitch_input = IC8BUF;
//	TARGET_YANGLE = YANGLE_RANGE*(float)((float)(pitch_input - PITCH_MID)/(PITCH_MAX - PITCH_MIN));
//	//if(TARGET_YANGLE < 1.0 && TARGET_YANGLE > -1.0) {TARGET_YANGLE = 0;}
    }
    //Reset_Timer4();
    _IC8IF = 0; //Clear IC8 interrupt flag
}