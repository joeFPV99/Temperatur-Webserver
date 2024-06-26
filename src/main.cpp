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
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
 
}

void loop() {
 

}


