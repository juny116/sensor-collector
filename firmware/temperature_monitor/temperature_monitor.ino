#if !defined(WIFI_SSID) || !defined(WIFI_PASSWORD) || !defined(SENSOR_API_KEY)
#error "Provide WIFI_SSID, WIFI_PASSWORD and SENSOR_API_KEY through your local build environment"
#endif
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#define ONE_WIRE_BUS D4
#define THINKSPEAK 0
//#define IFTTT 0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#if THINKSPEAK
const char* host = "api.thingspeak.com";
String url = String("/update?api_key=") + SENSOR_API_KEY;   // Your Own Key here
const int httpPort = 80;
int interval = 60000;

#else
const char* host = "maker.ifttt.com";
String url = String("/trigger/hot_temp/with/key/") + SENSOR_API_KEY;   // Your Own Key here
const int httpPort = 80;
int interval = 5000;
#endif

const char* ssid = WIFI_SSID;   // Your own ssid here
const char* password = WIFI_PASSWORD;  // Your own password here

String working() {
  sensors.requestTemperatures();
  Serial.print("Temperature for Device 1 is: ");
  Serial.print(sensors.getTempCByIndex(0));
  float r = sensors.getTempCByIndex(0);// do some magic here!
  return(String("field1=")+String(r));
}

String working_ifttt() {
  sensors.requestTemperatures();
  Serial.print("Temperature for Device 1 is: ");
  Serial.print(sensors.getTempCByIndex(0));
  float r = sensors.getTempCByIndex(0);// do some magic here!
  return(String("value1=")+String(r));
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
  client.println("User-Agent: ESP8266 Jun Kim");
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

void delivering_ifttt(String payload) {
  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpPort)) {
    Serial.print("connection failed: ");
    Serial.println(payload);
    return;
  }
  String getheader = "GET "+ String(url) +"?"+ String(payload) +" HTTP/1.1";
  client.println(getheader);
  client.println("User-Agent: ESP8266 Jun Kim");
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
  sensors.begin();
  Serial.println("ESPArduinoThingSpeak.cpp - Temperature");
}

#if THINKSPEAK
unsigned long mark = 0;
void loop() {
  if (millis() > mark ) {
     mark = millis() + interval;
     String payload = working();
     delivering(payload);
  }
}

#else
unsigned long mark = 0;
void loop() {
  if (millis() > mark ) {
     mark = millis() + interval;
     String payload = working_ifttt();
     delivering_ifttt(payload);
  }
}
#endif
