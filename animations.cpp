#include <map>
#include "animations.hh"
using namespace std;

extern NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip;

std::map<MatrixAnimationIDs, const char*> MatrixAnimationNames = {
    {MatrixAnimationIDs::blank, "blank"},
    {MatrixAnimationIDs::breathe, "breath"}
};
    
std::map<EyeAnimationIDs, const char*> EyeAnimationNames = {
    {EyeAnimationIDs::blank, "blank"},
    {EyeAnimationIDs::breathe, "breath"}
};

MatrixAnimationIDs findAnimationID(string name) {
    for ( const auto &i : MatrixAnimationNames ) {
        if (i.second == name) {
            return i.first;
        }
    }
    return MatrixAnimationIDs::error;
}

void createMatrixAnimation(MatrixAnimationIDs id, int startLED, int numLEDs, MatrixAnimationChannelControllerDescriptor &matrix) {
    switch (id)
    {
    case MatrixAnimationIDs::blank:
        matrix.animation = id;
        matrix.duration = 100; // 1 sec, assuming centisecs for animation controller
        matrix.controller = solidColorMatrix
        break;
    
    case MatrixAnimationIDs::breathe:
    default:
        break;
    }
}


//
// ANIMATIONS
//
// NOTE: Durations are relative to the duration set for the animation controller!
//
// animation function generator that creates a solid colour
std::unique_ptr<AnimationFunction_t> solidColorMatrix(HslColor colour, int startLED, int numLEDs) {
    auto al = [colour, startLED, numLEDs] (AnimationParam param) -> void {
        // only need to do this at the start of the animation cycle!
        if (param.progress == 0) {
            for (int i = 0; i < numLEDs; i++) {
                strip.SetPixelColor(startLED + i, colour);
            }
    };

    auto af = new std::function<void (AnimationParam param)>(al);
    auto aup = std::unique_ptr<AnimationFunction_t>(af);

    return aup; 
}

std::unique_ptr<AnimationFunction_t> solidColorEye(HslColor colour, int startLED, int numLEDs) {
    
    auto al = [colour] (AnimationParam param) -> void {
        int eye = param.index;
        strip.SetPixelColor(eye, colour);
    };

    auto af = new std::function<void (AnimationParam param)>(al);
    auto aup = std::unique_ptr<AnimationFunction_t>(af);

    return aup; 
}
