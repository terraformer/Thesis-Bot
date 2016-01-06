/* 
Copyright 2016 Christian Liebl. 

Licenced under the GNU GPL Version 3.

Contact: christian.liebl@live.de

Basisfunktionen
*/

int singleIR() { // Einfache Boden IR Messung
  int dist_rechts = analogRead(IRRechts);
  int dist_links = analogRead(IRLinks);
  
  if (dist_rechts < dist_links)
    return (dist_rechts);
  else
    return(dist_links);
}

long singleMeasurementSONAR() // einfache Frontsonar Messung
{
  long duration = 0;
  // Measure: Put up Trigger...
  digitalWrite(TRIGGER_PIN, HIGH);
  // ... wait for 11 µs ...
  delayMicroseconds(11);
  // ... put the trigger down ...
  digitalWrite(TRIGGER_PIN, LOW);
  // ... and wait for the echo ...
  duration = pulseIn(ECHO_PIN, HIGH);
  return (long) (((float) duration / USONIC_DIV));
} 


long measureIR() // gemittelte Bodensensor Messung
{
  double measureSum = 0.0;
  for (int i = 0; i < MEASURE_SAMPLESir; i++)
  {
    delay(MEASURE_SAMPLE_DELAYir);
    measureSum += singleIR();
  }
  return measureSum / MEASURE_SAMPLESir;
}

long measureSONAR() // gemittelte Front Sonar Messung
{
  long measureSum = 0;
  for (int i = 0; i < MEASURE_SAMPLES; i++)
  {
    delay(MEASURE_SAMPLE_DELAY);
    measureSum += singleMeasurementSONAR();
  }
  return measureSum / MEASURE_SAMPLES;
}

boolean boden_test(){ //Auswertung Bodensensor
   int test = measureIR();
   if (test > 60) // Referenz messen statt hardcoode!!! TODO!!!
     return 1; // Boden vorhanden
   else
     return 0; // kein Boden!
}

int diff(int a,int b) { // Ganzzahldifferenz ermitteln
  return (abs(abs(a)-abs(b)));
}

void waitUpTo(unsigned long ms) { // Hilfsfunktion für Vorschub auf Timing Basis (inspiriert durch Quelle: http://forum.arduino.cc/index.php?topic=252523.0)
  unsigned long goal  = millis() + ms;
  while((millis() < goal) and boden_test()); // Solange bis X und Boden unter den Rädern
}

void mag_setup () {
  while (!compass.begin())
  {
    delay(500);
      Serial.println("no Mag");
  }
      Serial.println("Mag init");
  

  // Messbereich setzen
  compass.setRange(HMC5883L_RANGE_1_3GA);

  // Betriebsmodus setzen
  compass.setMeasurementMode(HMC5883L_CONTINOUS);

  // Datenrate festlegen
  compass.setDataRate(HMC5883L_DATARATE_15HZ);

  // Anzahl Rohmessungen für Mittelwert Bestimmung
  compass.setSamples(HMC5883L_SAMPLES_8);
}


// Correct angle
float correctAngle(float heading)
{
  if (heading < 0) { heading += 2 * PI; }
  if (heading > 2 * PI) { heading -= 2 * PI; }

  return heading;
}

int richtung() { // Richtung mittels Magnetometer Bestimmen. Dabei das Magnetometer fortlaufend kalibrieren.
  Vector mag = compass.readRaw();

  // Determine Min / Max values
  if (mag.XAxis < minX) minX = mag.XAxis;
  if (mag.XAxis > maxX) maxX = mag.XAxis;
  if (mag.YAxis < minY) minY = (mag.YAxis + minY) / 2;
  if (mag.YAxis > maxY) maxY = (mag.YAxis + maxY) / 2;

  // Calculate offsets
  offX = (maxX + minX)/2;
  offY = (maxY + minY)/2;
  
  float X = mag.XAxis - offX;
  float Y = mag.YAxis - offY;

  // Ausrichtung ermitteln   
  int heading = int(correctAngle(atan2(Y, X)) * 180/M_PI);
  return heading;  
}


void motorHalt(){ // Motor Bremsen und dann stromlos
  digitalWrite(B1A,HIGH);
  digitalWrite(B1B,HIGH);
  digitalWrite(A1A,HIGH);
  digitalWrite(A1B,HIGH);
  delay(100);
  digitalWrite(B1A,LOW);
  digitalWrite(B1B,LOW);
  digitalWrite(A1A,LOW);
  digitalWrite(A1B,LOW);
}

void motorStop(){ // Motor stromlos
  digitalWrite(B1A,LOW);
  digitalWrite(B1B,LOW);
  digitalWrite(A1A,LOW);
  digitalWrite(A1B,LOW);
}

void motorRetour(){ // Beide Motoren retour
  digitalWrite(B1A,HIGH);   
  digitalWrite(B1B,LOW);
  digitalWrite(A1A,HIGH);   
  digitalWrite(A1B,LOW);
}

void motorVor(){ // Beide Motoren vor
  if (boden_test()) { // Sicher ist sicher! Prüfen ob Boden unter den Rädern
  digitalWrite(B1A,LOW);   
  digitalWrite(B1B,HIGH);
  digitalWrite(A1A,LOW);   
  digitalWrite(A1B,HIGH);
  }
}

