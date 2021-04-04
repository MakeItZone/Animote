#ifndef animations_h
#define animations_h
#include <memory>
#include <NeoPixelBus.h>
#include "shared_animation_types.hh"

using namespace std; 

// error is a flag, not an animation name!
enum class MatrixAnimationIDs { error=-1, blank, breathe, First=undefined, Last=breathe };
enum class EyeAnimationIDs { error=-1, blank, breathe, First=undefined, Last=breathe };

struct MatrixAnimationChannelControllerDescriptor {
    MatrixAnimationIDs animation;
    uint16_t duration;
    std::unique_ptr<AnimationFunction_t> controller;
};

struct EyeAnimationChannelControllerDescriptor {
    EyeAnimationIDs animation;
    uint16_t duration;
    std::unique_ptr<AnimationFunction_t> controller;
};

MatrixAnimationIDs findAnimationID(string name);
void createMatrixAnimation(MatrixAnimationIDs id, int startLED, int numLEDs, MatrixAnimationChannelControllerDescriptor &matrix);

// animations
std::unique_ptr<AnimationFunction_t> solidColorMatrix(HslColor colour, int startLED, int numLEDs);
std::unique_ptr<AnimationFunction_t> solidColorEye(HslColor colour, int startLED, int numLEDs);

#endif