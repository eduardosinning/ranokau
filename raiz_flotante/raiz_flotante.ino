#include <Arduino.h>
#include <WiFi.h>
const char *SSID = "RanoKau";
const char *PWD = "cooperacion";
void connectToWiFi() {
  Serial.print("Connectiog to ");
 
  WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected.");
  
}

