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
#include <string.h>
#include <array>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

//CHANGE defines !!
#define SPI_CS 1
#define SPI_MOSI 1
#define SPI_MISO 1
#define SPI_CLK 1

#define RREF 430 //PT100 uses 430Ohm on the PCB 

using namespace std;

//timer variables 
 unsigned long lastTime = 0;
 unsigned long timerDelay = 10000;

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
AsyncEventSource event("/event");

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

// Check and print any faults
String dispFault(){

  uint8_t fault = MAX4Wire.readFault();
  if (fault) {
    Serial.print("Fault 0x"); 
    Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      return("RTD High Threshold");
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      return("RTD Low Threshold"); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      return("REFIN- > 0.85 x Bias"); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      return("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      return("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      return("Under/Over voltage"); 
    }
    MAX4Wire.clearFault();
  }

}

//get sensor reading for JSON
String getSensorReadingsJSON (){
    uint16_t valueRTD = MAX4Wire.readRTD();
    uint16_t valueTemp = MAX4Wire.temperature(100,RREF);
    reading["PT100 - Temperature"] = String(valueRTD);
    reading["PT100 - Raw RTD Value"] = String(valueTemp);
    String JSONString = JSON.stringify(reading);
    return JSONString;
}

//get the values for the OLED 
array<uint16_t,2> getSensorReadingOLED(){
    uint16_t valueRTD = MAX4Wire.readRTD();
    uint16_t valueTemp = MAX4Wire.temperature(100,RREF);
    return{valueRTD, valueTemp};
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

  // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/index.html", "text/html");
    });

    server.serveStatic("/", LittleFS, "/");

    // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    lastTime = lastTime + timerDelay;
    request->send(200, "text/plain", "OK!");
  });

  event.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&event);

  // Start server
  server.begin();

}


void loop() {
    Serial.print("Test for Monitor");
    
    //clear any HW faults and checks for new ones 
    MAX4Wire.clearFault();
    String faultToDisp = dispFault();

    //get temp & RTD value
    array<uint16_t,2> tempAndRTD = getSensorReadingOLED();

    OLED.setCursor(0,0);
    OLED.println("Temperatur: \n" + String(tempAndRTD[0]));
    OLED.setCursor(0,20);
    OLED.println("RTD Raw: \n" + String(tempAndRTD[1]));
    OLED.setCursor(0,40);
    OLED.println("Hardware fault: \n" + faultToDisp);
    OLED.display();
   
    if ((millis() - lastTime) > timerDelay) {
        // Send Events to the client with the Sensor Readings Every 10 seconds
        event.send("ping",NULL,millis());
        event.send(getSensorReadingsJSON().c_str(),"new_readings" ,millis());
        lastTime = millis();
      } 
  
    delay(2000);
}


