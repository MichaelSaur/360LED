#include <Arduino.h>

void startUdp();
void writeUDP(IPAddress ip, String text);
void broadcastUDP(String msg);
void handleMasterUdp();
bool handleClientUdp();
String IPtoString(IPAddress ip);