void motorLinks(byte P){ // P für PWM Puls der die Motorleistung steuert.
  if (boden_test()) { // Sicher ist sicher! Prüfen ob Boden unter den Rädern
  analogWrite(B1A,P);   
  digitalWrite(B1B,LOW);
  digitalWrite(A1A,LOW);   
  analogWrite(A1B,P);
  }  else {
      motorRetour();
      delay(50);
      motorStop();
  }
}  

void motorRechts(byte P){
  if (boden_test()) {
  digitalWrite(B1A,LOW);   
  analogWrite(B1B,P);
  analogWrite(A1A,P);   
  digitalWrite(A1B,LOW);
//  delay(100);  
  } else {
      motorRetour();
      delay(50);
      motorStop();
  }
}  

void motorLinks90(){
  long alt = richtung();
  long neu = alt - 90;
  if (neu < 0)
    neu = neu + 360 ;
    
  digitalWrite(B1A,HIGH);   
  digitalWrite(B1B,LOW);
  digitalWrite(A1A,LOW);   
  digitalWrite(A1B,HIGH);

  while ((abs(abs(richtung())-abs(neu)) > 3) and (abs(abs(richtung())-abs(neu))< 357)) {

  delay(100); 
 } 
 motorStop(); 
}  

void motorRechts90(){
  long alt = richtung();
  Serial.print(alt);
  Serial.println("rechts alt grad");
  long neu = alt + 90;
  if (neu > 360)
    neu = neu - 360;
  digitalWrite(B1A,LOW);   
  digitalWrite(B1B,HIGH);
  digitalWrite(A1A,HIGH);   
  digitalWrite(A1B,LOW);

  while ((abs(abs(richtung())-abs(neu)) > 3) and (abs(abs(richtung())-abs(neu))< 357)) {
  delay(100); 
 } 
 motorStop();  
} 

void rotausrichten(int tick) { // Ausrichten Mittels Rotationsencoder
  int alt = rotpos1;
  int neu = alt + tick;
  while (rotpos1 != neu) {
    if (tick < 0) {
      motorRechts(200);
    } else {
      motorLinks(200);
    }
  }
  motorHalt();
}

int fit360 (int ergo) { // winkel anpassen Trimmen auf 0 - 360 Grad
     if (ergo > 359) 
        return ergo - 360;
     else if (ergo < 0)
        return ergo + 360;
     else
        return ergo;
} 


void ausrichten(int neu){ //Ausrichten mitteln Magnetometer //zuckelt manchmal rum ... :( Masseträgheit ... hm MPU 6050 doch einbauen? klar TODO: Implementieren! 
  int alt = richtung();
  int akt = alt;
  neu = fit360(neu); 

    while (akt != neu) { //Problem wenn nicht alle Winkelgrade vom Kompass aufgelöst werden sollten!?
      akt = richtung();
      int diff = fit360(neu - akt);
      byte P = 255;  
    if (diff < 15)
       P = byte(((175.0 / 180.0) * diff) + 80); //maxwert PWM zugabe / maxdrehwinkel * betrags diff zu aktueller winkel + basisPWM // integer 
    else if (diff > 345)
       P = byte(((175.0 / 180.0) * (355-diff)) + 80); //255+basispwm nö
     
    if (diff > 180 )
    {   
      motorRechts(P);     
    } else {
      motorLinks(P);
    }

   }
   motorHalt();
} 

int rotcalib() { // Rotationsencoder mittels Magnetometerwerten kalibrieren
   int viertel = rotpos1 = rotpos2 = 0;

   ausrichten(90);
   delay(3000);
   viertel = abs(rotpos1) + abs(rotpos2);

   ausrichten(180);
   delay(3000);
   viertel += abs(rotpos1) + abs(rotpos2);

   ausrichten(270);
   delay(3000);
   viertel += abs(rotpos1) + abs(rotpos2);

   ausrichten(0);
   delay(3000);
   viertel += abs(rotpos1) + abs(rotpos2);

   return viertel = viertel / 8; // Anzahl der Rotationsimpulse für eine 90 Grad Drehung
}

void rechtsausweichen(){ //Spur und Richtungswechsel rechtsrum
  ausrichten(Ursprung+90);
  motorVor();
  waitUpTo(1500); // Vorschub genauer festlegen. TODO!!!
  ausrichten(Ursprung);
  motorVor();  
}

void linksausweichen(){ //Spur und Richtungswechsel linksrum
  ausrichten(Ursprung+90);
  motorVor();
  waitUpTo(1500); // Vorschub genauer festlegen. TODO!!!
  ausrichten(Ursprung+180);
  motorVor();  
}


void Orientieren() {
  Serial.println("Magnetometer Kalibrieren in 10 Sekunden");
  delay(10000);
  for (int i=0; i < 5; i++){
      motorLinks90();
      Serial.print(i*12.5);
      Serial.println("%");
  }  

  for (int i=5; i < 9; i++){
      motorRechts90();
      Serial.print(i*12.5);
      Serial.println("%");
  }  

  Serial.print(offX);
  Serial.print(" offX; ");

  Serial.print(offY);
  Serial.println(" offY.");
}

