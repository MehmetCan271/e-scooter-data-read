
void guc_devre() {
  float vToplam = 0.0; 
  int i=0;
for (i=0;i<20;i++) 
{ 
RawValue = analogRead(analogIn); 
Voltage = (RawValue / 1024.0)*5000; 
Amps = ((Voltage - ACSoffset) / mVperAmp); 
float okunanA = analogRead(A4); 
vA = okunanA * 5 / 1024; 
vA = vA *((RA + RB )/9.97); 
vToplam = vToplam + vA; 
toplam = (toplam + Amps); 
} 
Amps = toplam / 20; 
toplam = 0; 
v = vToplam / 20; 
int deger = analogRead(A2);
sicaklik = Termistor(deger); 
/*Serial.print("Voltaj (V):"); 
Serial.print(v); 
Serial.print(" , Amps = "); 
Serial.print(Amps, 3); 
Serial.print(" , "); 
Serial.println(sicaklik); 
sdkart = SD.open("data.txt", FILE_WRITE); 
if (sdkart) { 
sdkart.print(float(v)); 
sdkart.print(","); 
sdkart.print(float(Amps)); 
sdkart.print(","); 
sdkart.println(float(sicaklik)); 
sdkart.close (); 
} 
else { 
Serial.println("data.txt acilamadi"); 
} 
delay(1000); */

}
double Termistor(int analogOkuma) { 
double sicaklik; 
sicaklik = log(((10240000 / analogOkuma) - 10000)); 
sicaklik = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * sicaklik * sicaklik)) * sicaklik); 
sicaklik = sicaklik - 273.15; 
return sicaklik; 
} 
