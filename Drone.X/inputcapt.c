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
    OpenTimer2(T2_OFF & T2_GATE_OFF & T2_PS_1_8 & T2_SOURCE_INT, 0xFFFF);
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
    UnlockRP;
    _IC1R = 5; //Tie IC1 on RP5
    _IC2R = 6; //Tie IC2 on RP6
    _IC7R = 7; //Tie IC7 on RP7
    _IC8R = 19; //Tie IC8 on RP19
    LockRP;
    
    _TRISB5 = 1;
    _TRISB6 = 1;
    _TRISB7 = 1;
    _TRISC3 = 1;

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
        throttle_input = IC1BUF;
    }

    if(throttle_input<5000){throttle_input=5000;}
    if(throttle_input>9000){throttle_input=9000;}

   // Reset_Timer4(); //Watchdog Timer
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
        roll_input = IC2BUF;
    }

    if(roll_input<5000){roll_input=5000;}
    if(roll_input>9000){roll_input=9000;}

//  Reset_Timer4(); //Watchdog Timer
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
        pitch_input = IC7BUF;
    }

    if(pitch_input<5000){pitch_input=5000;}
    if(pitch_input>9000){pitch_input=9000;}

//   Reset_Timer4(); //Watchdog Timer
    _IC7IF = 0; //Clear IC7 interrupt flag
}

void __attribute__((interrupt, auto_psv)) _IC8Interrupt(void)
{
    int useless;
    if(_RC3)
    {
	ReStart_T2();
	useless = IC8BUF;
    }
    else if(!_RC3)
    {
        yaw_input = IC8BUF;
    }

    if(yaw_input<5000){yaw_input=5000;}
    if(yaw_input>9000){yaw_input=9000;}

    //Reset_Timer4(); //Watchdog Timer
    _IC8IF = 0; //Clear IC8 interrupt flag
}
