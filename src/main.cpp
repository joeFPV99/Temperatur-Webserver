#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <Hash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DNSServer.h>
#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <ESP8266WiFi.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define DHT1PIN 6 
#define DHT2PIN 7 
#define DHT3PIN 16 
#define DHTTYPE DHT11

//connstruct DHT objs
DHT dht1(DHT1PIN,DHTTYPE);
DHT dht2(DHT2PIN,DHTTYPE);
DHT dht3(DHT3PIN,DHTTYPE);

//construct Adafruit OLED obj
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {

    Serial.begin(9600);
    while (!Serial);

    // initialize oled
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    OLED.clearDisplay();

    // initialize DHT11
    dht1.begin();
    dht2.begin();
    dht3.begin();

}

void loop() {
 
    float t = dht1.readTemperature();
    if (isnan(t)) {
        Serial.println("Fehler beim Lesen der Temperatur!");
    }
    else {
        Serial.print("Temperatur: ");
        Serial.println(t);
    }
    // Hole den Feuchtigkeitswert des DHT11
    float h = dht1.readHumidity();
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


