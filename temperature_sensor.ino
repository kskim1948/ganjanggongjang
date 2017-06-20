#include <OneWire.h>#include <DallasTemperature.h>#include <ESP8266WiFi.h>#include <WiFiClientSecure.h>#include <Time.h>#include <TimeLib.h>
#define ONE_WIRE_BUS D4   
//Time interval between measuring temperature.int interval = 1000;   // 10 sec
// Information to connect private Wificonst char* ssid = "ktEgg_493";  // Your own ssid hereconst char* password = "info22093"; // Your own password here
// Information to connect ThingSpeakconst char* host     = "api.thingspeak.com";String url           = "/update?api_key=2X09F0XCZ4FUW1NY";   // Your Own Key hereconst int httpPort   = 80;
// Information to connect My Private Server(Ubuntu VM @ Sogang Univ)const char* private_server = "52.78.118.210";const int serverPort       = 8000;
/********************************************************************/// Setup a oneWire instance to communicate with any OneWire devices  // (not just Maxim/Dallas temperature ICs) OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/// Pass our oneWire reference to Dallas Temperature. DallasTemperature sensors(&oneWire);
/***************************Program Code*****************************/String working() {   double temperature;    Serial.print("Requesting temperatures...");   sensors.requestTemperatures(); // Send the command to get temperature readings   Serial.println("DONE");     Serial.print("Temperature is: ");   temperature=sensors.getTempCByIndex(0);     // Why "byIndex"?   // 0 refers to the first IC on the wire   // You can have more than one DS18B20 on the same bus.    Serial.println(temperature);    return String(temperature);}
void delivering(String payload) {   WiFiClient client;  
  Serial.print("\nconnecting to Host: ");  Serial.println(private_server);
  //Connect to Private Server  if(client.connect(private_server,serverPort)){    //http call to server by using GET Method.    String getheader = "GET /?temp="+ String(payload) +" HTTP/1.1";    client.println(getheader);    client.println("User-Agent: ESP8266");      client.println("Host: " + String(private_server));      client.println("Connection: close");      client.println();      Serial.println(getheader);//To Check    while (client.connected()) {      String line = client.readStringUntil('\n');      Serial.println(line);    }  }  else{    Serial.print("connection failed to ");    Serial.println(private_server);  }    Serial.println("Done cycle.");}
//Connect to WiFivoid connect_ap() {  Serial.println();  Serial.print("connecting to WiFi: ");  Serial.println(ssid);  WiFi.begin(ssid, password);    while (WiFi.status() != WL_CONNECTED) {    delay(500);    Serial.print(".");  }  Serial.print("\n Got WiFi, IP address: ");  Serial.println(WiFi.localIP());  }
void setup() {  Serial.begin(115200);  connect_ap();
  Serial.println();  Serial.println("Arduino: Measure Temperature");   Serial.println("Dallas Temperature IC Control Library Demo");   // Start up the library   sensors.begin(); }
unsigned long mark = 0;void loop() {  if(second()%10==0)      {        String payload = working();                    delivering(payload);      }}

