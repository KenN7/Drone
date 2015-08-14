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

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        80000000 //7370000L
#define FCY             SYS_FREQ/2

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

//#define led1            _LATA2
//#define led2            _LATA3
//#define led3            _LATA4
#define T1              400 //Timer 1 frequency
#define dt              0.0025 //Timer 1 duration
#define c_filter        0.98 //coef of the complementary filter (must be <1)
#define timeConstant    1.1// constant for second order complementary

#define UnlockRP        __builtin_write_OSCCONL(OSCCON & 0xBF)
#define LockRP          __builtin_write_OSCCONL(OSCCON | 0x40)


///// TODO Bullshit a refactorer :
extern signed int ACCEL_XOUT;
extern signed int ACCEL_YOUT;
extern signed int ACCEL_ZOUT;
extern float GYRO_XRATE;
extern float GYRO_YRATE;
extern float GYRO_ZRATE;
extern int GYRO_XRATERAW;
extern int GYRO_YRATERAW;
extern int GYRO_ZRATERAW;
extern unsigned char GYRO_XOUT_L;
extern unsigned char GYRO_XOUT_H;
extern unsigned char GYRO_YOUT_L;
extern unsigned char GYRO_YOUT_H;
extern unsigned char GYRO_ZOUT_L;
extern unsigned char GYRO_ZOUT_H;
extern signed int GYRO_XOUT;
extern signed int GYRO_YOUT;
extern signed int GYRO_ZOUT;
extern unsigned char ACCEL_XOUT_L;
extern unsigned char ACCEL_XOUT_H;
extern unsigned char ACCEL_YOUT_L;
extern unsigned char ACCEL_YOUT_H;
extern unsigned char ACCEL_ZOUT_L;
extern unsigned char ACCEL_ZOUT_H;
extern signed long GYRO_XOUT_OFFSET_1000SUM;
extern signed long GYRO_YOUT_OFFSET_1000SUM;
extern signed long GYRO_ZOUT_OFFSET_1000SUM;
extern float GYRO_XANGLE;
extern float GYRO_YANGLE;
extern float GYRO_ZANGLE;
extern long GYRO_XANGLERAW;
extern long GYRO_YANGLERAW;
extern long GYRO_ZANGLERAW;
extern float ACCEL_XANGLE;
extern float ACCEL_YANGLE;
extern float ACCEL_ZANGLE;
extern signed int GYRO_XOUT_OFFSET;
extern signed int GYRO_YOUT_OFFSET;
extern signed int GYRO_ZOUT_OFFSET;
extern float COMPLEMENTARY_XANGLE;
extern float COMPLEMENTARY_XANGLEPREV;
extern float COMPLEMENTARY_YANGLE;
extern float COMPLEMENTARY_YANGLEPREV;

extern int numled; //le numéro de la led sur le cercle de 0 à 15
extern unsigned short int g; // l'intensité de vert
extern unsigned short int r; // l'intensité de rouge
extern unsigned short int b; // l'intensité de bleu
extern unsigned short int pixel[120];
extern int l; //luminosité en pourcentage.

// END BULLSHIT


/******************************************************************************/
/* Function Prototypes                                                        */
/******************************************************************************/

void ConfigureOscillator(void);    /* Handles clock switching/osc initialization */
void InitApp(void);             /* I/O and global Initialization         */

// Protos for accelerometer and gyroscope
void test_accel(void);
unsigned char Initialize_Accel(void);
unsigned char Read_Accel(volatile int *data);
void Process_Accel(volatile int * raw_data, volatile float * data);

unsigned char Initialize_Gyro(void);
void Calibrate_Gyro(volatile int * raw_data);
unsigned char Read_Gyro(volatile int *data);
void Process_Gyro(volatile int * raw_data, volatile float * data);
void smoothSensorReadings();


//TODO protos pour bullshit MP605
void Setup_MPU6050(void);
void MPU6050_Test_I2C(void);
int MPU6050_Check_Registers(void);
void Calibrate_Gyros(void);
void Get_Accel_Values(void);
void Get_Accel_Angles(volatile float * data);
void Get_Gyro_Rates(volatile float * data);

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

//TODO BULLSHIT
void second_order_complementary_filter(void);

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

//Protos for LED ring Neopixels.
void Set_Pixels(int, unsigned short int, unsigned short int, unsigned short int);
void Set_led(int);
void noInterrupts(void);
void reInterrupts(void);
void flash(int);
void eteindre(void);
void SOS(int);
void turning_point(int);
void degrade(int);
void ironmanheart(int);
void stack_point(int);
void arc_en_ciel(int);
