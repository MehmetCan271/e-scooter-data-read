
#include <Wire.h>
#include <Kalman.h> 

#define RESTRICT_PITCH 

Kalman kalmanX; 
Kalman kalmanY;
Kalman kalmanZ;


/* IMU Data */
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int16_t tempRaw;

double gyroXangle, gyroYangle,gyroZangle; // Angle calculate using the gyro only
double compAngleX, compAngleY,compAngleZ; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY,kalAngleZ; // Calculated angle using a Kalman filter

int i=0;
uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data

// TODO: Make calibration routine

void baslat() {

  Wire.begin();
#if ARDUINO >= 157
  Wire.setClock(400000UL); // Set I2C frequency to 400kHz
#else
  TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
#endif

  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
  while (i2cWrite(0x19, i2cData, 4, false)); // Write to all four registers at once
  while (i2cWrite(0x6B, 0x01, true)); // PLL with X axis gyroscope reference and disable sleep mode

  while (i2cRead(0x75, i2cData, 1));
  if (i2cData[0] != 0x68) { // Read "WHO_AM_I" register
    Serial.print(F("Error reading sensor"));
    while (1);
  }

  delay(100); // Wait for sensor to stabilize

  /* Set kalman and gyro starting angle */
  while (i2cRead(0x3B, i2cData, 6));
  accX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
  accY = (int16_t)((i2cData[2] << 8) | i2cData[3]);
  accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);

#ifdef RESTRICT_PITCH // Eq. 25 and 26
  roll  = atan2(accY, accZ) * RAD_TO_DEG;
  pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  pitch = atan2(-accX, accZ) * RAD_TO_DEG;
#endif
yaw = atan2(sqrt(accX * accX + accY * accY), accZ) * RAD_TO_DEG;

  kalmanX.setAngle(roll); // Set starting angle
  kalmanY.setAngle(pitch);
  gyroXangle = roll;
  gyroYangle = pitch;
  compAngleX = roll;
  compAngleY = pitch;

  timer = micros();
}

void sensor_filter() {
  /* Update all the values */
  //smartdelay2(1000);
  //gps.f_get_position(&latitude, &longitude);
  while(i==0){
    baslat();
    i++;
  }
  
  while (i2cRead(0x3B, i2cData, 14));
  accX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
  accY = (int16_t)((i2cData[2] << 8) | i2cData[3]);
  accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);
  tempRaw = (int16_t)((i2cData[6] << 8) | i2cData[7]);
  gyroX = (int16_t)((i2cData[8] << 8) | i2cData[9]);
  gyroY = (int16_t)((i2cData[10] << 8) | i2cData[11]);
  gyroZ = (int16_t)((i2cData[12] << 8) | i2cData[13]);;

  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();

  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
  double roll  = atan2(accY, accZ) * RAD_TO_DEG;
  double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
#endif
yaw = atan2(sqrt(accX * accX + accY * accY), accZ) * RAD_TO_DEG;

  double gyroXrate = gyroX / 131.0; // Convert to deg/s
  double gyroYrate = gyroY / 131.0; // Convert to deg/s
  double gyroZrate = gyroZ / 131.0; // Convert to deg/s

#ifdef RESTRICT_PITCH
  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
  if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
    kalmanX.setAngle(roll);
    compAngleX = roll;
    kalAngleX = roll;
    gyroXangle = roll;
  } else
    kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

  if (abs(kalAngleX) > 90)
    gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
  kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);

  // Z-axis integration
  if (abs(kalAngleX) > 90 || abs(kalAngleY) > 90)
    gyroZrate = -gyroZrate; // Invert rate, so it fits the restricted accelerometer reading
  kalAngleZ = kalmanZ.getAngle(atan2(sqrt(accX * accX + accY * accY), accZ) * RAD_TO_DEG, gyroZrate, dt);
#else
  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
  if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
    kalmanY.setAngle(pitch);
    compAngleY = pitch;
    kalAngleY = pitch;
    gyroYangle = pitch;
  } else
    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter

  if (abs(kalAngleY) > 90)
    gyroXrate = -gyroXrate; // Invert rate, so it fits the restriced accelerometer reading
  kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

  // Z-axis integration
  if (abs(kalAngleX) > 90 || abs(kalAngleY) > 90)
    gyroZrate = -gyroZrate; // Invert rate, so it fits the restricted accelerometer reading
  kalAngleZ = kalmanZ.getAngle(atan2(sqrt(accX * accX + accY * accY), accZ) * RAD_TO_DEG, gyroZrate, dt);
