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

/*****************************************************************************/
/*                   Global variable of Uart received data                   */
/*****************************************************************************/

typedef volatile struct {
    unsigned char id[10];
    unsigned char len;
    int params[2]; // fonction parameters
    int rdy; //response ready
} uart_data;
uart_data data_RX;

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
    int pos;
    int val = 0;

    while(DataRdyUART1())
    {
        b = ReadUART1();

        if(state == 0) // id de l'ordre
        {
            data_RX.id[pos++] = b;
            if (b==' ')
            {
                state = 1;
                pos = 0;
            }
        }
        else if(state == 1) //number of params
        {
            data_RX.len = b;
            state = 2;
            pos = data_RX.len;
        }
        else if(state == 2) //valeurs
        {
            if (pos == 0) 
            {
                state = 3;
            }
            else if (pos > 0 && b==' ')
            {
                data_RX.params[data_RX.len-pos] = val;
                pos--;
                val = 0;
            }
            else
            {
                val = val*10 + b - '0';
            }
        }
        else if(state == 3 && b=='\n') //stop
        {
             data_RX.rdy = 1;
             state = 0;
        }
        else
        {
             data_RX.rdy = -1; // Erreur.
             state = 0;
        }
    }
}

void PutCharUART1(unsigned char b)
{
    while(BusyUART1());
    WriteUART1(b);
}

void PutLong(long i)
{
    long j;
    if( i<0 )
    {
        PutCharUART1('-');
        if (i != 2166136261u)
        {
            j = -i;
        }
        else 
        {
            PutCharUART1('2');
            j = 147483648;
        }   
    }
    else j=i;
    if (j/10)
        PutLong(j/10);
    PutCharUART1('0' + j % 10);
}

void ProcessRxData()
{
//typedef volatile struct {
//    unsigned char id[10];
//    unsigned char len;
//    int params[2]; // fonction parameters
//    int rdy; //response ready
//} uart_data;

    if (data_RX.rdy = 1)
    {
        if ((strcmp(data_RX.id, "getid"))==0)
        {
            putsUART1((unsigned int*)"id drone = 1\n");
        }
        else if ((strcmp(data_RX.id, "Kp"))==0)
        {
            putsUART1((unsigned int*)"val de Kp :\n");
        }
        else
        {
            putsUART1((unsigned int*)"id not recognized!");
        }
    }
    else if (data_RX.rdy = -1)
    {
        putsUART1((unsigned int*)"Reception Error!\n");
    }
    else
    {
        putsUART1((unsigned int*)"Data not rdy!\n");
    }
}

/*************************************************
 *          RX Interrupt
 *
 *************************************************/


void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
     data_RX.rdy = 0;
     GetData();
     ProcessRxData();
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