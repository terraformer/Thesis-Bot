/* 
Copyright 2016 Christian Liebl. 

Licenced under the GNU GPL Version 3.

Contact: christian.liebl@live.de
*/

#include <Wire.h>           //Arduino Core Functions
#include "boards.h"         //PCB Version bzw Pin Zuordnung
#include "HMC5883L.h" 		  //Magnetometer
#include "rotary.h"   		  //Rotary Encoder
#include "configuration.h"  //Global Configuration

// Encoder Handler 
Rotary r = Rotary(rot1a, rot1b);
Rotary r2 = Rotary(rot2a, rot2b);

int distance;
boolean flip = false;

void setup()
{
  Serial.println("Runnung"); //debug

// Initializing Trigger Output and Echo Input
pinMode(TRIGGER_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);

// init motorpins
pinMode(B1A, OUTPUT);
pinMode(B1B, OUTPUT);
pinMode(A1A, OUTPUT);
pinMode(A1B, OUTPUT);

// weitere Interrupts für encoder freischalten
PCICR |= (1 << PCIE2) | (1 << PCIE1);
PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT23);
sei();

// Reset the trigger pin and wait a half a second
digitalWrite(TRIGGER_PIN, LOW);
delayMicroseconds(500);

analogReference(INTERNAL); // Für IR 

Serial.begin(9600);

Serial.println("Booting ...");

// Magnetometer initialisiren ..
mag_setup();

// kalibrieren
Orientieren();

//und auswerten 
Ursprung = richtung();

}

// No tilt compensation
float noTiltCompensate(Vector mag)
{
  float heading = atan2(mag.YAxis, mag.XAxis);
  return heading;
}

void demo_rot90() { // 90 Grad Drehungen auf Basis der Rotationsencoderwerte
  int vier = rotcalib();
  Serial.println("Vierteldrehung");
  Serial.println(vier);
  rotausrichten(vier);
  delay(3000);
  rotausrichten(-vier);
  delay(3000);
  rotausrichten(2*vier);
  delay(3000);
  rotausrichten(-2*vier);
  delay(3000);
}

void distanzieren(int cm) { //Abstand zu Hinternis ändern
  int Wand = measureSONAR();
  while (Wand != cm){
    if (Wand > cm)
      motorVor();
    else
      motorRetour();
    Wand = measureSONAR();
    Serial.println(Wand);
  }
  motorStop();
}

void demoX() { //Zufallsrichtung
  while (true) {
    ausrichten(random(0,360));
    delay(500);
  }
}
void demoY() { // Distanzwechsel   
  distanzieren(10);
  delay(1000);
  distanzieren (30);
}

void tisch_demo0() {
  int Wand = measureSONAR();
  boolean Boden = boden_test();
  int sonarcounter = 0;

  while (boden_test() and Wand > 10) {
    motorVor();
    Boden = boden_test();
    sonarcounter++;
    if (sonarcounter == 20) {
      sonarcounter = 0;
      Wand = measureSONAR();
    }

  }
//  Serial.println(Boden);
//  Serial.println("Stop!");
  motorHalt();
  motorRetour();
  delay(1500);
  int  neu = richtung() + 180;
  if (neu >= 360)
      neu = neu - 360  ; 
  ausrichten(neu);
}

void feature_demo0(){
  int Wand = measureSONAR();
  boolean Boden = boden_test();
  int sonarcounter = 0;

  while (boden_test() and Wand > 10) {
    motorVor();
    Boden = boden_test();
    sonarcounter++;
    if (sonarcounter == 10) {
      sonarcounter = 0;
      Wand = measureSONAR();
    }

  }
//  Serial.println(Boden);
//  Serial.println("Stop!");
  motorHalt();
  motorRetour();//ersetzt durch motor sicherheit
  delay(750);

  if (flip) {
    rechtsausweichen();
    flip = false;  
  //  richtung();    
  } else {
    linksausweichen();
    flip = true;
 //   richtung();
  } 

}

void tisch_demo() { // Vor und zurück mit Hinderniserkennung
  //int Boden = measureIR();
  int Wand = measureSONAR();

  while ((Wand > 10) and boden_test()) {
    motorVor();
    //delay(50);
    //Boden = measureIR();
    Wand = measureSONAR();
  }

  motorHalt();
  motorRetour();
  delay(3000);
 int  neu = richtung() + (int)random(135,225);
  if (neu >= 360)
      neu = neu - 360  ; 
  ausrichten(neu);
}

void drehen_demo(){

ausrichten(90);
delay(3000);

ausrichten(180);
delay(3000);

ausrichten(270);
delay(3000);

ausrichten(0);
delay(3000);

ausrichten(270);
delay(3000);

ausrichten(360);
delay(3000); 
}


