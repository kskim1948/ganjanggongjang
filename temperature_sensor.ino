
const char* host = "api.thingspeak.com";
String url = "/update?api_key=3GOJDFXCK0U4I6R3";  // Your Own Key here
const int httpPort = 80;
int interval = 500;

#define calibration_factor -1500.0 //This value is obtained using the SparkFun_HX711_Calibration sketch


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "HX711.h"

const char* ssid = "SsdaJapheemyundeejinda";  // Your own ssid here
const char* password = "01088940051"; // Your own password here
char scaleString[6];
float currentWeight = 0;


HX711 scale(D2,D3);

unsigned long tabletime =0;
unsigned long start = 0;
unsigned long finish=0;
int flag=0;
String working(float data) {
  if (10<data){
    if(flag==0)
    {
      flag=1;
      start=millis();
    }
  }
 else if (-10>data){
    if(flag==0)
      {
        flag=1;
        start=millis();
      }
    }
 if(data<10){
  if(flag==1)
   {
    finish=millis();
    tabletime+=finish-start;
    flag=0;
    finish=0;
    start=0;
   }
 }
 else if(data>-10){
  if(flag==1)
   {
    finish=millis();
    tabletime+=finish-start;
    flag=0;
    finish=0;
    start=0;
   }
 }
dtostrf(tabletime, 2, 2, scaleString);
Serial.println(scaleString);
Serial.println(data);
return(String("field1=")+String(scaleString));
}

void delivering(String payload) {
  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpPort)) {
    Serial.print("connection failed: ");
    Serial.println(payload);
    return;
  }
  String getheader = "GET "+ String(url) +"&"+ String(payload) +" HTTP/1.1";
  client.println(getheader);
  client.println("User-Agent: ESP8266");
  client.println("Host: " + String(host));
  client.println("Connection: close");
  client.println();
  
  Serial.println(getheader);
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    }
    Serial.println("Done cycle.");
}

void connect_ap() {
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
    Serial.print("\n Got WiFi, IP address: ");
    Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  connect_ap();
  
  Serial.println("ESPArduinoThingSpeak.cpp - 2017/05/28");

  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();  //Assuming there is no weight on the scale at start up, reset the scale to 0
  currentWeight = scale.get_units(1);
}

unsigned long mark = 0;
void loop() {
  if (millis() > mark ) {
    String payload = working(scale.get_units());
    delivering(payload);
    }
}
