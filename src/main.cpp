#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Ultrasonic.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <WiFi.h>
#include "WifiClient.h"
#include <PubSubClient.h>
#include "MQTTClient.h"


#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
Ultrasonic ultrasonic1(12, 13);
Adafruit_MPU6050 mpu;

WiFiClient     espClient;
PubSubClient   MQTT(espClient);

static long long pooling = 0;

void setup() {
  Serial.begin(9600);

  dht.begin();
  pinMode(DHTPIN, INPUT);
 
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  MQTTConnect(&MQTT);   
}

void loop() {      

  if(WiFi.status() == WL_CONNECTED)
  { 
    if(!MQTT.connected()) MQTTConfig(&MQTT);

    if(pooling > 1000000)
    {
      if(mpu.getMotionInterruptStatus()) { 
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        String response = String(dht.readTemperature()) + ':' +
                          String(dht.readHumidity())    + ':' +
                          String(ultrasonic1.read())    + ":" +
                          String(a.acceleration.x)      + "," +
                          String(a.acceleration.y);

        Serial.println(response);

        if (!isnan(dht.readTemperature()) && !isnan(dht.readHumidity()))
        {
          publish_data(&MQTT, topic_data, response);
        }
      }

      pooling = 0;
    }else pooling++;

    MQTT.loop(); 
  }else WIFIConnect();  

  // if (Serial.available()) {
  //   String cmd = Serial.readStringUntil('\n');
  //   Serial.println("leitura:" + cmd);
  //   cmd.trim(); 

  //   if (cmd == "data") {          
      
  //     if(mpu.getMotionInterruptStatus()) {
  //       /* Get new sensor events with the readings */
  //       sensors_event_t a, g, temp;
  //       mpu.getEvent(&a, &g, &temp);

  //       Serial.println(
  //         String(dht.readTemperature()) + ':' +
  //         String(dht.readHumidity())    + ':' +
  //         String(ultrasonic1.read())    + ":" +
  //         String(a.acceleration.x)      + "," +
  //         String(a.acceleration.y)
  //       );
  //     }

  //     delay(10);      
  //   }
  // }
}



