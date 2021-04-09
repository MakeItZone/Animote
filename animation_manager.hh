#ifndef animation_manager_h
#define animation_manager_h
#include <NeoPixelAnimator.h>
#include "animations.hh"

using namespace std;

const int NUMBER_OF_FIGURES = 5;

void animationSetup();
int updateMatrixAnimation(int figure, const string &animation);
int updateEyeAnimation(int figure, const string &animation);
void updateAnimationTimebase(uint16_t timebasems);
void updateAnimationMQTT();

#endif