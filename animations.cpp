#include <string>
#include "animations.hh"

using namespace std;

extern NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip;
extern NeoPixelAnimator neoAnimator;

//
// ANIMATIONS
//
// NOTE: Durations are relative to the duration set for the animation controller!
//

//  animation for the 'spare'/'status'/'level shifting' NeoPixel
void statusAnimation(const AnimationParam& param) 
{
    const RgbColor red(128, 0, 0);
    const RgbColor green(0, 128, 0);
    const RgbColor blue(0, 0, 128);

    switch (param.state)
    {
    case AnimationState_Started:
        {
            break;
        }
    case AnimationState_Progress:
    {
        float adjProgress;
        RgbColor colour                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                ;
        if (param.progress < 1.0/3) {
            adjProgress = NeoEase::ExponentialInOut(param.progress * 3);
            colour = RgbColor::LinearBlend(red, green, adjProgress);
        } else if (param.progress < 2.0/3) {
            adjProgress = NeoEase::ExponentialInOut((param.progress - 1.0/3) * 3);
            colour = RgbColor::LinearBlend(green, blue, adjProgress);
        } else {
            adjProgress = NeoEase::ExponentialInOut((param.progress - 2.0/3) * 3);
            colour = RgbColor::LinearBlend(blue, red, adjProgress);
        }
        strip.SetPixelColor(0, colour);
        break;
    }
    case AnimationState_Completed:
    {
        // restart animation so it continues!
        neoAnimator.RestartAnimation(param.index);
        break;
    }

    default:
        break;
    }
}

std::unique_ptr<AnimationFunction_t> exampleAnimationFactory() {
    auto al = [] (const AnimationParam& param) -> void {
        switch (param.state)
        {
        case AnimationState_Started:
        {
            break;
        }
        case AnimationState_Progress:
        {
            break;
        }
        case AnimationState_Completed:
        {   // restart animation so it continues!
            neoAnimator.RestartAnimation(param.index);
            break;
        }
        default:
            // getting here would be an error state
            break;
        }
    };

    auto af = new std::function<void (const AnimationParam& param)>(al);
    auto aup = std::unique_ptr<AnimationFunction_t>(af);
    return aup; 
}

// Wall 1 - set the colours
std::unique_ptr<AnimationFunction_t> wallSteadyColorsAnimationFactory(const int startLED, const int numLEDs) {
    auto al = [startLED, numLEDs] (const AnimationParam& param) -> void {
        switch (param.state)
        {
        case AnimationState_Started:
        {
            const palette_t &palette = nightColours;
            strip.SetPixelColor(startLED,   palette.at("brown")); 
            strip.SetPixelColor(startLED+1, palette.at("brown")); 
            strip.SetPixelColor(startLED+2, palette.at("grey")); 
            strip.SetPixelColor(startLED+3, palette.at("grey")); 
            strip.SetPixelColor(startLED+4, palette.at("blue")); 
            strip.SetPixelColor(startLED+5, palette.at("orange")); 
            strip.SetPixelColor(startLED+6, palette.at("violet")); 
            strip.SetPixelColor(startLED+7, palette.at("yellow")); 
            strip.SetPixelColor(startLED+8, palette.at("black")); 
            strip.SetPixelColor(startLED+9, palette.at("black")); 
            strip.SetPixelColor(startLED+10, palette.at("white")); 
            strip.SetPixelColor(startLED+11, palette.at("green")); 
            strip.SetPixelColor(startLED+12, palette.at("green")); 
            strip.SetPixelColor(startLED+13, palette.at("red")); 
            strip.SetPixelColor(startLED+14, palette.at("red")); 

            break;
        }
        case AnimationState_Progress:
        {
            for (int i = startLED; i<startLED+numLEDs; i++) {
                if(param.progress <0.9) {
                    HslColor color = strip.GetPixelColor(i);
                    HslColor newColor = HslColor(color.H, color.S, 0.4f);
                    strip.SetPixelColor(i, newColor);
                } else {
                    HslColor color = strip.GetPixelColor(i);
                    HslColor newColor = HslColor(color.H, color.S, 0);
                    strip.SetPixelColor(i, newColor);
                }
            }
            break;
        }
        case AnimationState_Completed:
        {   // restart animation so it continues!
            neoAnimator.RestartAnimation(param.index);
            break;
        }
        default:
            // getting here would be an error state
            break;
        }
    };

    auto af = new std::function<void (const AnimationParam& param)>(al);
    auto aup = std::unique_ptr<AnimationFunction_t>(af);
    return aup; 
}


