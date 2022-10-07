#include <Arduino.h>
#include <FastLED.h>
#include <globals.h>
#include <animations.h>

Animation::Animation(){
  this->name = "new Animation";
  this->id= -1;
}
Animation::Animation(String _name, void (*func)()){
  this->name = _name;
  this->id = -1;
  this->run = *func;
}

void Rainbow(){
    fill_rainbow(leds,NUM_LEDS,animationTime%255,deltaHue);
    FastLED.show();
}

void FullRGB(){
    fill_solid(leds,NUM_LEDS,CHSV(animationTime%255,255,255));
    FastLED.show();
}

void Walking_Rainbow(){
    fadeToBlackBy(leds,NUM_LEDS,50);
    leds[map(animationTime%255,0,255,0,29)] = CHSV(animationTime%255,255,255);
    FastLED.show();
}
void MasterSlave(){
  if(MASTERMODE){
    fill_solid(leds,NUM_LEDS,CRGB(255,0,0));
  }else{
    fill_solid(leds,NUM_LEDS,CRGB(0,255,0));
  }
  FastLED.show();
}
void animationMaster(){
    fill_solid(leds,NUM_LEDS,CRGB(255,0,0));
    FastLED.show();
}

void animationClient(){
    fill_solid(leds,NUM_LEDS,CRGB(0,255,0));
    FastLED.show();
}

void animationIsMaster(){
    int i;
    uint8_t d = delaySecond/2;
    for(i=0;i<NUM_LEDS;i++){
      leds[map(i,0,NUM_LEDS-1,NUM_LEDS/2,0)] = CRGB(255,0,0);
      leds[map(i,0,NUM_LEDS-1,NUM_LEDS/2,NUM_LEDS-1)] = CRGB(255,0,0);
      FastLED.show();
      delay(d);
    }
    for(i=0;i<NUM_LEDS;i++){
      fadeToBlackBy(leds,NUM_LEDS,50);
      FastLED.show();
      delay(d);
    }
}

void animationIsClient(){
    int i;
    uint8_t d = delaySecond/2;
    for(i=0;i<NUM_LEDS;i++){
      leds[map(i,0,NUM_LEDS-1,NUM_LEDS/2,0)] = CRGB(0,255,0);
      leds[map(i,0,NUM_LEDS-1,NUM_LEDS/2,NUM_LEDS-1)] = CRGB(0,255,0);
      FastLED.show();
      delay(d);
    }
    for(i=0;i<NUM_LEDS;i++){
      fadeToBlackBy(leds,NUM_LEDS,50);
      FastLED.show();
      delay(d);
    }
}

void animationLoading(){
    int i;
    int d = 1000/NUM_LEDS;
    for(i=0;i<NUM_LEDS;i++){
      fadeToBlackBy(leds,NUM_LEDS,50);
      leds[i] = CRGB(0,98,255);
      FastLED.show();
      Serial.println(i);
      delay(d);
    }
    for(i=NUM_LEDS-1;i>=0;i--){
      fadeToBlackBy(leds,NUM_LEDS,50);
      leds[i] = CRGB(0,98,255);
      FastLED.show();
      Serial.println(i);
      delay(d);
    }
}

void animationStartUp(){
    int i;
    for(i=0;i<NUM_LEDS;i++){
    leds[i] = CRGB(0,98,255);
    FastLED.show();
    delay(delaySecond);
  }
}

void AddAnimation(Animation a){
  int id;
  for(id=0;id<MAX_NUM_ANIMATIONS;id++){
    if(animations[id].id == -1){
      animations[id] = a;
      animations[id].id = id;
      NUM_ANIMATIONS++;
      return;
    }
  }
}

void addGlitter( fract8 chanceOfGlitter){
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
  FastLED.show();
}

void Rainbow_With_Glitter(){
  Rainbow();
  addGlitter(80);
}

void Confetti(){
  // random colored speckles that blink in and fade smoothly
  if(animationTime%2==0){
    fadeToBlackBy( leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV( animationTime%255 + random8(64), 200, 255);
    FastLED.show();
  }
}

void Sinelon(){
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  leds[beatsin16(13, 0, NUM_LEDS-1)] |= CHSV(animationTime%255, 255, 192);
  leds[beatsin16(13, 0, NUM_LEDS-1, 0, PHASE90)] |= CHSV(animationTime%255+128, 255, 192);
  FastLED.show();
}

void BPM(){
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, animationTime%255+(i*2), beat-animationTime%255+(i*10));
  }
  FastLED.show();
}

void Juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.show();
}

void Lasertag(){
  fadeToBlackBy( leds, NUM_LEDS, 20);
  leds[beatsin16(20,0, NUM_LEDS-1)] |= CHSV( 240, 220, 220);
  leds[beatsin16(30,0, NUM_LEDS-1,0,PHASE90)] |= CHSV( 0, 220, 220);
  leds[beatsin16(40,0, NUM_LEDS-1)] |= CHSV( 200, 255, 255);
  leds[beatsin16(50,0, NUM_LEDS-1,0,PHASE90)] |= CHSV( 0, 220, 220);
  FastLED.show();
}

void RainbowBump(){
  fadeToBlackBy(leds, NUM_LEDS, 40);
  uint8_t beat = beat8(120);
  leds[map(beat,0,255,NUM_LEDS/2,NUM_LEDS-1)] |= CHSV(animationTime%255+beat, 255, 255);
  leds[map(255-beat, 0,255, 0, NUM_LEDS/2)] |= CHSV(animationTime%255+beat, 255, 255);
  FastLED.show();
}

void initAnimations(){
  AddAnimation(Animation("Rainbow",Rainbow));
  AddAnimation(Animation("FullRGB",FullRGB));
  AddAnimation(Animation("Walking Rainbow",Walking_Rainbow));
  AddAnimation(Animation("Rainbow With Glitter",Rainbow_With_Glitter));
  AddAnimation(Animation("Confetti",Confetti));
  AddAnimation(Animation("Sinelon",Sinelon));
  AddAnimation(Animation("BPM",BPM));
  AddAnimation(Animation("Juggle",Juggle));
  AddAnimation(Animation("Lasertag",Lasertag));
  AddAnimation(Animation("RainbowBump",RainbowBump));
}