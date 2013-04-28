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
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        80000000 //7370000L
#define FCY             SYS_FREQ/2

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

#define led1            _LATA2
#define led2            _LATA3
#define led3            _LATA4
#define T1              400 //Timer 1 frequency
#define dt              1/T1 //Timer 1 duration
#define c_filter        0.95 //coef of the complementary filter (must be <1)

/******************************************************************************/
/* Function Prototypes                                                        */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void ConfigureOscillator(void);    /* Handles clock switching/osc initialization */
void InitApp(void);             /* I/O and Peripheral Initialization          */

// Protos for accelerometer and gyroscope
unsigned char Initialize_Accel(void);
unsigned char Read_Accel(int *data);
void Process_Accel(int * raw_data, float * data);

unsigned char Initialize_Gyro(void);
unsigned char Read_Gyro(int *data);
void Process_Gyro(int * raw_data, float * data);

//Protos for T2 and InputCapture
void Initialize_T2();
void ReStart_T2();
void Initialize_IC();

//Protos for T3 and Outputcompare
void Initialize_T3();
void ReStart_T3();
void Initialize_OC();
void Start_OC();

//Protos for T1 and the filter for accel and gyros values
void Initialize_T1();
void ReStart_T1();
float complementary_filter(float value, float gyro, float accel);