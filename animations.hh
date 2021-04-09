#ifndef animations_h
#define animations_h
#include <memory>
#include <map>
#include <NeoPixelBus.h>
#include "shared_animation_types.hh"

using namespace std; 
typedef const std::map<const int, const string> rcodes_t;
typedef const std::map<const string, const HsbColor> palette_t;
typedef const std::map<const string, const AnimEaseFunction> easemap_t;
// resistor codes
rcodes_t resistorCodes = {
    {0, "black"},
    {1, "brown"},
    {2, "red"},
    {3, "orange"},
    {4, "yellow"},
    {5, "green"},
    {6, "blue"},
    {7, "violet"},
    {8, "grey"},
    {9, "white"}
};

// palettes
palette_t dayColours = {
    {"black", HsbColor(212.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f)},
    {"brown", HsbColor(22.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f)},
    {"red",   HsbColor(2.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f)},
    {"orange",HsbColor(14.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f)},
    {"yellow",HsbColor(50.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f)},
    {"green", HsbColor(85.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f)},
    {"blue",  HsbColor(155.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f)},
    {"violet",HsbColor(185.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f)},
    {"grey",  HsbColor(100.0f/255.0f, 100.0f/255.0f, 255.0f/255.0f)},
    {"white", HsbColor(100.0f/255.0f, 0.0f/255.0f, 255.0f/255.0f)}
};

palette_t nightColours = {
    {"black", HsbColor(212.0f/255.0f, 200.0f/255.0f, 60.0f/255.0f)},
    {"brown", HsbColor(22.0f/255.0f,210.0f/255.0f, 60.0f/255.0f)},
    {"red",   HsbColor(2.0f/255.0f, 200.0f/255.0f, 60.0f/255.0f)},
    {"orange",HsbColor(14.0f/255.0f, 200.0f/255.0f, 60.0f/255.0f)},
    {"yellow",HsbColor(50.0f/255.0f, 200.0f/255.0f, 60.0f/255.0f)},
    {"green", HsbColor(85.0f/255.0f, 200.0f/255.0f, 60.0f/255.0f)},
    {"blue",  HsbColor(155.0f/255.0f, 200.0f/255.0f, 60.0f/255.0f)},
    {"violet",HsbColor(185.0f/255.0f, 200.0f/255.0f, 60.0f/255.0f)},
    {"grey",  HsbColor(100.0f/255.0f, 60.0f/255.0f, 60.0f/255.0f)},
    {"white", HsbColor(100.0f/255.0f, 0.0f/255.0f, 60.0f/255.0f)}
};

// easing names to function map
easemap_t easingFnsByName = {
    {"linear"   ,  NeoEase::Linear},
    {"quadraticin"          ,  NeoEase::QuadraticIn},
    {"quadraticout"         ,  NeoEase::QuadraticOut},
    {"quadraticinout"       ,  NeoEase::QuadraticInOut},
    {"quadraticcenter"      ,  NeoEase::QuadraticCenter},
    {"cubicin"              ,  NeoEase::CubicIn},
    {"cubicout"             ,  NeoEase::CubicOut},
    {"cubicinout"           ,  NeoEase::CubicInOut},
    {"cubiccenter"          ,  NeoEase::CubicCenter},
    {"quarticin"            ,  NeoEase::QuarticIn},
    {"quarticout"           ,  NeoEase::QuarticOut},
    {"quarticinout"         ,  NeoEase::QuarticInOut},
    {"quarticcenter"        ,  NeoEase::QuarticCenter},
    {"quinticin"            ,  NeoEase::QuinticIn},
    {"quinticout"           ,  NeoEase::QuinticOut},
    {"quinticinout"         ,  NeoEase::QuinticInOut},
    {"quinticcenter"        ,  NeoEase::QuinticCenter},
    {"sinusoidalin"         ,  NeoEase::SinusoidalIn},
    {"sinusoidalout"        ,  NeoEase::SinusoidalOut},
    {"sinusoidalinout"      ,  NeoEase::SinusoidalInOut},
    {"sinusoidalcenter"     ,  NeoEase::SinusoidalCenter},
    {"exponentialin"        ,  NeoEase::ExponentialIn},
    {"exponentialout"       ,  NeoEase::ExponentialOut},
    {"exponentialinout"     ,  NeoEase::ExponentialInOut},
    {"exponentialcenter"    ,  NeoEase::ExponentialCenter},
    {"circularin"           ,  NeoEase::CircularIn},
    {"circularout"          ,  NeoEase::CircularOut},
    {"circularinout"        ,  NeoEase::CircularInOut},
    {"circularcenter"       ,  NeoEase::CircularCenter},
    {"gamma"                ,  NeoEase::Gamma}
};

struct AnimationChannelControllerDescriptor {
    std::unique_ptr<string> animationName;
    uint16_t duration;
    std::unique_ptr<AnimationFunction_t> controller;
};

int createMatrixAnimation(const string &animationName, const int startLED, const int numLEDs, AnimationChannelControllerDescriptor &matrix);
int createEyeAnimation(const string &animationName, const int startLED, const int numLEDs, AnimationChannelControllerDescriptor &eye);

// animations
void statusAnimation(const AnimationParam& param);
std::unique_ptr<AnimationFunction_t> solidColourAnimationFactory(const HslColor &colour, const int startLED, const int numLEDs);

#endif