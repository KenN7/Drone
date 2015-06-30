/*
* Template dsPIC33F
* Compiler : Microchip xC16
* uC : 33FJ64MC804
* Juin 2015
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
#include <stdio.h>
#include "header.h"        /* Function / Parameters                           */
#include <timer.h>
#include "delay.h"
#include <libpic30.h>

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

int numled=0; //le numéro de la led sur le cercle de 0 à 15
unsigned short int g=0; // l'intensité de vert
unsigned short int r=0; // l'intensité de rouge
unsigned short int b=0; // l'intensité de bleu
unsigned short int pixel[120]; //tableau contenant les valeurs de chaque led

void Set_Pixels(int numled, unsigned short int r, unsigned short int g, unsigned short int b){
    pixel[numled*3]=g;
    pixel[numled*3+1]=r;
    pixel[numled*3+2]=b;
}

void Set_led(void){ //Envoie au cercle de led les instruction contenue dans pixel[48]
    noInterrupts();

    int i=0;//compteur octet
    int j=0;//compteur bit
    unsigned short int m=0b10000000;
    int k;

    _RC7=0;
    __delay_us(50);

    while(i<120){
        while(j<8){
            k = (pixel[i]<<j)&m;
            if(k==0){
                _RC7=1;
                __delay32(14);
                _RC7 = 0;
                __delay32(32);
            }
            else{
                _RC7=1;
                __delay32(28);
                _RC7 = 0;
                __delay32(24);
            }
            j++;
        }
        i++;
        j=0;
    }
    reInterrupts();
}

void flash(void){
    numled=0;
    r=255;
    g=255;
    b=255;
    while(numled<40){
        Set_Pixels(numled,r,g,b);
        numled++;
    }
    Set_led();
}

void eteindre(void){
    numled=0;
    r=0;
    g=0;
    b=0;
    while(numled<40){
        Set_Pixels(numled,r,g,b);
        numled++;
    }
    Set_led();
}

void SOS(void){
    int i;
    for(i=0; i==3; i++){
        flash();
        __delay_ms(250);
        eteindre();
        __delay_ms(250);
    }
    for(i=0; i==3; i++){
        flash();
        __delay_ms(500);
        eteindre();
        __delay_ms(500);
    }
    for(i=0; i==3; i++){
        flash();
        __delay_ms(250);
        eteindre();
        __delay_ms(250);
    }
}

void turning_point (void){//point bleu qui fait un tour de cercle.
    numled=0;
    r=0;
    g=128;
    b=255;
    while(numled<40){
        Set_Pixels(numled,r,g,b);
        Set_led();
        r=0;
        g=0;
        b=0;
        Set_Pixels(numled,r,g,b);
        g=128;
        b=255;
        numled++;
        __delay_ms(100);
    }
}

void degrade(void){
    numled=0;
    r=255;
    g=0;
    b=0;
    while(g<255){
        while(numled<40){
            Set_Pixels(numled,r,g,b);
            numled++;
        }
        Set_led();
        g++;
        numled=0;
        __delay_ms(25);
    }

    numled=0;
    r=255;
    g=255;
    b=0;
    while(r>0){
        while(numled<40){
            Set_Pixels(numled,r,g,b);
            numled++;
        }
        Set_led();
        r--;
        numled=0;
        __delay_ms(25);
    }

    numled=0;
    r=0;
    g=255;
    b=0;
    while(b<255){
        while(numled<40){
            Set_Pixels(numled,r,g,b);
            numled++;
        }
        Set_led();
        b++;
        numled=0;
        __delay_ms(25);
    }

    numled=0;
    r=0;
    g=255;
    b=255;
    while(g>0){
        while(numled<40){
            Set_Pixels(numled,r,g,b);
            numled++;
        }
        Set_led();
        g--;
        numled=0;
        __delay_ms(25);
    }

    numled=0;
    r=0;
    g=0;
    b=255;
    while(r<255){
        while(numled<40){
            Set_Pixels(numled,r,g,b);
            numled++;
        }
        Set_led();
        r++;
        numled=0;
        __delay_ms(25);
    }

    numled=0;
    r=255;
    g=0;
    b=255;
    while(b>0){
        while(numled<40){
            Set_Pixels(numled,r,g,b);
            numled++;
        }
        Set_led();
        b--;
        numled=0;
        __delay_ms(25);
    }
}//le cercle entier change de couleur en faisant un dégradé

void ironmanheart(void){
    numled=0;
    r=1;
    g=1;
    b=63;

    while(g<63){
        while(numled<40){
            Set_Pixels(numled,r,g,b);
            numled++;
        }
        Set_led();
        r++;
        g++;
        b++;
        numled=0;
        __delay_ms(25);
    }

     while(g>0){
        while(numled<40){
            Set_Pixels(numled,r,g,b);
            numled++;
        }
        Set_led();
        r=r-1;
        g=g-1;
        b=b-1;
        numled=0;
        __delay_ms(25);
    }
    
}

void stack_point(void){
    int a=0;
    numled=0;
    r=128;
    g=0;
    b=0;
    Set_Pixels(numled,r,g,b);
    Set_led();
    __delay_ms(100);
    numled++;

    while(a<40){
        while(numled<(40-a)){
            Set_Pixels(numled-1,0,g,b);
            Set_Pixels(numled,r,g,b);
            Set_led();
            numled++;
            __delay_ms(100);
        }
        a++;
        numled=0;
        Set_Pixels(numled,r,g,b);
        Set_led();
        __delay_ms(100);
        numled++;
    }
}

