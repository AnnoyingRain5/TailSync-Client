#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include <libTailSync.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("[FATAL]: Failed to initialize ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(ParsePacket);

  setColourCallback(handleColourPacket);
  setPulseCallback(handlePulsePacket);
  setEndSessionCallback(handleEndSessionPacket);
}

void loop() {
  // nothing here
}

void handleColourPacket(ColourPacket packet){
  Serial.printf("[LOG]: colour! r: %d g: %d b: %d", packet.colour[0][0].red, packet.colour[0][0].green, packet.colour[0][0].blue);
  Serial.printf("[LOG]: Average head colour: %d", AverageColour(packet.colour[1][3], packet.colour[1][4], packet.colour[2][3], packet.colour[2][4]));
}

void handlePulsePacket(){
  Serial.println("[LOG]: pulse!");
}

void handleEndSessionPacket(){
  Serial.println("[LOG]: End session");
}
