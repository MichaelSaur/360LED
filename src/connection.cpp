#include <WiFiNINA.h>
#include <Arduino.h>
#include <WiFiUdp.h>
#include <globals.h>
#include <LEDClient.h>

char incomingPacket[256];
const int clientTimeout = 10000;

void updateClient(IPAddress ip);
void updateMaster(IPAddress ip);
void checkClients();
bool checkMaster();

void startUdp(){
    udp.begin(UDPPORT);
}

void writeUDP(IPAddress ip, String text){
  //checkWiFi();
  char buffer[text.length()+1];
  text.toCharArray(buffer,text.length()+1);
  udp.beginPacket(ip, UDPPORT);
  udp.write((uint8_t*)buffer,text.length()+1);
  udp.endPacket();
}

void broadcastUDP(String msg){
  for (uint8_t i=0; i<MAX_NUM_CLIENTS; i++) {
    if(LEDClients[i].ip != IPAddress(0,0,0,0)){
        writeUDP(LEDClients[i].ip,msg);
    }
  }
}

void handleMasterUdp(){
    int packetSize = udp.parsePacket();
    if (packetSize){ //Check if packet is avalible
        int len = udp.read(incomingPacket, 255);
        if (len > 0)
        {
            incomingPacket[len] = '\0';
        }
        
        String text = (String)incomingPacket;
        if((String)incomingPacket == "Client\0"){
            updateClient(udp.remoteIP());
        }
        Serial.print("UDP: ");
        Serial.print(incomingPacket);
        Serial.print(" from: ");
        Serial.println(udp.remoteIP());
    }
    checkClients();
}

bool handleClientUdp(){
    int packetSize = udp.parsePacket();
    if (packetSize){ //Check if packet is avalible
        int len = udp.read(incomingPacket, 255);
        if (len > 0)
        {
            incomingPacket[len] = '\0';
        }
        
        String text = (String)incomingPacket;
        if(text == "Master\0"){
            updateMaster(udp.remoteIP());
        }
        if(text.startsWith("sync:")){
            String animationTimeString = text.substring(5,text.length());
            Serial.println(text);
            Serial.println(animationTimeString);
            animationTime = animationTimeString.toInt();
        }
        if(text.startsWith("animation:")){
            String animationIdString = text.substring(10,text.length());
            Serial.println(text);
            Serial.println(animationIdString);
            animationId = animationIdString.toInt();
        }
        Serial.print("UDP: ");
        Serial.print(incomingPacket);
        Serial.print(" from: ");
        Serial.println(udp.remoteIP());
    }
    return checkMaster();
}


void updateClient(IPAddress ip){
    int i;
    for(i=0;i<MAX_NUM_CLIENTS;i++){
        if(LEDClients[i].ip == ip){
            LEDClients[i].lastActive = millis();
            return;
        }
    }
    // new Client
    for(i=0;i<MAX_NUM_CLIENTS;i++){
        if(LEDClients[i].ip == IPAddress(0,0,0,0)){
            LEDClients[i] = LEDClient(ip,millis());
            Serial.print("New client connected with Ip ");
            Serial.println(ip);
            broadcastUDP("newIp:" + IPtoString(LEDClients[i].ip));
            writeUDP(ip,"animation:" + (String)animationId);
            return;
        }
    }
    
}

void updateMaster(IPAddress ip){
    Master = LEDClient(ip,millis());
    Serial.print("Master on ");
    Serial.println(ip);
}

void checkClients(){
    int i;
    for(i=0;i<MAX_NUM_CLIENTS;i++){
        if(LEDClients[i].ip != IPAddress(0,0,0,0) && LEDClients[i].lastActive < (millis() - clientTimeout)){
            // remove client
            Serial.print("client ");
            Serial.print(LEDClients[i].ip);
            Serial.println(" timed out");
            broadcastUDP("delIp:" + IPtoString(LEDClients[i].ip));
            LEDClients[i].ip = IPAddress(0,0,0,0);
            LEDClients[i].lastActive = 0;
            return;
        }
    }
}

bool checkMaster(){
    if(Master.ip != IPAddress(0,0,0,0) && Master.lastActive < (millis() - clientTimeout)){
        // remove Master
        Serial.print("Master ");
        Serial.print(Master.ip);
        Serial.println(" timed out");
        Master.ip = IPAddress(0,0,0,0);
        Master.lastActive = 0;
        return false;
    }
    return true;
}

String IPtoString(IPAddress ip){
    char buffer [50];
    sprintf (buffer, "Ip: %d.%d.%d.%d", ip[0],ip[1],ip[2],ip[3]);
    return (String)buffer;
}