#include <Arduino.h>
#include <FastLED.h>
#include <connection.h>
#include <animations.h>
#include <LEDClient.h>
#include <WiFi.h>

extern bool MASTERMODE;
extern bool AUTOMODE;

// ANIMATIONS
#define NUM_LEDS 29
extern CRGB leds[NUM_LEDS];
extern uint8_t brightness;
extern uint8_t delaySecond;
extern uint8_t deltaHue;
extern uint16_t animationTime;
extern uint16_t animationId;
#define PHASE90 32768

// WiFi
extern WiFiUDP udp;
extern const uint16_t UDPPORT;
extern IPAddress MasterIp;
#define MAX_NUM_CLIENTS 10
extern LEDClient LEDClients[MAX_NUM_CLIENTS];
extern LEDClient Master;
#define MAX_NUM_ANIMATIONS 100
extern int NUM_ANIMATIONS;
extern Animation animations[MAX_NUM_ANIMATIONS];