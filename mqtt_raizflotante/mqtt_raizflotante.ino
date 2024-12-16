#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define RELE_ON LOW
#define RELE_OFF HIGH

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long periodo = 20000;  //the value is a number of milliseconds



//actuadores raiz flotante
const int electrovalvula_1 = 16;
const int electrovalvula_2 = 14;
const int electrovalvula_3 = 27;
const int electrovalvula_4 = 25;
const int electrovalvula_5 = 33;
const int electrovalvula_6 = 32;
int electrovalvulasPins[] = {
  electrovalvula_1, electrovalvula_2, electrovalvula_3, electrovalvula_4, electrovalvula_5, electrovalvula_6
}; 
const int bomba = 26;
const int pin_manual = 19;
const int pin_button = 18;
const int pin_dht22 = 4; 
boolean rutina = false;
int cont=0;
int pinCount = 6;


DHT sensorTH (pin_dht22, DHT22);    

//Conectarse a internet
const char *SSID = "RanoKau";
const char *PWD = "cooperacion";
void connectToWiFi() {
  Serial.println("Conectando a : " );
  WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected.");
}


// MQTT cliente
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 
char *mqttServer = "192.168.31.50";
int mqttPort = 1883;

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
}

void setup() {
  Serial.begin(9600);
  connectToWiFi();
  setupMQTT();
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(electrovalvulasPins[thisPin], OUTPUT);
  }
  pinMode(bomba, OUTPUT);
  pinMode(pin_button, INPUT);
  sensorTH.begin();
  Serial.print("Primero se apaga todo:");
  digitalWrite(bomba,RELE_OFF);
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    if (digitalRead(electrovalvulasPins[thisPin])==RELE_ON){
        digitalWrite(electrovalvulasPins[thisPin], RELE_OFF);
    }     
  }
}


