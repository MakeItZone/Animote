#include<H4Plugins.h>
#include "secrets.h"

// necessary forward declarations
void onMQTTConnect();
void onMQTTDisconnect();

H4_USE_PLUGINS(115200,H4_Q_CAPACITY,false) // Serial baud rate, Q size, SerialCmd autostop
H4P_VerboseMessages h4vm; // enable text messages instead of error coders
H4P_WiFi h4wifi(SSID,SSIDPSK,"testbed");

//H4P_AsyncMQTT h4mqtt("192.168.1.4",1883,"","",onMQTTConnect,onMQTTDisconnect,myLWT); // Custom lwt
H4P_AsyncMQTT h4mqtt("192.168.1.4",1883,"","",onMQTTConnect,onMQTTDisconnect); // NO Custom lwt

uint32_t myMqttCallback(vector<string> vs){
  Serial.printf("USER: Msg received with payload=%s\n",H4PAYLOAD.c_str()); // convert payload to C-style string
  if(H4PAYLOAD=="good") {
      h4mqtt.publishDevice("goodpayload","Thank You!");
      return H4_CMD_OK;
  }
  else {
      h4mqtt.publishDevice("badpayload",H4PAYLOAD);
      return H4_CMD_PAYLOAD_FORMAT;
  }
}

void onMQTTConnect(){
    Serial.print("USER: MQTT connected\n");
    h4mqtt.subscribeDevice("mytopic",myMqttCallback);
}

void onMQTTDisconnect(){
    Serial.print("USER: MQTT Disconnected\n");
}
