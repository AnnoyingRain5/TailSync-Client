#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <esp_now.h>
#include <libTailSync.h>

#define LED_COUNT 10

uint8_t Broadcast_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

CRGB leds[LED_COUNT] = {};

void handleColourPacket(ColourPacket packet) {
  for (int i = 0; i < LED_COUNT; i++) {
    uint16_t row = i / 8;
    uint16_t col = i % 8;
    leds[i] = CRGB{packet.colour[row][col].red, packet.colour[row][col].green,
                   packet.colour[row][col].blue};
  }
  FastLED.show();
}

void handlePulsePacket() { Serial.println("[LOG]: pulse!\r\n"); }

void handleEndSessionPacket() { Serial.println("[LOG]: End session\r\n"); }
void handleMetachange(MetaPacket packet) {
  Serial.printf("LOG: Meta changed! Channel Name: %s DJ Name: %s, Song Name: "
                "%s, Colour Rate: %d\r\n",
                packet.channelName, packet.djName, packet.songName,
                packet.colourRate);
}

void setup() {
  Serial.begin(921600);
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    while (1) {
      Serial.println("[FATAL]: Failed to initialize ESP-NOW\r\n");
    }
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, Broadcast_MAC, 6);
  peerInfo.channel = 1;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    while (1) {
      Serial.println("[FATAL]: Failed to initialize ESP-NOW peer\r\n");
    }
  }

  esp_now_register_recv_cb(ParsePacket);

  setColourCallback(handleColourPacket);
  setPulseCallback(handlePulsePacket);
  setEndSessionCallback(handleEndSessionPacket);
  setMetaChangeCallback(handleMetachange);

  FastLED.addLeds<WS2812, 1, GRB>(leds, LED_COUNT)
      .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);
  FastLED.show(); // turn the LEDs off until we get data
}

void loop() {
  // nothing here
}
