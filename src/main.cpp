#include <Arduino.h>
#include <Wire.h>
#include <Hash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <ESP8266WiFi.h>
#include "Adafruit_MAX31865.h"
#include <Arduino_JSON.h>
#include "LittleFS.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

//CHANGE defines !!
#define SPI_CS 1
#define SPI_MOSI 1
#define SPI_MISO 1
#define SPI_CLK 1

#define RREF 430 //PT100 uses 430Ohm on the PCB 

//construct Adafruit OLED obj
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//create JSON obj
JSONVar reading;

//wifi credentials 
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

//create Asyncweb obj on port 80
AsyncWebServer server(80);

//create event source on /event
AsyncEventSource event ("/event");

//create MAX31865 4 wire obj with software SPI: CS, MOSI, MISO, CLK
Adafruit_MAX31865 MAX4Wire = Adafruit_MAX31865(SPI_CS,SPI_MISO, SPI_MOSI, SPI_CLK); //TODO 

// Initialize LittleFS
void initLittleFS() {
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  else{
    Serial.println("LittleFS mounted successfully");
  }
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println("WIFI Conntected!");
}


//initialize OLED 
void initOLED () { 
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    OLED.clearDisplay();
    OLED.setTextSize(1);
    OLED.setTextColor(WHITE);
}




/*****************************************************************************************/


void setup() {

    Serial.begin(74880);
    while (!Serial)
    {
    }
    
    //initialize MAX31865 in 4 wire mode
    MAX4Wire.begin(MAX31865_4WIRE);

    //initialize WIFI, FileSystem & OLED screen 
    initWiFi();
    initLittleFS();
    initOLED();

}

void loop() {
    Serial.print("Test for Monitor");
    
    MAX4Wire.clearFault();
    uint16_t valueRTD = MAX4Wire.readRTD();
    MAX4Wire.temperature(100,RREF);

    OLED.setCursor(0,0);
    OLED.println("Temperatur: \n" + String(valueRTD));
    OLED.display();
    delay(2000);



  // Check and print any faults
  uint8_t fault = MAX4Wire.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold"); 
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold"); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias"); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage"); 
    }
    MAX4Wire.clearFault();
  }


}


