#include <Arduino.h>
#include <Wire.h>
#include <Hash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DNSServer.h>
#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <ESP8266WiFi.h>
#include "Adafruit_MAX31865.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

//construct Adafruit OLED obj
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {

    Serial.begin(74880);
    while (!Serial)
    {
    }
    
    // initialize oled
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    OLED.clearDisplay();
}

void loop() {
 

    // Gebe die Sensordaten auf dem Display aus
    OLED.clearDisplay();
    OLED.setTextSize(1);
    OLED.setTextColor(SSD1306_WHITE);
    OLED.setCursor(0,0);
    OLED.println("Temperatur: ");
    OLED.display();
    delay(2000);

}


