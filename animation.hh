#ifndef animation_h
#define animation_h

#include <map>

using namespace std;

const int NUMBER_OF_FIGURES = 10;

enum class MatrixAnimations { breathe, blink, swirl, First=breathe, Last=swirl };
enum class EyeAnimations { breathe, blink, angry, First=breathe, Last=angry };

void animationSetup();
void updateMatrixAnimation(int figure, string animation);
void updateEyeAnimation(int figure, string animation);
void updateAnimationMQTT();

***REMOVED***