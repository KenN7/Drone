/*
* Template dsPIC33F
* Compiler : Microchip xC16
* �C : 33FJ64MC802
* Juillet 2012
*    ____________      _           _
*   |___  /| ___ \    | |         | |
*      / / | |_/ /___ | |__   ___ | |_
*     / /  |    // _ \| '_ \ / _ \| __|
*    / /   | |\ \ (_) | |_) | (_) | |_
*   /_/    |_| \_\___/|____/ \___/'\__|
*			      7robot.fr
*/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <p33Fxxxx.h>      /* Includes device header file                     */
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include "header.h"        /* Function / Parameters                           */
#include <timer.h>
#include "delay.h"
#include <libpic30.h>

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/
extern unsigned int InitI2C(void);

void ConfigureOscillator(void)
{
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBDbits.PLLDIV = 41; // M=43
    CLKDIVbits.PLLPOST = 0; // N1=2
    CLKDIVbits.PLLPRE  = 0; // N2=2
    // Fosc = M/(N1.N2)*Fin
}

/******************************************************************************/
/* Global variables                                                             */
/******************************************************************************/

int raw_dataA[3];
int raw_dataG[4];
float dataA[2];
float dataG[3];
float filtered_angles[2];

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

void InitApp(void)
{
    //pin des LEDs en sortie
    _TRISA2 = 0;
    _TRISA3 = 0;
    _TRISA4 = 0;
    //Si on a un interrupteur sur la pin RB5 (par exemple), on la met en entree
    //_TRISB5 = 1;
    //Et on active la pullup qui va bien (registres CNPU1 et CNPU2)
    //_CN27PUE = 1;
    // activation de la priorite des interruptions
    _NSTDIS = 0;
    
    AD1PCFGL = 0x1FF; //desactivation of analogue pins
    
    InitI2C();
    __delay_ms(500);
    Initialize_Accel(); //I2C init
    Initialize_Gyro();
    Calibrate_Gyro(raw_dataG);

    Initialize_T2(); //Timer 2 for Input Capture
    Initialize_IC();

    Initialize_T3(); //Timer 3 for Output compare
    Initialize_OC();

    Initialize_T1(); //Timer 1 for control loop
    //LED WARNING :
    led1 = 1; led2 = 1; led3 = 1; __delay_ms(500); led1 = 0; led2 = 0; led3 = 0; __delay_ms(500);
    led1 = 1; led2 = 1; led3 = 1; __delay_ms(500); led1 = 0; led2 = 0; led3 = 0; __delay_ms(500); 
    led1 = 1; led2 = 1; led3 = 1; __delay_ms(500); led1 = 0; led2 = 0; led3 = 0; __delay_ms(500);
    led1 = 1; led2 = 1; led3 = 1; __delay_ms(1000);
    
    Start_OC();
    ReStart_T1();
}

// Some functions definitions
void Initialize_T1()
{
    OpenTimer1(T1_OFF & T1_GATE_OFF & T1_PS_1_8 & T1_SOURCE_INT, 12500);
    ConfigIntTimer1(T1_INT_PRIOR_3 & T1_INT_ON);
}

void ReStart_T1()
{
    T1CONbits.TON = 0;
    WriteTimer1(0);
    T1CONbits.TON = 1;
}

//Runs a complementary filter configured via coef c_filter (in header.h)

/*!\ TODO CHECKER SI LES NUMEROS DE TABLEAUX CORRESPONDENT AUX MEMES ANGLES !!! /!\*/

void Complementary_filter(float * filtered, float * data_gyro, float * data_accel)
{
    filtered[0] = data_gyro[0]*c_filter + data_accel[0]*(1-c_filter); // Here gyro is an angle
    filtered[1] = data_gyro[1]*c_filter + data_accel[1]*(1-c_filter);
    //filtered_value = (value + gyro*dt)*c_filter + accel*(1-c_filter); // Here gyro is a rate (angle/dt)
    //COMPLEMENTARY_YANGLE = (COMPLEMENTARY_YANGLE + GYRO_YRATE*dt)*a + ACCEL_YANGLE*(1-a);
}

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/
/* Add interrupt routine code here. */

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void)
{
    Read_Accel(raw_dataA);
    Read_Gyro(raw_dataG);
    Process_Accel(raw_dataA, dataA);
    Process_Gyro(raw_dataG, dataG);
    Complementary_filter(filtered_angles, dataG, dataA);
    
    OC1RS = 5600;

//	PID();
//      Update_PWM();
    
    led1 = !led1;    // On bascule l'�tat de la LED
    _T1IF = 0;      // On baisse le FLAG
}


