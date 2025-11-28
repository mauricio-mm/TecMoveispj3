#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include "utils.h"

#define ID_MQTT "esp_iot"
#define topic_data   "lab318/data"
#define topic_light  "lab318/light"

void MQTTConnect (PubSubClient *MQTT);
void MQTTConfig  (PubSubClient *MQTT);
void publish_data(PubSubClient *MQTT, const char *topic, String data);
void callback    (char *topic, byte *payload, unsigned int length);

#endif