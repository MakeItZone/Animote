#include <map>
#include "animations.hh"
using namespace std;

extern NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip;

std::map<MatrixAnimations, const char*> MatrixAnimationNames;
std::map<EyeAnimations, const char*> EyeAnimationNames;

void CreateListOfAnimations() {
    MatrixAnimationNames[MatrixAnimations::blank] = "blank";
    MatrixAnimationNames[MatrixAnimations::breathe] = "breath";
    MatrixAnimationNames[MatrixAnimations::blink] = "blink";
    MatrixAnimationNames[MatrixAnimations::swirl] = "swirl";

    EyeAnimationNames[EyeAnimations::blank] = "blank";
    EyeAnimationNames[EyeAnimations::breathe] = "breath";
    EyeAnimationNames[EyeAnimations::blink] = "blink";
    EyeAnimationNames[EyeAnimations::angry] = "angry";


}

// animation function generator that creates a solid colour
std::unique_ptr<AnimationFunction_t> solidColorMatrix(HslColor colour) {
    auto al = [colour] (AnimationParam param) -> void {
        int figure = param.index;
        for (int i = figure * 10; i < (figure + 1) *10; i++) {
            strip.SetPixelColor(i, colour);
        }
    };

    auto af = new std::function<void (AnimationParam param)>(al);
    auto aup = std::unique_ptr<AnimationFunction_t>(af);

    return aup; 
}

std::unique_ptr<AnimationFunction_t> solidColorEye(HslColor colour) {
    
    auto al = [colour] (AnimationParam param) -> void {
        int eye = param.index;
        strip.SetPixelColor(eye, colour);
    };

    auto af = new std::function<void (AnimationParam param)>(al);
    auto aup = std::unique_ptr<AnimationFunction_t>(af);

    return aup; 
}
