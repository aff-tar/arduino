#include "DHT.h"
#include <LiquidCrystal.h>
#include <SFE_BMP180.h>
#include <Wire.h>

#define       ALTITUDE                50.0
#define       DHTPIN                  22
#define       DHTTYPE                 DHT22
#define       DSM501A_INPUT_PIN_25    30
#define       SAMPLETIME_MS           10000

unsigned long duration = 0;
unsigned long starttime = 0;
unsigned long endtime = 0;
unsigned long sample_period = 0;
unsigned long lowpulseoccupancy = 0;
unsigned long concentration = 0;
unsigned long count = 0;
double        df_temperature = 0;
double        df_pressure = 0;
double        df_humidity = 0;
float         f_ratio = 0;
float         f_concentration = 0;
float         f_ppmv = 0;
char          s_temperature[8];
char          s_humidity[8];
char          s_pressure[8];
char          s_ppmv[16];
char          s_ratio[8];

SFE_BMP180    Pressure;
DHT           Dht(DHTPIN, DHTTYPE);
LiquidCrystal Lcd(8, 9, 4, 5, 6, 7);

void setup(void) {
  Lcd.begin(16, 2);
  Lcd.print("Reading sensors");
  Dht.begin();
  Pressure.begin();
  pinMode(DSM501A_INPUT_PIN_25, INPUT);
  starttime = millis();
  //Lcd.clear();
}

void loop() {

  df_humidity = (double)(Dht.readHumidity());
  BM180_readPressureAndTemperature();
  DSM501A_getPM(DSM501A_INPUT_PIN_25, (float)df_temperature);

  dtostrf(df_temperature, 5, 2, s_temperature);
  dtostrf(df_humidity, 4, 2, s_humidity);
  dtostrf(df_pressure, 5, 2, s_pressure);
  dtostrf(f_ppmv, 5, 1, s_ppmv);
  //dtostrf(f_ratio, 8, 7, s_ratio);

  Lcd.clear();
  //Printing Temperature
  Lcd.home();
  //Lcd.print("T:");
  Lcd.print(s_temperature);
  Lcd.print((char)223);
  Lcd.print("C ");

  //Printing PM2.5 lowpulseoccupancy
  //Lcd.print(sample_period);
  //Lcd.print(" ");
  //Lcd.print(lowpulseoccupancy);
  //Lcd.print(" ");
  //Lcd.print(count);
  //Lcd.print(" ");
  //Lcd.print(s_ratio);
  //Lcd.print(" ");
  //Lcd.setCursor(0, 1);
  //Lcd.print(concentration);
  //Lcd.print(" ");
  Lcd.print(s_ppmv);
  Lcd.print("p");

  //Printing Pressure
  Lcd.setCursor(0, 1);
  //Lcd.print("P:");
  Lcd.print(s_pressure);
  Lcd.print("hPa ");

  //Printing Humidity
  //Lcd.print("H:");
  Lcd.print(s_humidity);
  Lcd.print("%");

  //delay(1000);
}

void BM180_readPressureAndTemperature()
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
  status = Pressure.startPressure(3);
  if (0 == status)
    goto BM180_error;

  delay(status);
  status = Pressure.getPressure(df_pressure, df_temperature);
  if (0 == status)
    goto BM180_error;

  return;
  
BM180_error:
  //Lcd.setCursor(0, 1);
  //Lcd.print("BM180 error:");
  status = Pressure.getError();
  //Lcd.print(status);
}

float DSM501A_getPM(int pin, float f_temp) {

  starttime = millis();
  count = 0;
  lowpulseoccupancy = 0;

  while (1) {
    count++;
    lowpulseoccupancy += pulseIn(pin, LOW);
    sample_period = millis() - starttime;

    if (sample_period > SAMPLETIME_MS)
    {
      if (0 == lowpulseoccupancy)
        return (f_ppmv);
        
      f_ratio = ((float)(lowpulseoccupancy/10))/((float)sample_period);//; // In %
      concentration = 1.1 * pow(f_ratio, 3) - 3.8 * pow(f_ratio, 2) + 520 * f_ratio + 0.62; // using spec sheet curve

      //ppmv=mg/m3 * (0.08205*Tmp)/Molecular_mass
      //0.08205   = Universal gas constant in atm·m3/(kmol·K)
      f_ppmv = (float)(((concentration * 0.0283168) / 100) *  ((0.08205 * f_temp) / 0.01));
      return (f_ppmv);
    }
  }
}

