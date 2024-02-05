#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include<math.h> 


#define NEO7M_RX 10
#define NEO7M_TX 11
#define GPS_BAUD 9600

File excelFile;
File myFile;
SoftwareSerial neoSerial(NEO7M_RX, NEO7M_TX);
TinyGPSPlus gps;
float enlem, boylam;
double roll,pitch,yaw;
float latitude, longitude;
const int analogIn = A3; 
int mVperAmp = 25; 
int RawValue = 0; 
float ACSoffset = 2540; 
double Voltage = 0; 
double Amps = 0; 
float toplam; 
float v;
double sicaklik;
//int i=0; 
float vA = 0.0; 
float vB = 0.0; 
float vGiris = 0.0; 
float vOrtalama = 0.0;
float RA = 119.1; 
float RB = 9.97; 
void setup() {


  Serial.begin(9600);
  neoSerial.begin(GPS_BAUD); //GPS Modülü başlat

   if (!SD.begin(53)) /*CS Pini Arduino Megada 53. Pine bağlandı */{
    Serial.println("Hata: SD kart başlatılamadı!");
    return;
  }
  Serial.println("SD kart başarıyla başlatıldı.");

  //Text dosyasının oluşturulması
  /*myFile = SD.open("gps_data.txt", FILE_WRITE);
  // Excel dosyasının oluşturulması
  
  excelFile = SD.open("veri.xlsx", FILE_WRITE);
  if (!excelFile) {
    Serial.println("Excel dosyası oluşturulamadı!");
    return;
  }
  
  // Excel dosyasına başlık satırının yazılması
  excelFile.println("Voltaj,Amper,Sicaklik,X,Y,Z,Enlem,Boylam");
  excelFile.close();*/
  
}

void loop() {
  
  guc_devre();
  sensor_filter();
  //gps_read();
  /*gps.f_get_position(&enlem, &boylam);
  Serial.println(enlem,boylam);*/

}


