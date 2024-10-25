#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define MQTT_BROKER  "broker.emqx.io"
#define MQTT_TOPIC_SUBSCRIBE "binus/CompEng/IoT/esp32/cmd"
WiFiClient espClient;
PubSubClient mqtt(espClient);
boolean mqttConnect();

void setup() {
  pinMode(LED_D4, OUTPUT);
  pinMode(LED_D5, OUTPUT);
  Serial.begin(115200);
  Serial.println("Booting...");
  WiFi_Connect();
  mqttConnect();
}

void loop() {
  mqtt.loop();
}

void WiFi_Connect()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin("hd3_1", "celab123");
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.print("System connected with IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("RSSI: %d\n", WiFi.RSSI());
}

void mqttCallback(char* topic, byte* payload, unsigned int len) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.write(payload, len);
  Serial.println();
}
boolean mqttConnect() {
  char g_szDeviceId[30];
  sprintf(g_szDeviceId, "esp32_%08X",(uint32_t)ESP.getEfuseMac());
  mqtt.setServer(MQTT_BROKER, 1883);
  mqtt.setCallback(mqttCallback);
  Serial.printf("Connecting to %s clientId: %s\n", MQTT_BROKER, g_szDeviceId);

  boolean fMqttConnected = false;
  for (int i=0; i<3 && !fMqttConnected; i++) {
    Serial.print("Connecting to mqtt broker...");
    fMqttConnected = mqtt.connect(g_szDeviceId);
    if (fMqttConnected == false) {
      Serial.print(" fail, rc=");
      Serial.println(mqtt.state());
      delay(1000);
    }
  }
  if (fMqttConnected)
  {
    Serial.println(" success");
    mqtt.subscribe(MQTT_TOPIC_SUBSCRIBE);
    Serial.printf("Subcribe topic: %s\n", MQTT_TOPIC_SUBSCRIBE);
    // onPublishMessage();
  }
  return mqtt.connected();
}
