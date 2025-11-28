#include "MQTTClient.h"

const char* mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;

void MQTTConnect(PubSubClient *MQTT)
{
    MQTT->setServer(mqtt_broker, mqtt_port);   
    MQTT->setCallback(callback);

}

void MQTTConfig(PubSubClient *MQTT) 
{
    while(!MQTT->connected()) 
    {
        if (MQTT->connect(ID_MQTT))
        {
          Serial.println("Conectado ao Broker!");
          MQTT->subscribe(topic_data);
          MQTT->subscribe(topic_light);     
        } 
        else 
        {
          Serial.print("Falha na conexão. O status é: ");
          Serial.print(MQTT->state());      
        }
    }
}

void publish_data(PubSubClient *MQTT,const char *topic, String data)
{
  MQTT->publish(topic, data.c_str());
}

void callback(char *topic, byte *payload, unsigned int length) 
{
  if (String(topic) == topic_light) 
  { 
      String msg = "";

      for (unsigned int i = 0; i < length; i++) {
          msg += (char)payload[i];
      }    

      msg.trim();
      Serial.println("Light: " + msg);

      bool on_off = (msg == "True") ? true : false;
      //digitalWrite(LEDPIN, on_off ? HIGH : LOW);

  }
  
}