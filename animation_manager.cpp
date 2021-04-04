#include <map>
#include <memory>
#include <H4.h>
#include <NeoPixelBus.h>
#include "animation_manager.hh"
#include  "animations.hh"
#include "network.hh"

using namespace std;

const uint16_t PixelCount = 51; 
const uint8_t PixelPin = 3;  // make sure to set this to the correct pin, ignored for Esp8266 (setting it for clarity)

// Note: for Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  
//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);
NeoPixelAnimator neoAnimator(NUMBER_OF_FIGURES * 2 + 1, NEO_DECISECONDS); // twice as many figures - figures + eyes
NeoGamma<NeoGammaTableMethod> colorGamma;
H4_TIMER animationTimer;

#define colorSaturation 128

// keep track of the animiation name and current fucntion implementing it.
// unique_ptr has the nice property that it frees what it's pointing to when a new thing is assigned to it.
// This meams we don't have to track and manage memory.
MatrixAnimationChannelControllerDescriptor matrices[NUMBER_OF_FIGURES];
EyeAnimationChannelControllerDescriptor eyes[NUMBER_OF_FIGURES];

int matrixAnimationChannel(int figure){
    return figure * 2 + 1; // + 1 to skip status LED
}

int eyeAnimationChannel(int figure){
    return figure * 2 + 2; // + 1 to skip status LED
}

int figureStartLED(int figure) {
    return figure * 10 + 1;
}

int eyeStartLED(int figure) {
    return figure * 10 + 10; // 9 for the matrix + 1 for status/safety LED
}

void updateMatrixMQTT(int figure, string payload){
    // <device>/figure/<number>/animation/matrix
    char buf[3]; // max two digits
    string topic = string("status/figure/");
    snprintf(buf, 3, "%d", figure);
    topic += buf;
    topic += "/animation/matrix";
    mqttPublish(topic, payload);
}

void updateEyeMQTT(int figure, string payload) {
    // <device>/figure/<number>/animation/eye
    char buf[3]; // max two digits
    string topic = string("status/figure/");
    snprintf(buf, 3, "%d", figure);
    topic += buf;
    topic += "/animation/eye";
    mqttPublish(topic, payload);
}

void updateAnimationTimebaseMQTT(uint16_t speed) {
    // <device>/animation/speed
    char buf[6];
    string topic = string("status/animation/speed");
    snprintf(buf, 6, "%d", speed);
    mqttPublish(topic, buf);
}

int updateMatrixAnimation(int figure, string animation){
    if ((figure<0) || (figure > NUMBER_OF_FIGURES)) {
        Serial.printf("USER: update matrix animation - figure out of range: %d\n", figure);
        return -1;
    }

    const MatrixAnimationIDs id = findAnimationID(animation);
    if (id == MatrixAnimationIDs::error) { 
        Serial.printf("USER: invalid matrix animation name %s\n", animation.c_str());
        return -1;
    }

    // don't re-create & re-start the animation if it's the same as running animation
    if (id == matrices[figure].animation) {
        return 0;
    }

    // stop the channel animation so there's no chance of it being called while we're changing callbacks
    int channel = matrixAnimationChannel(figure);
    neoAnimator.StopAnimation(channel);

    createMatrixAnimation(id, figureStartLED(figure), 9,  &matrices[figure]);
    neoAnimator.StartAnimation(channel, matrices[figure].duration, matrices[figure].controller);
    return 0;
}

int updateEyeAnimation(int figure, string animation){
    if ((figure < 0 ) || (figure > NUMBER_OF_FIGURES)) {
        Serial.printf("USER: update eye animation - figure out of range: %d\n", figure);
        return -1;
    }
    bool found = false;
    for ( const auto &i : EyeAnimationNames ) {
        if (i.second == animation) {
            eyes[figure].first = i.first;
            updateEyeMQTT(figure);
            found = true;
            break;
        }
    }
    if (!found) { 
        Serial.printf("USER: invalid eye animation name %s\n", animation.c_str());
        return -1;
    }
    return 0;
}

void updateAnimationTimebase(uint16_t timebasems) {
    neoAnimator.setTimeScale(timebasems);
    Serial.printf("USER: animation speed: %d\n", timebasems);
    updateAnimationTimebaseMQTT(neoAnimator.getTimeScale());
}

void updateAnimationMQTT() {
    for (int i =0 ; i < NUMBER_OF_FIGURES; i++) {
        updateMatrixMQTT(i);
        updateEyeMQTT(i);
    }
    updateAnimationTimebaseMQTT(neoAnimator.getTimeScale());
}

void animationSetup() {
    CreateListOfAnimations();
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