// animation function generator that creates a solid colour
std::unique_ptr<AnimationFunction_t> solidColourAnimationFactory(const HslColor &colour, const int startLED, const int numLEDs) {
    auto al = [colour, startLED, numLEDs] (const AnimationParam& param) -> void {
        switch (param.state)
        {
        case AnimationState_Started:
        {   
            for (int i = 0; i < numLEDs; i++) {
                strip.SetPixelColor(startLED + i, colour);
            }
            break;
        }
        case AnimationState_Progress:
        {
            break;
        }
        case AnimationState_Completed:
        {    // restart animation so it continues!
            neoAnimator.RestartAnimation(param.index);
            break;
        }
        default:
        {
            // getting here would be an error state
            break;
        }
        }
    };

    auto af = new std::function<void (const AnimationParam& param)>(al);
    auto aup = std::unique_ptr<AnimationFunction_t>(af);

    return aup; 
}

std::unique_ptr<AnimationFunction_t> hglAnimationFactory(const int baseCode, const palette_t* const palette, const int conversationWidth, const int startLED, const int numLEDs ) {
    auto al = [baseCode, palette, conversationWidth, startLED, numLEDs] (const AnimationParam& param) -> void {
        switch (param.state)
        {
        case AnimationState_Started:
        {
            // move all the colours down one
            for (int i = numLEDs-1; i > 0; i--) {
                strip.SetPixelColor(startLED + i, strip.GetPixelColor(startLED +i-1));
            }
            int newColourIndex = 10 + baseCode - conversationWidth + random(2*conversationWidth);
            newColourIndex %= 10; //take the remainder to get back in range!
            const string& colourName = resistorCodes.at(newColourIndex);
            // at will throw an exception if the key is not in the map- which shouldn't be able to happen.
            strip.SetPixelColor(startLED, palette->at(colourName)); 
            break;
        }
        case AnimationState_Progress:
        {
            break;
        }
        case AnimationState_Completed:
        {   // restart animation so it continues!
            neoAnimator.RestartAnimation(param.index);
            break;
        }
        default:
            // getting here would be an error state
            break;
        }
    };

    auto af = new std::function<void (const AnimationParam& param)>(al);
    auto aup = std::unique_ptr<AnimationFunction_t>(af);
    return aup; 
}

std::unique_ptr<AnimationFunction_t> solidFadeAnimationFactory(const HslColor &colour, const float halfHoldTime, const AnimEaseFunction easingFn, const int startLED, const int numLEDs ) {
    auto al = [colour, halfHoldTime, easingFn, startLED, numLEDs] (const AnimationParam& param) -> void {
        switch (param.state)
        {
        case AnimationState_Started:
        {
            break;
        }
        case AnimationState_Progress:
        {
            float lum;
            if (param.progress <= (0.5 - halfHoldTime)) {
                lum = easingFn(param.progress /(0.5 - halfHoldTime)) * 0.45f;
            } else if (param.progress <= (0.5 + halfHoldTime)) {
                lum = 0.45f;
            } else {
                lum = easingFn((1.0f-param.progress)/(0.5 - halfHoldTime)) * 0.45f; 
            }
            auto updateColour = HslColor(colour.H, colour.S, lum);
            for (int i = 0; i < numLEDs; i++) {
                strip.SetPixelColor(startLED + i, updateColour);
            }

            break;
        }
        case AnimationState_Completed:
        {   // restart animation so it continues!
            neoAnimator.RestartAnimation(param.index);
            break;
        }
        default:
            // getting here would be an error state
            break;
        }
    };

    auto af = new std::function<void (const AnimationParam& param)>(al);
    auto aup = std::unique_ptr<AnimationFunction_t>(af);
    return aup; 
}

//
// -------------- decoding and animaton closure creation --------------
//

// return the next delimited chunk, and location just after the delimeter.
// if no delimeter found, returns sbstring from start to end, and location returned is the end of the string
std::size_t findChunk(const string &input, const std::size_t start, string& output, char delimeter = '-') {
    if (start >= input.length()) {
        return string::npos;
    } else {
        int end = input.find_first_of(delimeter, start);
        if (string::npos == end) {
            end = input.length();
        } 
        output = input.substr(start, end-start);

        int tmp = ((end + 1) >= input.length()) ? input.length() : (end+1);
        return tmp;
    }
}

std::size_t parseNumber(const string &args, const int start, int &num){
    string speedStr;
    int next = findChunk(args, start, speedStr);
    if (string::npos == next) {
        return string::npos;
    }
    num = atoi(speedStr.c_str());
    return next;
}

