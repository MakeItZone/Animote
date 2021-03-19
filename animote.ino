#include <H4Plugins.h>

#include "network.hh"
#include "animation.hh"
#include "time.hh"
#include "status.hh"
#include "secrets.h"

// NodeMCU has *2* LEDs(!)
// This sketch should be configured so that H4 uses the LED on the ESP8266 module
// And we use the LED connected to pin 16 on the main board for indicating our app's status
// See https://stackoverflow.com/questions/14894698/why-does-extern-const-int-n-not-work-as-expected
// for why this 'extern' is still a declaration and is valid
extern int const APP_STATUS_LED_PIN = 16;

// ESP8266 Flash button on GPIO0 
const int BUTTON_PIN = 0;

// Setup H4
H4_USE_PLUGINS(115200,H4_Q_CAPACITY,false) // Serial baud rate, Q size, SerialCmd autostop
H4P_VerboseMessages h4vm; // enable text messages instead of error coders
H4P_WiFi h4wifi(SSID,SSIDPSK,"", onWiFiConnect, onWiFiDisconnect);
H4P_AsyncMQTT h4mqtt(MQTTSERVER,1883,"","",onMqttConnect,onMqttDisconnect);
H4P_FlasherController h4fc; // for app status
H4P_MultiFunctionButton mfnb(BUTTON_PIN, INPUT_PULLUP, ACTIVE_LOW, 15); // pin, mode, sense, debounce (ms)
H4P_Heartbeat h4hb; // add up-time to web ui
// see https://github.com/philbowles/h4plugins/blob/master/dead/h4tk.md
// note: the DST adjustment seems to not be correct/working
H4P_Timekeeper h4tk("time.google.com","time1.google.com", -8*60, &H4P_Timekeeper::DST_USA); // -28800 = -8hrs for PST
//H4P_RemoteUpdate h4ru(); // remote update... TODO. See: dead/h4ru.md

// For debugging
//
// Heap Monitoring
// https://github.com/philbowles/h4plugins/blob/master/docs/h4hw.md
// example: https://github.com/philbowles/h4plugins/blob/master/examples/DIAGNOSTICS/H4P_HeapWarn/H4P_HeapWarn.ino
// 
// Queue Monitoring
// https://github.com/philbowles/h4plugins/blob/master/docs/h4qw.md
// example: https://github.com/philbowles/h4plugins/blob/master/examples/DIAGNOSTICS/H4P_QueueWarn/H4P_QueueWarn.ino
//
// Event Logging
// https://github.com/philbowles/h4plugins/blob/master/docs/events.md
// especially Serial Logger: https://github.com/philbowles/h4plugins/blob/master/docs/slog.md
// and  MQTT log destination: https://github.com/philbowles/h4plugins/blob/master/docs/mlog.md


void h4setup(){ // do the same type of thing as the standard setup
    pinMode(APP_STATUS_LED_PIN, OUTPUT);
    updateStatus(Status::boot);
    Serial.printf("USER: Attempting to connect to AP: %s\n", SSID.c_str());
    animationSetup();
}

