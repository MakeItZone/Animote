#ifndef animation_manager_h
#define animation_manager_h
#include <NeoPixelAnimator.h>

using namespace std;

const int NUMBER_OF_FIGURES = 10;

void animationSetup();
int updateMatrixAnimation(int figure, string animation);
int updateEyeAnimation(int figure, string animation);
void updateAnimationTimebase(uint16_t timebasems);
void updateAnimationMQTT();

#endif