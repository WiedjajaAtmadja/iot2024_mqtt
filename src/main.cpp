#include <Arduino.h>
#include <WiFi.h>

void setup() {
  pinMode(LED_D4, OUTPUT);
  pinMode(LED_D5, OUTPUT);
  Serial.begin(115200);
  Serial.println("Booting...");
  WiFi.mode(WIFI_STA);
  WiFi.begin("hd3_1", "celab123");
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }  
  Serial.print("System connected with IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("RSSI: %d\n", WiFi.RSSI());
}

void loop() {

}
