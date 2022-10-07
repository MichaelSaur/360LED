#include <Arduino.h>
#include <FastLED.h>

#include <Arduino.h>

#ifndef animations_h
#define animations_h
class Animation{
    public:
    String name;
    int id;
    Animation();
    Animation(String _name,  void (*func)());
    void (*run)();
};
#endif

void initAnimations();

void animationStartUp();
void animationIsMaster();
void animationIsClient();
void animationLoading();

void animationMaster();
void animationClient();