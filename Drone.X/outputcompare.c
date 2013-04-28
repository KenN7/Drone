/* 
 * File:   outputcompare.c
 * Author: Ken
 *
 * Created on 27 avril 2013, 14:51
 */

#include <p33Fxxxx.h>      /* Includes device header file                     */
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include "header.h"        /* Function / Parameters                           */
#include <libpic30.h>
#include <stdio.h>
#include <stdlib.h>
#include <outcompare.h>
#include <timer.h>

/*
 * 
 */

void Initialize_T3()
{
    OpenTimer3(T3_OFF & T3_GATE_OFF & T3_PS_1_1 & T3_SOURCE_INT, 0xFFFF);
    DisableIntT3;
}

void ReStart_T3()
{
    T3CONbits.TON = 0;
    WriteTimer3(0);
    T3CONbits.TON = 1;
}
void Initialize_OC()
{
    _RP10R = 0b10010; //Tie OC1 to RP10
    _RP11R = 0b10011; //Tie OC2 to RP11
    _RP12R = 0b10100; //Tie OC3 to RP12
    _RP13R = 0b10101; //Tie OC4 to RP13

    OpenOC1(OC_IDLE_CON & OC_TIMER3_SRC & OC_OFF, 65535, 700);
    OpenOC2(OC_IDLE_CON & OC_TIMER3_SRC & OC_OFF, 65535, 700);
    OpenOC3(OC_IDLE_CON & OC_TIMER3_SRC & OC_OFF, 65535, 700);
    OpenOC4(OC_IDLE_CON & OC_TIMER3_SRC & OC_OFF, 65535, 700);
    //OC_PWM_FAULT_PIN_ENABLE

    ConfigIntOC1(OC_INT_OFF & OC_INT_PRIOR_2);
    ConfigIntOC2(OC_INT_OFF & OC_INT_PRIOR_2);
    ConfigIntOC3(OC_INT_OFF & OC_INT_PRIOR_2);
    ConfigIntOC4(OC_INT_OFF & OC_INT_PRIOR_2);
    OC1R = 7;
}
// Pour changer la largeur du PWM il faut mettre OC1R entre 0 et 65535
void Start_OC()
{
    OC1CONbits.OCM = 0b110;
    OC2CONbits.OCM = 0b110;
    OC3CONbits.OCM = 0b110;
    OC4CONbits.OCM = 0b110;
    ReStart_T3();
}
