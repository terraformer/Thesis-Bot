
#ifndef FUN_H
#define FUN_H

/* 
Copyright 2016 Christian Liebl. 

Licenced under the GNU GPL Version 3.

Contact: christian.liebl@live.de

Basisfunktionen
*/

int singleIR() ;
long singleMeasurementSONAR();
long measureIR() ;
long measureSONAR() ;
boolean boden_test();
boolean front_test() ;
boolean safe() ;
void beep(int f) ;
int diff(int a,int b) ;
void waitUpTo(unsigned long ms) ;
void moveUpTo(int x) ;
void mag_setup () ;
float correctAngle(float heading) ;
int richtung() ;
void motorHalt();
void motorStop();
void motorRetour();
void motorVor();
void motorVor(byte PL, byte PR) ;
void motorLinks(byte P);
void motorRechts(byte P);
void motorLinks90();
void motorRechts90();
void rotausrichten(int tick);
int fit360 (int ergo) ;
void ausrichten(int neu);
int viertel();
int rotcalib() ;
void rechtsausweichen();
void linksausweichen();
void Orientieren();
int getoption(String com);

#endif
