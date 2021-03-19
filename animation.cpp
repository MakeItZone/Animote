#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "animation.hh"
#include "network.hh"

using namespace std;

const uint16_t PixelCount = 51; 
const uint8_t PixelPin = 3;  // make sure to set this to the correct pin, ignored for Esp8266 (setting it for clarity)

std::map<MatrixAnimations, const char*> MatrixAnimationNames;
std::map<EyeAnimations, const char*> EyeAnimationNames;

// Note: for Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  
//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);
NeoPixelAnimator neoAnimator(NUMBER_OF_FIGURES * 2 + 1);

#define colorSaturation 128

MatrixAnimations matrices[NUMBER_OF_FIGURES];
EyeAnimations eyes[NUMBER_OF_FIGURES];

void updateMatrixMQTT(int figure){
    // <device>/figure/<number>/animation/matrix
    char buf[3]; // max two digits
    string topic = string("status/figure/");
    snprintf(buf, 3, "%d", figure);
    topic += buf;
    topic += "/animation/matrix";

    string payload = MatrixAnimationNames[matrices[figure]];

    mqttPublish(topic, payload);
}

void updateEyeMQTT(int figure) {
    // <device>/figure/<number>/animation/eye
    char buf[3]; // max two digits
    string topic = string("status/figure/");
    snprintf(buf, 3, "%d", figure);
    topic += buf;
    topic += "/animation/eye";

    string payload = EyeAnimationNames[eyes[figure]];

    mqttPublish(topic, payload);
}

void updateMatrixAnimation(int figure, string animation){
    if (figure > NUMBER_OF_FIGURES) {
        Serial.printf("USER: update matrix animation - figure out of range: %d\n", figure);
        return;
    }
    bool found = false;
    for ( const auto &i : MatrixAnimationNames ) {
        if (i.second == animation) {
            matrices[figure] = i.first;
            updateMatrixMQTT(figure);
            found = true;
            break;
        }
    }
    if (!found) { 
        Serial.printf("USER: invalid matrix animation name %s\n", animation.c_str());
    }
}

void updateEyeAnimation(int figure, string animation){
    if (figure > NUMBER_OF_FIGURES) {
        Serial.printf("USER: update eye animation - figure out of range: %d\n", figure);
        return;
    }
    bool found = false;
    for ( const auto &i : EyeAnimationNames ) {
        if (i.second == animation) {
            eyes[figure] = i.first;
            updateEyeMQTT(figure);
            found = true;
            break;
        }
    }
    if (!found) { 
        Serial.printf("USER: invalid eye animation name %s\n", animation.c_str());
    }
}

void updateAnimationMQTT() {
    for (int i =0 ; i < NUMBER_OF_FIGURES; i++) {
        updateMatrixMQTT(i);
        updateEyeMQTT(i);
    }
}

void animationSetup() {
    MatrixAnimationNames[MatrixAnimations::breathe] = "breath";
    MatrixAnimationNames[MatrixAnimations::blink] = "blink";
    MatrixAnimationNames[MatrixAnimations::swirl] = "swirl";

    EyeAnimationNames[EyeAnimations::breathe] = "breath";
    EyeAnimationNames[EyeAnimations::blink] = "blink";
    EyeAnimationNames[EyeAnimations::angry] = "angry";

    RgbColor red(colorSaturation, 0, 0);
    RgbColor green(0, colorSaturation, 0);
    RgbColor blue(0, 0, colorSaturation);
    RgbColor white(colorSaturation);
    RgbColor black(0);

    HslColor hslRed(red);
    HslColor hslGreen(green);
    HslColor hslBlue(blue);
    HslColor hslWhite(white);
    HslColor hslBlack(black);

    strip.Begin();
    strip.SetPixelColor(0, hslRed);
    strip.SetPixelColor(1, hslGreen);
    strip.SetPixelColor(2, hslBlue);
    strip.SetPixelColor(3, hslWhite);
    strip.Show();
}