#endif

  gyroXangle += gyroXrate * dt; // Calculate gyro angle without any filter
  gyroYangle += gyroYrate * dt;
  gyroZangle += gyroZrate * dt;
  //gyroXangle += kalmanX.getRate() * dt; // Calculate gyro angle using the unbiased rate
  //gyroYangle += kalmanY.getRate() * dt;

  compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
  compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;
  compAngleZ = 0.93 * (compAngleZ + gyroZrate * dt) + 0.07 * atan2(sqrt(accX * accX + accY * accY), accZ) * RAD_TO_DEG;

  // Reset the gyro angle when it has drifted too much
  if (gyroXangle < -180 || gyroXangle > 180)
    gyroXangle = kalAngleX;
  if (gyroYangle < -180 || gyroYangle > 180)
    gyroYangle = kalAngleY;
  if (gyroZangle < -180 || gyroZangle > 180)
    gyroZangle = kalAngleZ;

  /* Print Data */
#if 0 // Set to 1 to activate
  /*Serial.print(accX); Serial.print("\t");
  Serial.print(accY); Serial.print("\t");
  Serial.print(accZ); Serial.print("\t");

  Serial.print(gyroX); Serial.print("\t");
  Serial.print(gyroY); Serial.print("\t");
  Serial.print(gyroZ); Serial.print("\t");

  Serial.print("\t");*/
#endif
  
  //Serial.print(gyroXangle); Serial.print("\t");
  //Serial.print(compAngleX); Serial.print("\t");
  //Serial.print(kalAngleX); Serial.print("\t");

  //Serial.print("\t");
  //sicaklik = Termistor(deger); 
  while (neoSerial.available() > 0) {
     if (gps.encode(neoSerial.read())) {
        if (gps.location.isValid()) {
          Serial.println("*****GPS Başlatıldı*****");
          Serial.print("Voltaj (V):"); 
          //excelFile.print("Voltaj (V):");
          Serial.print(v); 
          //excelFile.print(v);
          Serial.print(" , Amps = "); 
          //excelFile.print( ", Amps = ");
          //myFile.print(Amps,3);
          Serial.print(Amps, 3); 
          //myFile.print(",");
          Serial.print(" , "); 
          //myFile.print(sicaklik);
          Serial.println(sicaklik); 
          //myFile.print("\t");
          Serial.print("\t");

          Serial.print(roll); Serial.print("\t");

          Serial.print(pitch); Serial.print("\t");

          Serial.print(yaw); Serial.print("\t");

          Serial.print("Enlem: ");
          Serial.println(gps.location.lat(), 6);
          Serial.print("Boylam: ");
          Serial.println(gps.location.lng(), 6);

          // Verilerin Excel dosyasına yazılması
          excelFile = SD.open("veri.txt", FILE_WRITE);
          if (excelFile) {
            excelFile.print(v);
            excelFile.print(",");
            excelFile.print(Amps,3);
            excelFile.print(",");
            excelFile.print(sicaklik);
            excelFile.print(",");
            excelFile.print(roll);
            excelFile.print(",");
            excelFile.print(pitch);
            excelFile.print(",");
            excelFile.print(yaw);
            excelFile.print(",");
            excelFile.print(gps.location.lat(), 6);
            excelFile.print(",");
            excelFile.println(gps.location.lng(), 6);

            excelFile.close();
            Serial.println("Veriler Excel dosyasına yazıldı.");
          } else {
            Serial.println("Veri dosyası açılamadı!");
          }

        }
     }
  }
  
  
  
  //myFile.print(roll);myFile.print("\t");;myFile.print(pitch);
  //myFile.print("Enlem:");
  //Serial.print("Enlem: ");
  //print_float2(latitude, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
  //myFile.print(", Boylam:");
 // Serial.print(", Boylam: ");
  //print_float2(longitude, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  //Serial.println();
 

  
  //Serial.print(gyroYangle); Serial.print("\t");
  //Serial.print(compAngleY); Serial.print("\t");
  //Serial.print(kalAngleY); Serial.print("\t");




#if 0 // Set to 1 to print the temperature
  Serial.print("\t");

  double temperature = (double)tempRaw / 340.0 + 36.53;
  Serial.print(temperature); Serial.print("\t");
#endif

  Serial.print("\r\n");
  //delay(1000);
}




