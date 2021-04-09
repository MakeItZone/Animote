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
NeoPixelAnimator neoAnimator(NUMBER_OF_FIGURES * 2 + 1, NEO_CENTISECONDS); // twice as many figures - figures + eyes
NeoGamma<NeoGammaTableMethod> colorGamma;

// keep track of the animiation name and current function implementing it.
// unique_ptr has the nice property that it frees what it's pointing to when a new thing is assigned to it.
// This meams we don't have to track and manage memory.
AnimationChannelControllerDescriptor matrices[NUMBER_OF_FIGURES];
AnimationChannelControllerDescriptor eyes[NUMBER_OF_FIGURES];

void updateAndShowAnimitions() {
    neoAnimator.UpdateAnimations();
    strip.Show();
}


int matrixAnimationChannel(int figure){
    return figure * 2 + 1; // + 1 to skip status LED
}

int eyeAnimationChannel(int eye){
    return eye * 2 + 2; // + 1 to skip status LED
}

int figureStartLED(int figure) {
    return figure * 10 + 1;
}

int eyeStartLED(int figure) {
    return figure * 10 + 10; // 9 for the matrix + 1 for status/safety LED
}

void updateMatrixMQTT(int figure){
    // <device>/figure/<number>/animation/matrix
    if(matrices[figure].animationName) {
        char buf[3]; // max two digits
        string topic = string("status/figure/");
        snprintf(buf, 3, "%d", figure);
        topic += buf;
        topic += "/animation/matrix";
        mqttPublish(topic, *matrices[figure].animationName.get());
    }
}

void updateEyeMQTT(int eye) {
    // <device>/figure/<number>/animation/eye
    if(eyes[eye].animationName) {
        char buf[3]; // max two digits
        string topic = string("status/figure/");
        snprintf(buf, 3, "%d", eye);
        topic += buf;
        topic += "/animation/eye";
        mqttPublish(topic, *eyes[eye].animationName.get());
    }
}

void updateAnimationTimebaseMQTT(uint16_t speed) {
    // <device>/animation/speed
    char buf[6];
    string topic = string("status/animation/speed");
    snprintf(buf, 6, "%d", speed);
    mqttPublish(topic, buf);
}

int updateMatrixAnimation(int figure, const string &animationName){
    if ((figure<0) || (figure > NUMBER_OF_FIGURES)) {
        Serial.printf("USER: update matrix animation - figure out of range: %d\n", figure);
        return -1;
    }

    // don't update if we're already running this animation
    if(matrices[figure].animationName) { // only compare if it's been set! 
        if ( animationName == *matrices[figure].animationName.get()) {
            return 0;
        }
    } 

    AnimationChannelControllerDescriptor tempDescriptor;

    int result = createMatrixAnimation(animationName, figureStartLED(figure), 9,  tempDescriptor);
    if (-1 == result) {
        return -1;
    }
    // if we get this far, we've created the new animation to run for the given figure
    int channel = matrixAnimationChannel(figure);
    neoAnimator.StopAnimation(channel);
    matrices[figure].animationName = std::move(tempDescriptor.animationName);
    matrices[figure].duration = tempDescriptor.duration;
    matrices[figure].controller = std::move(tempDescriptor.controller);

    neoAnimator.StartAnimation(channel, matrices[figure].duration, *matrices[figure].controller.get());

    updateMatrixMQTT(figure);
    return 0;
}

int updateEyeAnimation(int eye, const string &animationName){
    if ((eye<0) || (eye > NUMBER_OF_FIGURES)) {
        Serial.printf("USER: update eye animation - eye out of range: %d\n", eye);
        return -1;
    }

    // don't update if we're already running this animation
    if(eyes[eye].animationName) {
        if ( animationName == *eyes[eye].animationName.get()) {
            return 0;
        }
    }

    AnimationChannelControllerDescriptor tempDescriptor;

    int result = createEyeAnimation(animationName, eyeStartLED(eye), 1,  tempDescriptor);
    if (-1 == result) {
        return -1;
    }
    // if we get this far, we've created the new animation to run for the given figure
    int channel = eyeAnimationChannel(eye);
    neoAnimator.StopAnimation(channel);
    eyes[eye].animationName = std::move(tempDescriptor.animationName);
    eyes[eye].duration = tempDescriptor.duration;
    eyes[eye].controller = std::move(tempDescriptor.controller);

    neoAnimator.StartAnimation(channel, eyes[eye].duration, *eyes[eye].controller.get());

    updateEyeMQTT(eye);
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
    for (int i =0; i < NUMBER_OF_FIGURES; i++) {
        const string blankName("blank");
        updateMatrixAnimation(i, blankName);
        updateEyeAnimation(i, blankName);
    }
    // calling updateAnimationMQTT() would be a redundant update to the figures & eyes - they're done in the loop above
    updateAnimationTimebaseMQTT(neoAnimator.getTimeScale());
    neoAnimator.StartAnimation(0, 5000, statusAnimation);    
    strip.Begin();
    strip.Show();
    h4.every(10, updateAndShowAnimitions);
}
