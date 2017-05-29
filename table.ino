#include <Time.h>
#include <TimeLib.h>


const char* host = "api.thingspeak.com";
String url = "/update?api_key=3GOJDFXCK0U4I6R3";  // Your Own Key here
const int httpPort = 8080;
int interval = 500;
#define calibration_factor -1500.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "HX711.h"
const char* ssid = "ktEgg_493";  // Your own ssid here
const char* password = "info22093"; // Your own password here
char scaleString[6];
float currentWeight = 0;
const char* private_server = "52.78.118.210";
const int serverPort = 8080;

HX711 scale(D2,D3);
unsigned long tabletime =0;
unsigned long start = 0;
unsigned long finish=0;
int flag=0,seat=0;
int h=0,m=0;

String working(float data) {
  if (10<data or -10>data){
    if(flag==0)
    {
      flag=1;
      start=millis();
    }
  }
 
 if(data<10 and data>-10){
  if(flag==1)
   {
    finish=millis();
    tabletime=finish-start;
    //tabletime/=1000;
    flag=0;
    finish=0;
    start=0;
    tabletime=0;
    seat=1;
   }
 }



dtostrf(tabletime, 2, 2, scaleString);

Serial.println(tabletime/1000);
Serial.println(scaleString);
Serial.println(data);
return String(scaleString);
//return(String("field1=")+String(scaleString));
}
void delivering(String payload) {
  WiFiClient client;

    Serial.print("\nconnecting to Host: ");
  Serial.println(private_server);

  //Connect to Private Server
  if(client.connect(private_server,serverPort)){
    //http call to server by using GET Method.
    String getheader = "GET /?temp="+ String(payload) +" HTTP/1.1";
    client.println(getheader);
    client.println("User-Agent: ESP8266");  
    client.println("Host: " + String(private_server));  
    client.println("Connection: close");  
    client.println();
  
    Serial.println(getheader);//To Check
    
  }
  else{
    Serial.print("connection failed to ");
    Serial.println(private_server);
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
  if(hour()==23&&minute()==59&&second()>55)
  {
    tabletime=0;
    flag=0;
  }
  if (millis() > mark) {
    mark+=interval;
    String payload = working(scale.get_units());
    
      if(minute()%5==0 && second()==0&&seat==0)
      {
        if(flag==0)
         {
            delivering(payload);  
         }
         else if (flag==1)
         {
          finish=millis();
          tabletime=finish-start;
          dtostrf(tabletime, 2, 2, scaleString);
          //tabletime=0;
          delivering(scaleString);
          //start=millis();
         }
      }
      if(seat==1)
    {
      delivering(payload);  
      seat=0;
    }
      
    }
}

