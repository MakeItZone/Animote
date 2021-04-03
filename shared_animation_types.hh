#ifndef animation_types_h
#define animation_types_h
#include <memory>
#include <NeoPixelAnimator.h>

using namespace std;

typedef std::function<void (AnimationParam param)> AnimationFunction_t;

#endif