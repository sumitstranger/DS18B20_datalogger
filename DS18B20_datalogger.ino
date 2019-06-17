
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <OneWire.h>
//28 4F CE 79 97 6 3 EE
// Based on the OneWire library example
#include <DallasTemperature.h>
#include "index.h" //Our HTML webpage contents with javascripts
 #define ONE_WIRE_BUS 33

 
OneWire oneWire(ONE_WIRE_BUS);//SSID and Password of your WiFi router
const char* ssid = "Redmi 5"; //Your wifi Hotspot name
const char* password = "asdfghjkl"; //password of the wifi

DallasTemperature sensors(&oneWire);
DeviceAddress sensor1 = { 0x28, 0x4F, 0xCE, 0x79, 0x97, 0x6, 0x3, 0xEE }; //Address of sensor

 
WebServer server(80); //Server on port 80
 

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleADC() {
sensors.requestTemperatures();
   float x = sensors.getTempC(sensor1);
   String adcValue = String(x);
   Serial.println(x);
   server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

void setup(void){
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 sensors.begin();
  //Onboard LED port Direction output

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/readADC", handleADC); //This page is called by java Script AJAX
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();          //Handle client requests
}
