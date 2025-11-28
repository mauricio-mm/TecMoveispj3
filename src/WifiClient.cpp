#include "WifiClient.h"

const char* ssid_STA = "POCOX6Pro";    
const char* password_STA = "esppucrs";

bool config_SPA();

void WIFIConnect()
{
    Serial.println("Inicializando Wi-Fi em modo dual (STA + AP)...");
    WiFi.mode(WIFI_STA);

    config_SPA();
}

bool config_SPA()
{
    Serial.print("Conectando a rede Wi-Fi ");
    Serial.println(ssid_STA);
    WiFi.begin(ssid_STA, password_STA);

    int tentativas = 0;
    while (WiFi.status() != WL_CONNECTED && tentativas < 15)
    {
        delay(1000);
        Serial.print(".");
        tentativas++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\nConectado ao Wi-Fi!");
        Serial.print("IP local STA: ");
        Serial.println(WiFi.localIP());
        return true;
    }
    else Serial.println("\nFalha ao conectar a rede Wi-Fi STA.");

    return false;
}