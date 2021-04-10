#ifndef animation_manager_h
#define animation_manager_h
#include <NeoPixelAnimator.h>
#include "animations.hh"

using namespace std;

const int NUMBER_OF_FIGURES = 1;
const int LEDS_PER_FIGURE = 15;

void animationSetup();
int updateMatrixAnimation(int figure, const string &animation);
void updateAnimationTimebase(uint16_t timebasems);
void updateAnimationMQTT();

#endif