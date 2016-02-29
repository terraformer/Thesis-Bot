#ifndef PINS_H
#define PINS_H

#include "boards.h"

//#if MB(BOARD_V2H) //initial Prototype Board

  ///Audio
  #define buzzer 13

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

  //other pins
  /*
   * 0 (RX) connects to TX of bluetooth module (3.3V -> 5v)
   * 1 (TX) connects to RX of bluetooth module (5v -> 3.3v dont forget voltage divider!)
   * A2 front left IR
   * A3 front right IR
   * A4 SDA (I²C)
   * A5 SCL (I²C)
   * A6 side left IR
   * A7 side right IR
   */
/*
#endif

#if MB(BOARD_V3F)

  ///Audio
  #define buzzer 13

  //Frontsonar (Digital)
  #define TRIGGER_PIN 8
  #define ECHO_PIN 12

  //Pin, der den Transistor für die IR LEDs schaltet.  
  #define Licht 11

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

#endif */

#endif
