#ifndef animations_h
#define animations_h
#include <memory>
#include <NeoPixelBus.h>
#include "animation_manager.hh"

using namespace std; 



enum class MatrixAnimations { blank, breathe, blink, swirl, First=blank, Last=swirl };
enum class EyeAnimations { blank, breathe, blink, angry, First=blank, Last=angry };

struct MatrixAnimationChannelControllerDescriptor {
    MatrixAnimations animationName;
    u_int period;
    std::unique_ptr<AnimationFunction_t> controller;
};

struct EyeAnimationChannelControllerDescriptor {
    EyeAnimations animationName;
    u_int period;
    std::unique_ptr<AnimationFunction_t> controller;
};



void CreateListOfAnimations( void );

std::unique_ptr<AnimationFunction_t> solidColorMatrix(HslColor colour);
std::unique_ptr<AnimationFunction_t> solidColorEye(HslColor colour);

#endif