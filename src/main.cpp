#include <Arduino.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <FastLED.h>
#include <connection.h>
#include <web.h>
#include <globals.h>
#include <animations.h>

// GENERAL
bool MASTERMODE;
bool AUTOMODE = true;

// FASTLED CONFIG
#define DATA_PIN    5
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

// ANIMATIONS
CRGB leds[NUM_LEDS];
uint8_t brightness = 128;
uint8_t delaySecond = 1000/NUM_LEDS;
uint8_t deltaHue = 255/NUM_LEDS;
uint16_t animationTime = 0;
uint16_t animationId = 0;
int NUM_ANIMATIONS = 0;

// WIFI CONFIG
char* ssid = "360LEDMASTER";
char* password = "12345678";
WiFiUDP udp;
const uint16_t UDPPORT = 5050;
IPAddress MasterIp = IPAddress(192,168,4,1);
LEDClient LEDClients[MAX_NUM_CLIENTS];
Animation animations[MAX_NUM_ANIMATIONS];
LEDClient Master = LEDClient(IPAddress(0,0,0,0),0);

// FUNCTIONS
void connectWiFi();
void checkWiFi();
void enableAP();
bool checkSSIDExists(String ssid);
void initMaster();
void handleMasterLoop();
void initClient();
void handleClientLoop();
void animate();

void setup() {
  // Serial
  Serial.begin(115200);

  // LEDS
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);
  initAnimations();
  animationStartUp();

  //FIXME ON Testing
  //animationId = NUM_ANIMATIONS-1;
  
  // WIFI
  bool exists = checkSSIDExists(ssid);
  if(exists){
    initClient();
  }else{
    initMaster();
  }
  startUdp();
}

void loop() {
  if(!MASTERMODE){
    handleClientLoop();
  }
  if(MASTERMODE){
    handleMasterLoop();
  }
  animate();

  EVERY_N_MILLISECONDS(8){
    animationTime++;
  }
  if(animationTime==65535){
    animationTime = 0;
    if(MASTERMODE){
      broadcastUDP("sync:0");
      delay(40); // ping 
    }
  }
  EVERY_N_SECONDS(5){
    broadcastUDP("sync:" + (String)animationTime);
    delay(40); // ping 
  }
  handleWebServer();
}

void handleMasterLoop(){
  handleMasterUdp();
  EVERY_N_SECONDS(3){
    broadcastUDP("Master");
  }
  EVERY_N_SECONDS(30){
    if(AUTOMODE){
      int newID;
      do{
        newID = random(0,NUM_ANIMATIONS);
      }while(newID == animationId);
      animationId = newID;
      Serial.println(animationId);
      broadcastUDP("animation:" + (String)animationId);
    }
  }
}

void handleClientLoop(){
  handleClientUdp();
  EVERY_N_SECONDS(3){
    checkWiFi();
    writeUDP(MasterIp,"Client");
  }
}

void connectWiFi(){
  WiFi.setHostname("360LED");
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    animationLoading();
    Serial.println("Connecting to WiFi..");
    if(WiFi.status() == WL_CONNECT_FAILED){
      Serial.println("failed, retrying");
      WiFi.begin(ssid,password);
    }
  }
  Serial.println(WiFi.localIP());
}

void checkWiFi(){
  if(WiFi.status() == WL_CONNECTED){
    return;
  }else{
    connectWiFi();
    /* if(WiFi.status() != WL_CONNECTED){
      bool exists = checkSSIDExists(ssid);
      if (!exists){
        initMaster();
      }
    } */
    return;
  }
}

void enableAP(){
  WiFi.setHostname("360LEDMASTER");
  WiFi.beginAP(ssid, password);
  while (WiFi.status() != WL_AP_LISTENING) {
    animationLoading();
    Serial.println("Creating access point..");
    if(WiFi.status() == WL_AP_FAILED){
      Serial.println("failed, retrying");
      WiFi.beginAP(ssid,password);
    }
  }
  Serial.println(WiFi.localIP());
}

bool checkSSIDExists(String ssid){
  int8_t n = WiFi.scanNetworks();
  int i;
  for(i=0;i<n;i++){
    if(WiFi.SSID(i) == ssid){
      return true;
    }
  }
  return false;
}

void initClient(){
  connectWiFi();
  MASTERMODE = false;
  writeUDP(MasterIp,"Client");
  animationIsClient();
  Master.lastActive = millis();  // time to receive heartbeat from master
}

void initMaster(){
  enableAP();
  MASTERMODE = true;
  initWebServer();
  int i;
  for(i=0;i<MAX_NUM_CLIENTS;i++){
    LEDClients[i] = LEDClient(IPAddress(0,0,0,0),0);
  }
  animationIsMaster();
}

void animate(){
  if(animationId != -1){
    animations[animationId].run();
  }
}