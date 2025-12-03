#include <Arduino.h>
#include <FastLED.h>
#include <TailSyncLogging.h>
#include <WiFi.h>
#include <esp_now.h>
#include <libTailSync.h>

#define LED_COUNT 10

uint8_t Broadcast_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

CRGB leds[LED_COUNT] = {};
CRGB led2[1] = {};

Logger logger = Logger("Client");

void handleColourPacket(ColourPacket packet) {
  // head
  for (int i = 0;
       i < std::min(sizeof(packet.head) / sizeof(Colour), (size_t)LED_COUNT);
       i++) {
    leds[i] =
        CRGB{packet.head[i].red, packet.head[i].green, packet.head[i].blue};
  }
  // body
  led2[0] = CRGB{packet.body[0].red, packet.body[0].green, packet.body[0].blue};
  FastLED.show();
}

void handlePulsePacket() { logger.log(DEBUG, "Pulse!"); }

void handleEndSessionPacket() { logger.log(DEBUG, "End session"); }
void handleMetachange(MetaPacket packet) {
  logger.log(DEBUG,
             "Meta changed! Channel Name: %s DJ Name: %s, Song Name: "
             "%s, Colour Rate: %d\r\n",
             packet.channelName, packet.djName, packet.songName,
             packet.colourRate);
}

void setup() {
  Serial.begin(921600);
  if (!setCpuFrequencyMhz(80)) {
    while (1) {
      logger.log(FATAL, "failed to set cpu clock");
    }
  }
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    while (1) {
      logger.log(FATAL, "Failed to initialize ESP-NOW");
    }
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, Broadcast_MAC, 6);
  peerInfo.channel = 1;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    while (1) {
      logger.log(FATAL, "Failed to initialize ESP-NOW peer");
    }
  }

  esp_now_register_recv_cb(ParsePacket);

  setColourCallback(handleColourPacket);
  setPulseCallback(handlePulsePacket);
  setEndSessionCallback(handleEndSessionPacket);
  setMetaChangeCallback(handleMetachange);

  FastLED.addLeds<WS2812, 1, GRB>(leds, LED_COUNT)
      .setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812, 21, RGB>(led2, 1).setCorrection(Typical8mmPixel);
  FastLED.setBrightness(255);
  FastLED.show(); // turn the LEDs off until we get data
}

void loop() {
  // nothing here
}
