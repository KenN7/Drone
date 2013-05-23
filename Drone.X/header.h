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
#define c_filter        0.90 //coef of the complementary filter (must be <1)

#define UnlockRP        __builtin_write_OSCCONL(OSCCON & 0xBF)
#define LockRP          __builtin_write_OSCCONL(OSCCON | 0x40)

/******************************************************************************/
/* Function Prototypes                                                        */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void ConfigureOscillator(void);    /* Handles clock switching/osc initialization */
void InitApp(void);             /* I/O and global Initialization         */

// Protos for accelerometer and gyroscope
unsigned char Initialize_Accel(void);
unsigned char Read_Accel(volatile int *data);
void Process_Accel(volatile int * raw_data, volatile float * data);

unsigned char Initialize_Gyro(void);
void Calibrate_Gyro(volatile int * raw_data);
unsigned char Read_Gyro(volatile int *data);
void Process_Gyro(volatile int * raw_data, volatile float * data);

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
void Complementary_filter(volatile float * filtered, volatile float * data_gyro, volatile float * data_accel);
void Snd_filter(volatile float * filtered, volatile float * data_gyro, volatile float * data_accel);

//Protos for motors update and PID
void PID();
void Update_PWM();
void testmoteurs();

//Protos for UART
void Init_UART();
void GetData();
void PutCharUART1(unsigned char b);
void PutLong(long i);
void ProcessRxData();