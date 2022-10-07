#include <LEDClient.h>

LEDClient::LEDClient(IPAddress ip,int lastActive){
    this->ip = ip;
    this->lastActive = lastActive;
}
LEDClient::LEDClient(){
    this->ip = IPAddress(0,0,0,0);
    this->lastActive = 0;
}