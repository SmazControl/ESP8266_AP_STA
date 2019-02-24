#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);
  String ESP_SSID;
  int numberOfNetworks = WiFi.scanNetworks();
  for(int i =0; i<numberOfNetworks; i++){
      Serial.print("Network name: ");
      Serial.println(WiFi.SSID(i));
      Serial.print("Signal strength: ");
      Serial.println(WiFi.RSSI(i));
      Serial.println("-----------------------");
      if (WiFi.SSID(i).substring(0,3)=="ESP") {
        ESP_SSID = WiFi.SSID(i);
      }
  }
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ESP_SSID.c_str(), "password");
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;
    int post = 0;
    Serial.print("[HTTP] begin...\n");
    http.begin("http://www.esp.com/get");

    Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
        post=payload.toInt();
        
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
    delay(10000);
    String PostData = "number="+String(post+100); // Post Random numer from Get +100
    http.begin("http://www.esp.com/post");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Serial.print("[HTTP] POST...\n");
    httpCode = http.POST(PostData);
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();    
  }
  delay(10000);
}
