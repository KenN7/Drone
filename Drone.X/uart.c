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
    unsigned char id[10]; //id of message
    unsigned char len; //lengh of message
    int params[2]; // fonction parameters
    int rdy; //response ready
} uart_data;

uart_data data_RX;

void Init_UART()
{
    UnlockRP;
_U1RXR = 24; //Tie RX to RP5
_TRISC8 = 1;	//Set RB5 as input
_RP25R = 0b00011; //Tie TX to RP6
_TRISC9 = 0;	//Set RB6 as output
    LockRP;
U1MODEbits.UARTEN = 0; // Bit15 TX, RX DISABLED, ENABLE at end of func
//U1MODEbits.notimplemented; // Bit14
U1MODEbits.USIDL = 0; // Bit13 Continue in Idle
U1MODEbits.IREN = 0; // Bit12 No IR translation
U1MODEbits.RTSMD = 0; // Bit11 Simplex Mode
//U1MODEbits.notimplemented; // Bit10
U1MODEbits.UEN = 0; // Bits8,9 TX,RX enabled, CTS,RTS not
U1MODEbits.WAKE = 0; // Bit7 No Wake up (since we don't sleep here)
U1MODEbits.LPBACK = 0; // Bit6 No Loop Back
U1MODEbits.ABAUD = 0; // Bit5 No Autobaud (would require sending '55')
U1MODEbits.URXINV = 0; // Bit4 IdleState = 1 (for dsPIC)
U1MODEbits.BRGH = 0; // Bit3 16 clocks per bit period
U1MODEbits.PDSEL = 0b00; // Bits1,2 8bit, no Parity
U1MODEbits.STSEL = 0; // Bit0 One Stop Bit
// Load a value into Baud Rate Generator.
// See section 17.3.1 of datasheet.
// U2BRG = (Fcy/(16*BaudRate))-1
// U2BRG = ((79.2275E6/2)/(16*57600))-1
// U2BRG = 41.98367
U1BRG = BRGVAL; // 43 80Mhz osc, 57600 Baud
// Load all values in for U1STA SFR
U1STAbits.UTXISEL1 = 0; //Bit15 Int when Char is transferred (1/2 config!)
U1STAbits.UTXINV = 0; //Bit14 N/A, IRDA config
U1STAbits.UTXISEL0 = 0; //Bit13 Other half of Bit15
//U2STAbits.notimplemented = 0; //Bit12
U1STAbits.UTXBRK = 0; //Bit11 Disabled
U1STAbits.UTXEN = 0; //Bit10 TX pins controlled by periph
U1STAbits.UTXBF = 0; //Bit9 *Read Only Bit*
U1STAbits.TRMT = 0; //Bit8 *Read Only bit*
U1STAbits.URXISEL = 0; //Bits6,7 Int. on character recieved
U1STAbits.ADDEN = 0; //Bit5 Address Detect Disabled
U1STAbits.RIDLE = 0; //Bit4 *Read Only Bit*
U1STAbits.PERR = 0; //Bit3 *Read Only Bit*
U1STAbits.FERR = 0; //Bit2 *Read Only Bit*
U1STAbits.OERR = 0; //Bit1 *Read Only Bit*
U1STAbits.URXDA = 0; //Bit0 *Read Only Bit*
IPC7 = 0x4400; //Mid Range Interrupt Priority level, no urgent reason
IFS0bits.U1TXIF = 0; //Clear the Transmit Interrupt Flag
IEC0bits.U1TXIE = 0; //Disable Transmit Interrupts
IFS0bits.U1RXIF = 0; //Clear the Recieve Interrupt Flag
IEC0bits.U1RXIE = 0; //Disable Recieve Interrupts
U1MODEbits.UARTEN = 1; //Turn the peripheral on
U1STAbits.UTXEN = 1;
printf("\nUART Setup Complete");
  /*
    OpenUART2(UART_EN & UART_IDLE_CON & UART_IrDA_DISABLE & UART_MODE_FLOW
        & UART_UEN_00 & UART_DIS_WAKE & UART_DIS_LOOPBACK
        & UART_DIS_ABAUD & UART_UXRX_IDLE_ONE & UART_BRGH_SIXTEEN
        & UART_NO_PAR_8BIT & UART_1STOPBIT,
          UART_INT_TX_BUF_EMPTY & UART_IrDA_POL_INV_ZERO
        & UART_SYNC_BREAK_DISABLED & UART_TX_ENABLE & UART_TX_BUF_NOT_FUL & UART_INT_RX_CHAR
        & UART_ADR_DETECT_DIS & UART_RX_OVERRUN_CLEAR,
          BRGVAL);

    ConfigIntUART2(UART_RX_INT_PR4 & UART_RX_INT_EN
                 & UART_TX_INT_PR4 & UART_TX_INT_DIS);

    UnlockRP;
     _U2RXR = 25; // U1RX on RP25
     _RP24R = 0b00101;  // U1TX (p.167) on RP24
    LockRP;*/
}

static int state = 0;

void GetData()
{
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

    if (data_RX.rdy == 1)
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
    else if (data_RX.rdy == -1)
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