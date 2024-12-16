#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// Configuración del sensor DHT22
#define DHTPIN 4      // Pin al que está conectado el sensor
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Configuración de WiFi
const char* ssid = "RanoKau";
const char* password = "cooperacion";

// Configuración de MQTT
const char* mqtt_server = "192.168.31.214";
const char* mqtt_topic = "sensor_plantinera/dht22";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    String payload = "Temperatura: " + String(t) + "°C Humedad: " + String(h) + "%";
    payload.toCharArray(msg, 50);
    Serial.print("Publicando mensaje: ");
    Serial.println(msg);
    client.publish(mqtt_topic, msg);
  }
}

