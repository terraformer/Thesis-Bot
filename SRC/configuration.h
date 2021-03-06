/* 
Copyright 2016 Christian Liebl. 

Licenced under the GNU GPL Version 3.

Contact: christian.liebl@live.de

configuration
*/

#ifndef configuration_h
#define configuration_h

#include "boards.h"

//allgemein
#define MEASURE_SAMPLES 5
#define MEASURE_SAMPLESir 5
//#define MEASURE_DELAY 250
#define MEASURE_SAMPLE_DELAYir 10
#define MEASURE_SAMPLE_DELAY 10

// Sonar
#define USONIC_DIV 76.0  // Achtung Temperaturabhängig!

//Board Revision
#define PCB BOARD_V2H

// Startwerte zur Kompasskalibrierung
int minX = 2000;
int maxX = -2000;
int minY = 1000;
int maxY = -2000;
int offX = 0;
int offY = 0;

// Magnetometer
HMC5883L compass;

//Variablen zum Speichern der Drehposition der Räder
volatile int rotpos1 = 0;
volatile int rotpos2 = 0;

int Ursprung; 

char RX = 'X'; //Serial Input Chars

//motor power
byte Power = 255; // for rotation
byte lpower = 255; // left wheel
byte rpower = 255;  //right wheel

int floorsafe = 60; // Min Sensor value for safe floor distance
int frontsafe = 60;

//Accel Gyro MPU 6050
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
double roll,pitch;
uint32_t timer;

#endif