void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      //clientId += String(random(0xffff), HEX);
      clientId +=String(1);
      Serial.println(clientId.c_str());
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("/raizflotante/piscina1");
        mqttClient.subscribe("/raizflotante/piscina2");
        mqttClient.subscribe("/raizflotante/piscina3");
        mqttClient.subscribe("/raizflotante/bomba");
        mqttClient.subscribe("/raizflotante/rutina");
      }
      
  }
}
void loop() {
  if (digitalRead(pin_button)==HIGH){
    
  }
  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
  float humedad = sensorTH.readHumidity();
  char humString[8];
  dtostrf(humedad, 1, 2, humString);
  mqttClient.publish("/raizflotante/dht22/humedad", humString);
  float temperatura = sensorTH.readTemperature();
  char tempString[8];
  dtostrf(temperatura, 1, 2, tempString);
  mqttClient.publish("/raizflotante/dht22/temperatura",tempString);

  if(rutina){
      currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
      if (currentMillis - startMillis >=periodo )  //test whether the period has elapsed
      { 
        Serial.print("Se apaga todo primero");
        Serial.print("\n");
        //Se apaga todo primero
        digitalWrite(bomba, RELE_OFF);
        delay(2000);
        for (int thisPin = 0; thisPin < pinCount; thisPin++) {
          if (digitalRead(electrovalvulasPins[thisPin])==RELE_ON){
              digitalWrite(electrovalvulasPins[thisPin], RELE_OFF);
          }
          
        }
        delay(2000);
        Serial.print("Esta todo apagado");
        Serial.print("\n");
        Serial.print("Se cambia :");
        Serial.print("\t");
        Serial.println(cont);
        //digitalWrite(electrovalvulasPins[cont],!digitalRead(electrovalvulasPins[cont]) );  //if so, change the state of the LED.  Uses a neat trick to change the state
        //digitalWrite(electrovalvulasPins[cont+1],!digitalRead(electrovalvulasPins[cont+1]))
        digitalWrite(electrovalvulasPins[cont],RELE_ON);
        digitalWrite(electrovalvulasPins[cont+1],RELE_ON);
        delay(2000);
        Serial.print("Se enciende la bomba");
        Serial.print("\n");
        digitalWrite(bomba,RELE_ON);
        delay(2000);

        startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
        
        cont=cont+2;
        if (cont>=5){
          Serial.print("Estas en el :");
          Serial.print("\t");
          Serial.print(cont);
          Serial.print("\n");
          cont=0;
        }
        
      }
  }
  else{
    
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  String byteRead = "";
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
        byteRead += (char)payload[i];
  }    
  Serial.println(byteRead);
  if (String(topic) == "/raizflotante/rutina"){
    if (byteRead == "off"){
      Serial.print("Estado rutina OFF");
      rutina = false;
      digitalWrite(rutina, LOW);
      Serial.print("Primero se apaga todo:");
      for (int thisPin = 0; thisPin < pinCount; thisPin++) {
        if (digitalRead(electrovalvulasPins[thisPin])==RELE_ON){
            digitalWrite(electrovalvulasPins[thisPin], RELE_OFF);
        }
          
      }
      //digitalWrite(LED, LED_OFF);
    }

    if (byteRead == "on"){
      Serial.println("Estado rutina ON");
      rutina = true;
      digitalWrite(rutina, HIGH);
      Serial.print("Primero se apaga todo:");
      for (int thisPin = 0; thisPin < pinCount; thisPin++) {
          if (digitalRead(electrovalvulasPins[thisPin])==RELE_ON){
              digitalWrite(electrovalvulasPins[thisPin], RELE_OFF);
          }
          
        }
      startMillis = millis();  //initial start time
      //digitalWrite(LED, LED_ON);
    }
  }
  else{
    if(rutina){
        Serial.print("estas en estado rutina");
    }
    else{
      if (String(topic) == "/raizflotante/piscina1"){
        Serial.print("entra a piscina 1");
        if (byteRead == "off"){
          Serial.println("piscina 1 OFF");
          digitalWrite(electrovalvula_1, RELE_OFF);
          digitalWrite(electrovalvula_2, RELE_OFF);
          //digitalWrite(LED, LED_OFF);
        }

        if (byteRead == "on"){
          Serial.println("piscina 1 ON");
          digitalWrite(electrovalvula_1, RELE_ON);
          digitalWrite(electrovalvula_2, RELE_ON);
          //digitalWrite(LED, LED_ON);
        }
      }
  
      if (String(topic) == "/raizflotante/piscina2"){
        if (byteRead == "off"){
          Serial.println("piscina 2 OFF");
          digitalWrite(electrovalvula_3, RELE_OFF);
          digitalWrite(electrovalvula_4, RELE_OFF);
          //digitalWrite(LED, LED_OFF);
        }

        if (byteRead == "on"){
          Serial.println("piscina 2 ON");
          digitalWrite(electrovalvula_3, RELE_ON);
          digitalWrite(electrovalvula_4, RELE_ON);
          //digitalWrite(LED, LED_ON);
        }
      }
      if (String(topic) == "/raizflotante/piscina3"){
        if (byteRead == "off"){
          Serial.println("piscina 3 OFF");
          digitalWrite(electrovalvula_5, RELE_OFF);
          digitalWrite(electrovalvula_6, RELE_OFF);
          //digitalWrite(LED, LED_OFF);
        }

        if (byteRead == "on"){
          Serial.println("piscina 3 ON");
          //digitalWrite(piscina3, HIGH);
          digitalWrite(electrovalvula_5, RELE_ON);
          digitalWrite(electrovalvula_6, RELE_ON);
          //digitalWrite(LED, LED_ON);
        }
      }
      if (String(topic) == "/raizflotante/bomba"){
        if (byteRead == "off"){
          Serial.println("Bomba Raiz flotante OFF");
          digitalWrite(bomba, RELE_OFF);
          //digitalWrite(LED, LED_OFF);
        }

        if (byteRead == "on"){
          Serial.println("Bomba Raiz flotante ON");
          digitalWrite(bomba, RELE_ON);
          //digitalWrite(LED, LED_ON);
        }
      }
    }
  }

  Serial.println();
  Serial.println(" — — — — — — — — — — — -");
}
void rutina_raizflotante(){


}