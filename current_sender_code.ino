#include <PubSubClient.h>
#include <WiFiNINA.h>

const char* ssid = "Galaxy A14 5G FF82";
const char* password = "janvi2206";
const char* mqtt_server = "your_mqtt_server";
const int mqtt_port = 1883;
const char* mqtt_topic = "Data Receiver";

const int voltagePins[] = {A0,A2,A4};
const int resistorValue = 10000;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ArduinoClient")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.println("Connection failed, retrying...");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  for (int i = 0; i < 3; i++) {
    int voltageValue = analogRead(voltagePins[i]);
    float voltage = map(voltageValue, 0, 1023, 0, 5.0);
    float current = voltage / resistorValue;

    char message[50];
    snprintf(message, 50, "Resistor %d: Voltage: %.2f V, Current: %.2f mA", i + 1, voltage, current * 1000);
    client.publish(mqtt_topic, message);
    Serial.println(message);
  }

  client.loop();
  delay(1000); // Adjust this for your desired sampling rate
}
