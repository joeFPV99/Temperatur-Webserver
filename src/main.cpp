#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define DHTPIN 5 //TBD!
#define DHTTYPE DHT11

//connstruct DHT obj
DHT dht(DHTPIN,DHTTYPE);

//construct Adafruit OLED obj
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void setup() {

Serial.begin(9600);
while (!Serial);

// initialize oled
OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
OLED.clearDisplay();

// initialize DHT11
dht.begin();

}

void loop() {
 
   float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Fehler beim Lesen der Temperatur!");
  }
  else {
    Serial.print("Temperatur: ");
    Serial.println(t);
  }
  // Hole den Feuchtigkeitswert des DHT11
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Fehler beim Lesen der Feuchtigkeit!");
  } 
  else {
    Serial.print("Feuchtigkeit: ");
    Serial.println(h);
  }
  // Gebe die Sensordaten auf dem Display aus
  OLED.clearDisplay();
  OLED.setTextSize(1);
  OLED.setTextColor(SSD1306_WHITE);
  OLED.setCursor(0,0);
  OLED.println("Temperatur: ");
  OLED.setCursor(0,8);
  OLED.println(t);
  OLED.setCursor(0,16);
  OLED.println("Humidity: ");
  OLED.setCursor(0,24);
  OLED.println(h);
  OLED.display();
  delay(2000);

}


