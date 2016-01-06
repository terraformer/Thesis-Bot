
#ifndef configuration_h
#define configuration_h

//allgemein
#define MEASURE_SAMPLES 5
#define MEASURE_SAMPLESir 5
#define MEASURE_DELAY 250
#define MEASURE_SAMPLE_DELAYir 10
#define MEASURE_SAMPLE_DELAY 10

// Sonar
#define USONIC_DIV 76.0  // Achtung Temperaturabhängig!

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

char inByte = 'X'; //Serial Input

byte Power = 200; // Motor Power for rotation

#endif