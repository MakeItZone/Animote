// ONLY INCLUDE H4Plugins.h in the main .ino file! (You'll get linker errors.)
#include <H4P_WiFi.h>
#include <H4P_AsyncMQTT.h>

#include "network.hh"
#include "status.hh"
#include "animation_manager.hh"
#include "secrets.h"

using namespace std;

extern H4P_WiFi h4wifi;
extern H4P_AsyncMQTT h4mqtt;

void onWiFiConnect(void)
{
    // do things on WiFi connect...
    // like reconnect to MQTT (not clear if MQTT auto-reconnect includes wifi failure)
    Serial.println("USER: WiFi connected!");
    Serial.printf("USER: Connected SSID is %s\n", WiFi.SSID().c_str());
    Serial.printf("USER: IP is %s\n", WiFi.localIP().toString().c_str());
    updateStatus(Status::running);
}

void onWiFiDisconnect(void)
{
    // do things on WiFi disconnect...
    // like shut down mqtt connections?
    Serial.println("USER: WiFi dis-connected!");
    updateStatus(Status::disconnected);
}

// Note, H4P has some utility functions for retrieving integers from topics and payloads
// (int PARAM(n), and int H4PAYLOAD_INT - see H4P_MQTT_Wildcards.ino in the examples for details.)
uint32_t myMqttCallback(vector<string> vs)
{
    Serial.printf("USER: Msg received with %d parts and payload=%s\n", vs.size(), H4PAYLOAD.c_str()); // convert payload to C-style string
    dumpvs(vs);

    if (vs.size() == 2) {
        if (vs[0]=="speed") {
            updateAnimationTimebase(PARAM_INT(1));
            return H4_CMD_OK;
        }
        return H4_CMD_PAYLOAD_FORMAT;
    }
    else if (vs.size() == 4) // topic parts below the subscription + payload
    {
            int figure = PARAM_INT(0);
            string category = vs[1];
            if (category == "animation")
            {
                string item = vs[2];
                if (item == "eye")
                {
                    // update eye animation flags
                    int ret = updateEyeAnimation(figure, H4PAYLOAD);
                    if (ret >= 0) {
                        return H4_CMD_OK;
                    } else {
                        Serial.println("Bad payload!");
                        return H4_CMD_PAYLOAD_FORMAT;
                    }
                }
                else if (item == "matrix")
                {
                    // update matrix animation flags
                    int ret = updateMatrixAnimation(figure, H4PAYLOAD);
                    if (ret >= 0) {
                        return H4_CMD_OK;
                    } else {
                        Serial.println("Bad payload!");
                        return H4_CMD_PAYLOAD_FORMAT;
                    }
                }
            }
    }

    Serial.printf("USER: bad payload!\n");
    return H4_CMD_PAYLOAD_FORMAT;
}

void onMqttConnect()
{
    Serial.print("USER: MQTT connected\n");
    // desired topics are:
    // <device>/figure/<number>/animation/eye
    // <device>/figure/<number>/animation/matrix
    h4mqtt.subscribeDevice("animation", myMqttCallback);
    h4mqtt.subscribeDevice("animation/speed", myMqttCallback);
    h4mqtt.subscribeDevice("figure/#", myMqttCallback);
    updateAnimationMQTT();
}

void onMqttDisconnect()
{
    Serial.print("USER: MQTT disconnected\n");
    h4mqtt.unsubscribeDevice("animation/speed");
    h4mqtt.unsubscribeDevice("animation");
    h4mqtt.unsubscribeDevice("figure/#");
}

void mqttPublish(string topic, string payload) {
    h4mqtt.publishDevice(topic, payload);
}