#ifndef LEDClient_h
#define LEDClient_h

#include <Arduino.h>
class LEDClient{
    public:
    IPAddress ip;
    int lastActive;
    LEDClient();
    LEDClient(IPAddress ip,int lastActive);
};
#endif