#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_Simple_AHRS.h>
#include <SD.h>
#include <SoftwareSerial.h>

Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(1000);
Adafruit_Simple_AHRS ahrs(&lsm.getAccel(), &lsm.getMag()); 
const int chipSelect = 12;
unsigned long temps;
String dades;
unsigned long i;
File myFile;
int led = 7;

void configureSensor(void){
    lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
    lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
    lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
}
void setup(void){
    Serial.begin(9600);
    if(!lsm.begin()){
        Serial.print(F("Ooops, no LSM9DS0 detected ..."));
    }
    configureSensor();
    Serial.print(F("\nInitializing SD card..."));
    pinMode (SS, OUTPUT);
    if (!SD.begin(chipSelect)) {
        Serial.println(F("Card failed, or not present"));
        while (1);
    }
    Serial.println(F("card initialized."));      
    dades = String(); 
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
}

void loop(void){ 
    sensors_event_t accel, mag, gyro, temp;
    myFile = SD.open("test.txt", FILE_WRITE);
    i=0;
    while (i<700) {
      lsm.getEvent(&accel, &mag, &gyro, &temp);
      temps = millis();
       if (myFile) {
              dades = String(temps) + ';' + String(accel.acceleration.x) 
              + ';' + String(accel.acceleration.y) + ';' + String(accel.acceleration.z) 
              + ';' + String(mag.magnetic.x) + ';' + String(mag.magnetic.y) 
              + ';' + String(mag.magnetic.z) + ';' + String(gyro.gyro.x) 
              + ';' + String(gyro.gyro.y) + ';' + String(gyro.gyro.z);
              myFile.println(dades);
           }
       i++; 
       }
    myFile.close();
    digitalWrite(led, HIGH);
    while (1);
}