int decodePaletteAndColour(const string& paletteName, const string& colourName, palette_t * &palette,HslColor &colour) {
    if ("day"==paletteName) {
        palette = &dayColours;
    } else if ("night" == paletteName) {
        palette = &nightColours;
    } else {
        return -1;
    }

    auto search = palette->find(colourName);
    if (palette->end() == search) {
        return  -1;
    }
    const HsbColor &hsbColour = search->second;
    colour = HslColor(RgbColor(hsbColour));
    return 0;
}

std::size_t parseStdParams(const string &animationName, std::size_t next, string &colourName, string &paletteName, HslColor &colour, palette_t *&palette){
    next = findChunk(animationName, next, paletteName);
    if (string::npos == next) {
        return string::npos;
    }

    next = findChunk(animationName, next, colourName);
    if (string::npos == next) {
        return string::npos;
    }
    
    int result = decodePaletteAndColour(paletteName, colourName, palette, colour);
    if (-1 == result) {
        return string::npos;
    }    

    return next;
}

// animationName format:
// name-pallette-basecolor-speed[-optional]
// e.g. 
//    x  blank
//    x  solid-day-red
//    x  hgl-day-green-100-3     <-- the '100' is the duration, and the '3' is how many colors ('friends') either side of the bsase colour select colours from
//    x  hgl-night-purple-200-5
//       solidfade-palette-color-speed-easefn-holdtime
//       relaxed-day-purple-250-20  <-- speed, then lightness variation
//
// Do all string parsing here!
int createAnimation(const string& animationName, const int startLED, const int numLEDs, AnimationChannelControllerDescriptor &matrix) {
    matrix.animationName=std::move(std::unique_ptr<string>(new string(animationName)));

    std::size_t next;
    string function;
    next = findChunk(animationName, 0, function);
    if (string::npos == next) {
        return string::npos;
    }

    if ("blank" == function) {
        matrix.controller=solidColourAnimationFactory(HslColor(0,0,0), startLED, numLEDs);
        matrix.duration = UINT16_MAX;
        return 0;
    }

    if ("wallsteady" == function) {
        int speed;
        next = parseNumber(animationName, next, speed);
        if (string::npos == next) {
            return -1;
        }
        matrix.controller= wallSteadyColorsAnimationFactory(startLED, numLEDs);
        matrix.duration = speed;
        return 0;

    }

    // standard params are function-palette-colour-speed
    HslColor baseColour;
    string colourName;
    string paletteName;
    const palette_t* palette;

    next = parseStdParams(animationName, next, colourName, paletteName, baseColour, palette);
    if (string::npos == next) {
        return -1;
    }

    if ("solid" == function) {
        matrix.controller=solidColourAnimationFactory(baseColour, startLED, numLEDs);
        matrix.duration = UINT16_MAX;
        return 0;
    }

    int speed;
    next = parseNumber(animationName, next, speed);
    if (string::npos == next) {
        return -1;
    }
    if (speed < 0) {
        speed = 0;
    }
    
    if ("hgl" == function) {
        int conversationWidth;
        next = parseNumber(animationName, next, conversationWidth);
        if (string::npos == next) {
            return -1;
        }
        if (conversationWidth < 0) {
            conversationWidth = 0;
        } else if (conversationWidth > 10) {
            conversationWidth = 10;
        }

        int baseCode = -1;
        auto it = resistorCodes.begin();

        while (it != resistorCodes.end()) {
            if (it->second == colourName) {
                baseCode = it->first;
                break;
            }
            it++;
        }

        if (-1 == baseCode) {
            // something has to be really broken to ever get here
            return -1;
        }
        matrix.controller=hglAnimationFactory(baseCode, palette, conversationWidth, startLED, numLEDs);
        matrix.duration = speed;
        return 0;
    } else if ("solidfade" == function) {
        // solidfade-palette-colour-speed-easefn-holdtime
        string easeFnName;
        next = findChunk(animationName, next, easeFnName);
        if (string::npos == next) {
            return string::npos;
        }
        
        auto search = easingFnsByName.find(easeFnName);
        if (easingFnsByName.end() == search) {
            return  -1;
        }
        const auto &easeFn = search->second;

        int holdTime;
        next = parseNumber(animationName, next, holdTime);
        if (string::npos == next) {
            return -1;
        }

        float halfHoldTime = holdTime/200.0f; // in half and as a percentage

        matrix.controller= solidFadeAnimationFactory(baseColour, halfHoldTime, easeFn, startLED, numLEDs);
        matrix.duration = speed;
        return 0;
    }

    return -1;
}

int createMatrixAnimation(const string &animationName, const int startLED, const int numLEDs, AnimationChannelControllerDescriptor &matrix) {
    // add handling for any matrix-specific implementations
    // but probably most eye animations can be single pixel versions of the matrix animations
    return createAnimation(animationName, startLED, numLEDs, matrix);
}