/******************************************************************************/
/* Interrupt Vector Options                                                   */
/******************************************************************************/
/*                                                                            */
/* Refer to the C30 (MPLAB C Compiler for PIC24F MCUs and dsPIC33F DSCs) User */
/* Guide for an up to date list of the available interrupt options.           */
/* Alternately these names can be pulled from the device linker scripts.      */
/*                                                                            */
/* dsPIC33F Primary Interrupt Vector Names:                                   */
/*                                                                            */
/* _INT0Interrupt      _C1Interrupt                                           */
/* _IC1Interrupt       _DMA3Interrupt                                         */
/* _OC1Interrupt       _IC3Interrupt                                          */
/* _T1Interrupt        _IC4Interrupt                                          */
/* _DMA0Interrupt      _IC5Interrupt                                          */
/* _IC2Interrupt       _IC6Interrupt                                          */
/* _OC2Interrupt       _OC5Interrupt                                          */
/* _T2Interrupt        _OC6Interrupt                                          */
/* _T3Interrupt        _OC7Interrupt                                          */
/* _SPI1ErrInterrupt   _OC8Interrupt                                          */
/* _SPI1Interrupt      _DMA4Interrupt                                         */
/* _U1RXInterrupt      _T6Interrupt                                           */
/* _U1TXInterrupt      _T7Interrupt                                           */
/* _ADC1Interrupt      _SI2C2Interrupt                                        */
/* _DMA1Interrupt      _MI2C2Interrupt                                        */
/* _SI2C1Interrupt     _T8Interrupt                                           */
/* _MI2C1Interrupt     _T9Interrupt                                           */
/* _CNInterrupt        _INT3Interrupt                                         */
/* _INT1Interrupt      _INT4Interrupt                                         */
/* _ADC2Interrupt      _C2RxRdyInterrupt                                      */
/* _DMA2Interrupt      _C2Interrupt                                           */
/* _OC3Interrupt       _DCIErrInterrupt                                       */
/* _OC4Interrupt       _DCIInterrupt                                          */
/* _T4Interrupt        _DMA5Interrupt                                         */
/* _T5Interrupt        _U1ErrInterrupt                                        */
/* _INT2Interrupt      _U2ErrInterrupt                                        */
/* _U2RXInterrupt      _DMA6Interrupt                                         */
/* _U2TXInterrupt      _DMA7Interrupt                                         */
/* _SPI2ErrInterrupt   _C1TxReqInterrupt                                      */
/* _SPI2Interrupt      _C2TxReqInterrupt                                      */
/* _C1RxRdyInterrupt                                                          */
/*                                                                            */
/* For alternate interrupt vector naming, simply add 'Alt' between the prim.  */
/* interrupt vector name '_' and the first character of the primary interrupt */
/* vector name.  There is no Alternate Vector or 'AIVT' for the 33E family.   */
/*                                                                            */
/* For example, the vector name _ADC2Interrupt becomes _AltADC2Interrupt in   */
/* the alternate vector table.                                                */
/*                                                                            */
/* Example Syntax:                                                            */
/*                                                                            */
/* void __attribute__((interrupt,auto_psv)) <Vector Name>(void)               */
/* {                                                                          */
/*     <Clear Interrupt Flag>                                                 */
/* }                                                                          */
/*                                                                            */
/* For more comprehensive interrupt examples refer to the C30 (MPLAB C        */
/* Compiler for PIC24 MCUs and dsPIC DSCs) User Guide in the                  */
/* <compiler installation directory>/doc directory for the latest compiler    */
/* release.                                                                   */
/*                                                                            */