//SD card reader
#include <SD.h>
File myFile;

//BMP temperature sensor
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
Adafruit_BMP3XX bmp;

//Thermistor
int ThermistorPin = 0;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void setup() {

  //SD card reader
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
  if (!SD.begin(10)) {
  Serial.println("initialization failed!");
  return;
  }
  Serial.println("initialization done.");
  
  //BMP temperature sensor
  
  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

}

void loop() {

  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;

  Serial.println(bmp.temperature);
  myFile = SD.open("HAB_data.txt", FILE_WRITE);
  myFile.print(Tc);
  myFile.print(", ");
  myFile.print(bmp.temperature);
  myFile.print(", ");
  myFile.print(bmp.pressure / 100);
  myFile.close();
  Serial.println("Data logged.");
  
  delay(2000);

}
