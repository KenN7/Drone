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
int l;

void Set_Pixels(int numled, unsigned short int r, unsigned short int g, unsigned short int b){
    pixel[numled*3]=g;
    pixel[numled*3+1]=r;
    pixel[numled*3+2]=b;
}

void Set_led(int l){ //Envoie au cercle de led les instruction contenue dans pixel[48]
    int cpt=0;
    while(cpt<120){
        pixel[cpt]=pixel[cpt]/(100-l);
        cpt++;
    }

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

void flash(int l){
    numled=0;
    r=255;
    g=255;
    b=255;
    while(numled<40){
        Set_Pixels(numled,r,g,b);
        numled++;
    }
    Set_led(l);
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
    Set_led(l);
}

void SOS(int l){
    int i;
    for(i=0; i==3; i++){
        flash(l);
        __delay_ms(250);
        eteindre();
        __delay_ms(250);
    }
    for(i=0; i==3; i++){
        flash(l);
        __delay_ms(500);
        eteindre();
        __delay_ms(500);
    }
    for(i=0; i==3; i++){
        flash(l);
        __delay_ms(250);
        eteindre();
        __delay_ms(250);
    }
}

void turning_point (int l){//point bleu qui fait un tour de cercle.
    numled=0;
    r=0;
    g=128;
    b=255;
    while(numled<40){
        Set_Pixels(numled,r,g,b);
        Set_led(l);
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

void degrade(int l){
    numled=0;
    r=255;
    g=0;
    b=0;
    while(g<255){
        while(numled<40){
            Set_Pixels(numled,r,g,b);
            numled++;
        }
        Set_led(l);
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
        Set_led(l);
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
        Set_led(l);
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
        Set_led(l);
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
        Set_led(l);
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
        Set_led(l);
        b--;
        numled=0;
        __delay_ms(25);
    }
}//le cercle entier change de couleur en faisant un dégradé

void ironmanheart(int l){
    numled=0;
    r=1;
    g=1;
    b=63;

    while(g<63){
        while(numled<40){
            Set_Pixels(numled,r,g,b);
            numled++;
        }
        Set_led(l);
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
        Set_led(l);
        r=r-1;
        g=g-1;
        b=b-1;
        numled=0;
        __delay_ms(25);
    }
    
}

void stack_point(int l){
    int a=0;
    numled=0;
    r=128;
    g=0;
    b=0;
    Set_Pixels(numled,r,g,b);
    Set_led(l);
    __delay_ms(100);
    numled++;

    while(a<24){
        while(numled<(24-a)){
            Set_Pixels(numled-1,0,g,b);
            Set_Pixels(numled,r,g,b);
            Set_led(l);
            numled++;
            __delay_ms(100);
        }
        a++;
        numled=0;
        Set_Pixels(numled,r,g,b);
        Set_led(l);
        __delay_ms(100);
        numled++;
    }
}

void arc_en_ciel(int l){
    Set_Pixels(0,204,0,0);
    Set_Pixels(1,204,51,0);
    Set_Pixels(2,204,102,0);
    Set_Pixels(3,204,153,0);
    Set_Pixels(4,204,204,0);
    Set_Pixels(5,153,204,0);
    Set_Pixels(6,102,204,0);
    Set_Pixels(7,51,204,0);
    Set_Pixels(8,0,204,0);
    Set_Pixels(9,0,204,51);
    Set_Pixels(10,0,204,102);
    Set_Pixels(11,0,204,153);
    Set_Pixels(12,0,204,204);
    Set_Pixels(13,0,153,204);
    Set_Pixels(14,0,102,204);
    Set_Pixels(15,0,51,204);
    Set_Pixels(16,0,0,204);
    Set_Pixels(17,51,0,204);
    Set_Pixels(18,102,0,204);
    Set_Pixels(19,153,0,204);
    Set_Pixels(20,204,0,204);
    Set_Pixels(21,204,0,153);
    Set_Pixels(22,204,0,102);
    Set_Pixels(23,204,0,51);

    Set_led(l);
}

