#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

void handlePost() {
  String message = "Number of args received:";
  message += webServer.args();  
  message += "\n";                  
  for (int i = 0; i < webServer.args(); i++) {
    message += "Arg nº" + (String)i + " –> ";
    message += webServer.argName(i) + ": ";
    message += webServer.arg(i) + "\n";
    if(webServer.argName(i)=="number") {
      Serial.println("Received "+String(webServer.arg(i)));
    }
  }
  webServer.send(200, "text/plain", message);
}

void handleGet() {
  String message = String(random(256));
  Serial.println("Send "+message);
  webServer.send(200, "text/plain", message);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  String Id = "ESP"+String(ESP.getChipId());
  WiFi.softAP(Id.c_str(),"password");
  Serial.println("AP Web Server Start...");
  Serial.println(Id);
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT, "www.esp.com", apIP);
  webServer.on("/post", handlePost); 
  webServer.on("/get", handleGet);
  webServer.onNotFound([]() {
    Serial.println("Hello World!");
    String message = "Hello World!\n\n";
    message += "URI: ";
    message += webServer.uri();

    webServer.send(200, "text/plain", message);
  });
  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