//IR Frontsensoren nur teilimplementiert
int LEDcalib() { // Umgebungslicht IR Anteil bestimmen. (Offset)
  // IR der LEDs und der Umgebung
  digitalWrite(Licht,HIGH);
  int Gesamt = measureIR(); // Hier fehlt die Funktion FrontIR() die statt den Boden IR Sensoren die Front Sensoren auswertet. Testweise wird mit nach vorne umgesteckten Bodensensoren gearbeitet.

  // Nur IR der Umgebung
  digitalWrite(Licht,LOW);
  int Umwelt = measureIR(); // s.o.
  
  return (Gesamt - Umwelt); // IR Anteil der LEDs  
}

// Analog zu measureIR(); für Frontsensoren zu implementieren
int FrontIR () {
  return 1014; // Testwert
}

void wand_suchen(){
  ausrichten(0);
  int Wand = measureSONAR();
  int Winkel;
  int sonar;
  for (int i=0; i <= 8; i++){ // muss reichen sonst dauerts ewig ....
    ausrichten(i*45);
    sonar = measureSONAR();
    if (sonar < Wand) {
      Wand = sonar;
      Winkel = i;
    }  
  }
  ausrichten(Winkel); // Wand gefunden und darauf ausgerichtet

  if (measureSONAR() > 10) {
  motorVor(); /// besser machen!!!
  }
  while (measureSONAR() > 10)
    delay(200);
  motorStop();
}

void wand_ausrichten(){
  int drehwinkel = 25;
  int alt = richtung();
  int mitte=measureSONAR();
  ausrichten(alt-drehwinkel);
  int rechts=measureSONAR();
  ausrichten(alt+drehwinkel); 
  int links=measureSONAR();
  float neu;
  if (links+rechts / 2 == mitte) {
  while (diff(rechts,links)> 0) {
    ausrichten(alt+drehwinkel); 
    Serial.print(alt+drehwinkel);
    Serial.println("grad"); 
    links = measureSONAR();
    Serial.print(links);
    Serial.println("links");
    ausrichten(alt-drehwinkel);
    Serial.print(alt-drehwinkel);
    Serial.println("grad");
    rechts = measureSONAR();
    Serial.print(rechts);
    Serial.println("rechts");
    neu = (((float)links / (float)rechts) * drehwinkel * 2 - 90);
    Serial.print(neu);
    Serial.println("neu");
    Serial.print(alt);
    Serial.println("alt");
    ausrichten(alt+neu);
    Serial.print(alt+neu);
    Serial.println("neugrad");
  }
  } else
    wand_suchen();
}


void muster_1_demo(){
  delay(MEASURE_DELAY);
  long distanceSONAR = measureSONAR();
  long distanceIR = 1; //measureIR(); 

  // richtung(); 
 
  if ((distanceSONAR > 10.0) and (distanceIR < 10)) {
  motorVor();
  Serial.print(distanceSONAR);
  Serial.println(" cm (Sonar) weiter ...");
  } else {
  Serial.print(distanceSONAR);
  Serial.println(" cm (Sonar) Achtung! ausweichen!");
  
  if (flip) {
    if (distanceIR > 10) {
      motorRetour();
      delay(3500);      
    }  
    rechtsausweichen();
    flip = false;  
  //  richtung();    
  } else {
    if (distanceIR > 10) {
      motorRetour();
      delay(3500);    
    }  
    linksausweichen();
    flip = true;
 //   richtung();
  }  
  // testmanöver
 // ausrichten(180);
  }
}

void bahnwechselzurueck(){
  //90 Grad drehen 
  //nächste Spur anfahren (Rumpfbreite vor)
  //weiter 90 Grad -> versetzt zurück und dann das nächste mal anders rum lg anna :)
}

ISR(PCINT2_vect) {
  char result = r.process();
  char result2 = r2.process();
  if (result) {
     result == DIR_CW ? rotpos1++ : rotpos1--;
  }
  if (result2) {
     result2 == DIR_CW ? rotpos2++ : rotpos2--;
  }
}


void loop()
{

// send data only when you receive data:
   if (Serial.available() > 0) {
      // read the incoming byte:
      inByte = Serial.read();

      // say what you got:
      Serial.print("bestätige: ");
      Serial.println(inByte);

      switch (inByte) {
         case '0':
            drehen_demo();
            break;
         case '1':
            tisch_demo0(); // :)
            break;
         case '2':
            feature_demo0(); // :)
            break;
          case '3':
            demo_rot90();
            break;
          case '4':
            muster_1_demo();
            break;
          case 'W':
            motorVor();
            break;
          case 'A':
            motorLinks(Power);
            break;
          case 'D':
            motorRechts(Power);
            break;
          case 'S':
            motorRetour();
            break;
          case 'X':
            motorHalt();
            break;
          default:
            Serial.println("unbekanntes Kommando!");        
      }
   }


//Serial.println("Neue Testrunde");
//delay(10000);
//richtung();


//weitere verfügbare Demos: (einkommentieren zum aktivieren)

//wand_suchen();

//demox();

//demoY();

//Serial.println(boden_test());
 
//wand_ausrichten(); //buggy
}

void boden_demo(){

int test = measureIR();

Serial.println(test);
//delay(100);
 
}
