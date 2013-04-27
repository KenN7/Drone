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
#include "timer.h"

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
    _IC1R = 5;
    _IC2R = 6;
    _IC7R = 7;
    _IC8R = 20;

    IC1CONbits.ICSIDL = 0; //Continue in idle mode
    IC1CONbits.ICTMR = 1; //Use Timer2
    IC1CONbits.ICI = 0b00; //Interrupt on every capture
    IC1CONbits.ICM = 0b001; //Every edge
    IC2CONbits.ICSIDL = 0; //Continue in idle mode
    IC2CONbits.ICTMR = 1; //Use Timer2
    IC2CONbits.ICI = 0b00; //Interrupt on every capture
    IC2CONbits.ICM = 0b001; //Act as interrupt pin only
    IC7CONbits.ICSIDL = 0; //Continue in idle mode
    IC7CONbits.ICTMR = 1; //Use Timer2
    IC7CONbits.ICI = 0b00; //Interrupt on every capture
    IC7CONbits.ICM = 0b001; //Act as interrupt pin only
    IC8CONbits.ICSIDL = 0; //Continue in idle mode
    IC8CONbits.ICTMR = 1; //Use Timer2
    IC8CONbits.ICI = 0b00; //Interrupt on every capture
    IC8CONbits.ICM = 0b001; //Act as interrupt pin only

    IEC0bits.IC1IE = 1; //Enable interrupt
    IPC0bits.IC1IP = 0b110; //Priority 6
    IEC0bits.IC2IE = 1; //Enable interrupt
    IPC1bits.IC2IP = 0b110; //Priority 6
    IEC1bits.IC7IE = 1; //Enable interrupt
    IPC5bits.IC7IP = 0b110; //Priority 6
    IEC1bits.IC8IE = 1; //Enable interrupt
    IPC5bits.IC8IP = 0b110; //Priority 6
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