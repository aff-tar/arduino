#include "DHT.h"
#include <LiquidCrystal.h>
#include <SFE_BMP180.h>
#include <Wire.h>

#define     ALTITUDE      50.0
#define     DHTPIN        23
#define     DHTTYPE       DHT22

float       f_humidity;
float       f_pressure;
float       f_temperature;
double      df_temperature;
double      df_pressure;
double      df_humidity;
char        s_temperature[6];
char        s_humidity[6];
char        s_pressure[7];

SFE_BMP180    Pressure;
DHT           Dht(DHTPIN, DHTTYPE);
LiquidCrystal Lcd(8, 9, 4, 5, 6, 7);

void setup(void) {
  Lcd.begin(16, 2);
  Lcd.print("Reading sensors");
  Dht.begin();
  Pressure.begin();
  Lcd.clear();
}

void loop() {

  df_humidity = (double)(Dht.readHumidity());
  readPressureAndTemperature();

  dtostrf(df_temperature, 4, 2, s_temperature);
  dtostrf(df_humidity, 2, 2, s_humidity);
  dtostrf(df_pressure, 4, 2, s_pressure);

  //Printing Temperature
  Lcd.home();
  //Lcd.print("T:");
  Lcd.print(s_temperature);
  Lcd.print((char)223);
  Lcd.print("C ");

  //Printing Humidity
  Lcd.setCursor(10, 0);
  //Lcd.print("H:");
  Lcd.print(s_humidity);
  Lcd.print("%");

  //Printing Pressure
  Lcd.setCursor(4, 1);
  //Lcd.print("P:");
  Lcd.print(s_pressure);
  Lcd.print("hPa");

  delay(1000);
}

void readPressureAndTemperature()
{
  volatile char status;

  status = Pressure.startTemperature();
  if (0 == status)
    goto BM180_error;
    
  delay(status);
  status = Pressure.getTemperature(df_temperature);
  if (0 == status)
    goto BM180_error;
    
  delay(status);
  f_temperature = (float)df_temperature;
  status = Pressure.startPressure(3);
  if (0 == status)
    goto BM180_error;
    
  delay(status);
  status = Pressure.getPressure(df_pressure, df_temperature);
  if (0 == status)
    goto BM180_error;
  //p0 = Pressure.sealevel(df_pressure, ALTITUDE);
  f_pressure = (float)df_pressure;
  
BM180_error:
  //Lcd.setCursor(0, 1);
  //Lcd.print("BM180 error:");
  status = Pressure.getError();
  //Lcd.print(status);
}

