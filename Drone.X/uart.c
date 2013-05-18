/*
* Template dsPIC33F
* Compiler : Microchip xC16
* uC : 33FJ64MC804
* Juillet 2012
*    ____________      _           _
*   |___  /| ___ \    | |         | |
*      / / | |_/ /___ | |__   ___ | |_
*     / /  |    // _ \| '_ \ / _ \| __|
*    / /   | |\ \ (_) | |_) | (_) | |_
*   /_/    |_| \_\___/|____/ \___/'\__|
*			      7robot.fr
*/

#include <p33Fxxxx.h>      /* Includes device header file                     */
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include "header.h"        /* Function / Parameters                           */
#include <libpic30.h>
#include <stdio.h>
#include <stdlib.h>
#include <uart.h>
#include <string.h>

//Baud Rate pour la liaison UART
#define BAUDRATE 115200
#define BRGVAL ((FCY / BAUDRATE / 16) - 1)

void Init_UART()
{
    OpenUART1(UART_EN & UART_IDLE_CON & UART_IrDA_DISABLE & UART_MODE_FLOW
        & UART_UEN_00 & UART_DIS_WAKE & UART_DIS_LOOPBACK
        & UART_DIS_ABAUD & UART_UXRX_IDLE_ONE & UART_BRGH_SIXTEEN
        & UART_NO_PAR_8BIT & UART_1STOPBIT,
          UART_INT_TX_BUF_EMPTY & UART_IrDA_POL_INV_ZERO
        & UART_SYNC_BREAK_DISABLED & UART_TX_ENABLE & UART_TX_BUF_NOT_FUL & UART_INT_RX_CHAR
        & UART_ADR_DETECT_DIS & UART_RX_OVERRUN_CLEAR,
          BRGVAL);

    ConfigIntUART1(UART_RX_INT_PR4 & UART_RX_INT_EN
                 & UART_TX_INT_PR4 & UART_TX_INT_DIS);

    UnlockRP;
     _U1RXR = 25; // U1RX on RP25
     _RP24R = 3;  // U1TX (p.167) on RP24
    LockRP;
}

static int state = 0;
void GetData() {
    unsigned char b;
    while(DataRdyUART1())
    {
        b = ReadUART1();

        if(state == 0 && b == '\n')
            state = 1;
        else if(state == 1)
        {
            posAX = -3;
            checksumAX = 0;
            responseAX.len = 1;
        }
        else if(state == 2)
        {
            posAX = -2;
            responseAX.id = b;
        }
        else if(state == 3 && b < 2 + 4 /*taille de ax.parameters*/)
        {
            posAX = -1;
            checksumAX = responseAX.id + b;
            responseAX.len = b - 2;
        }
        else
            posAX = -5; // Erreur.
    }
}

/*************************************************
 *          RX Interrupt
 *
 *************************************************/


void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
 //   Machine();
    _U1RXIF = 0;      // On baisse le FLAG
}

/*************************************************
 *          TX Interrupt
 *
 *************************************************/


void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
{
   _U1TXIF = 0; // clear TX interrupt flag
}