#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <libTailSync.h>

uint8_t Broadcast_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void handleColourPacket(ColourPacket packet) {
  Serial.printf("[LOG]: colour! r: %d g: %d b: %d", packet.colour[0][0].red,
                packet.colour[0][0].green, packet.colour[0][0].blue);
  Serial.printf("[LOG]: Average head colour: %d",
                AverageColour(packet.colour[1][3], packet.colour[1][4],
                              packet.colour[2][3], packet.colour[2][4]));
}

void handlePulsePacket() { Serial.println("[LOG]: pulse!"); }

void handleEndSessionPacket() { Serial.println("[LOG]: End session"); }

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    while (1) {
      Serial.println("[FATAL]: Failed to initialize ESP-NOW");
    }
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, Broadcast_MAC, 6);
  peerInfo.channel = 1;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    while (1) {
      Serial.println("[FATAL]: Failed to initialize ESP-NOW peer");
    }
  }

  esp_now_register_recv_cb(ParsePacket);

  setColourCallback(handleColourPacket);
  setPulseCallback(handlePulsePacket);
  setEndSessionCallback(handleEndSessionPacket);
}

void loop() {
  // nothing here
}
