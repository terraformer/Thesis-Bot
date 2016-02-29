// Based on
// MPU-6050 Short Example Sketch
// by Arduino User JohnChi
// August 17, 2014
// Public Domain
    
// and inspired vby Kristian Lauszus, TKJ Electronics 2012 Code released under the GNU General Public License 2.0
// https://github.com/TKJElectronics/Example-Sketch-for-IMU-including-Kalman-filter/blob/master/IMU/MPU6050/MPU6050.ino 
    
void print_mpu6050_raw(){
   Wire.beginTransmission(MPU_addr);
   Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
   Wire.endTransmission(false);
   Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
   AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
   AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
   AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
   Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
   GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
   GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
   GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
   Serial.print("AcX = "); Serial.print(AcX);
   Serial.print(" | AcY = "); Serial.print(AcY);
   Serial.print(" | AcZ = "); Serial.print(AcZ);
   Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
   Serial.print(" | GyX = "); Serial.print(GyX);
   Serial.print(" | GyY = "); Serial.print(GyY);
   Serial.print(" | GyZ = "); Serial.println(GyZ);
 }

void print_mpu6050() {
   double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
   timer = micros();
   
   Wire.beginTransmission(MPU_addr);
   Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
   Wire.endTransmission(false);
   Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
   AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
   AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
   AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
   Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
   GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
   GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
   GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
   Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
      
   roll =  (atan2(AcY,AcZ)+PI)*RAD_TO_DEG;
   pitch =  (atan2(AcX,AcZ)+PI)*RAD_TO_DEG;
   
   double gyroXrate = GyX / 131.0; // Convert to deg/s
   double gyroYrate = GyY / 131.0; // Convert to deg/s
   
   pitch = 0.10 *(pitch+gyroXrate*dt) + 0.9*pitch;
   roll = 0.10 *(roll+gyroYrate*dt) + 0.9*roll;
   
   Serial.print(" | pitch = "); 
   Serial.print((int)pitch-180);
   Serial.print(" | roll = "); 
   Serial.println((int)roll-180);
}
