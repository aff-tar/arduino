#include "DHT.h"
#include <LiquidCrystal.h>


#define DHTPIN 22     // what pin we're connected to

#define DHTTYPE DHT22   

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(8,9,4,5,6,7); 

void setup(void) {
  lcd.begin(16, 2);
  lcd.print("Reading sensor");
  dht.begin();
  lcd.clear();
}

void loop() {
  
  float temperature, humidity;

  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

 //lcd.clear();

 char tempF[6]; 
 char humF[6];
 dtostrf(temperature, 5, 1, tempF);
 dtostrf(humidity, 2, 0, humF);

 lcd.home();
 lcd.print("T:"); 
 lcd.print(tempF);
 lcd.print((char)223);
 lcd.print("C ");
 lcd.print("H: ");
 lcd.print(humF);
 lcd.print("%");
 
 //delay(1000); 
}
