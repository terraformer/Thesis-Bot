/* 
Copyright 2016 Christian Liebl. 

Licenced under the GNU GPL Version 3.

Contact: christian.liebl@live.de

Pin Configuration
*/

#ifndef boards_h
#define boards_h

//Frontsonar (Digital)
#define TRIGGER_PIN 11
#define ECHO_PIN 12

//Pin, der den Transistor für die IR LEDs schaltet.  
#define Licht 8

//Motorpins (PWM fähig)
#define B1A 10
#define B1B 5
#define A1A 6
#define A1B 9

//encoderpins (Interrupt)
#define rot1a 2
#define rot1b 3
#define rot2a 4
#define rot2b 7

//int distanceIR; (Analog)
#define IRRechts A1  //Photo resistor Pin Boden
#define IRLinks A0  // Photo resistor Pin Boden

#endif
