#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Credentials.h>

#define BUTTON_PIN 2

// https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFiClientSecure/examples/WiFiClientSecure/WiFiClientSecure.ino
// TODO USE PROPER CERTIFICATE
const char *test_root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n"
    "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n"
    "DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n"
    "PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n"
    "Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n"
    "AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n"
    "rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n"
    "OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n"
    "xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n"
    "7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n"
    "aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n"
    "HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n"
    "SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n"
    "ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n"
    "AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n"
    "R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n"
    "JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n"
    "Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n"
    "-----END CERTIFICATE-----\n";

WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);
char deviceId[36];

const int relay = 15;

int lastState = HIGH; // the previous state from the input pin
int currentState;     // the current reading from the input pin
bool on = false;
void connectToWiFi()
{
    Serial.print("Connecting to ");

    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }

    WiFi.begin(WIFI_SSID, WIFI_PW);
    wifiClient.setCACert(test_root_ca);
    Serial.println(WIFI_SSID);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println("Connected to wifi.");

    strncpy(deviceId, WiFi.macAddress().c_str(), 36);
    Serial.printf("Device online, id: |%s|\n", deviceId);
}

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Callback - ");
    Serial.print("Message:");

    if (!on)
    {
        digitalWrite(BUTTON_PIN, HIGH);
    }
    else
    {
        digitalWrite(BUTTON_PIN, LOW);
    }

    on = !on;

    // for (int i = 0; i < length; i++)
    // {
    //   // Serial.print((char)payload[i]);
    //   digitalWrite(BUTTON_PIN, HIGH);
    // }
}

void setupMQTT()
{
    mqttClient.setServer("b59f35eed5a04082a35c80b3230c7b20.s1.eu.hivemq.cloud", 8883);

    // set the callback function
    mqttClient.setCallback(mqttCallback);
}

void reconnectMQTT()
{
    Serial.println("Connecting to MQTT Broker...");
    while (!mqttClient.connected())
    {
        Serial.println("Reconnecting to MQTT Broker..");

        if (mqttClient.connect(deviceId, deviceId, "jessesjesseS1"))
        {
            mqttClient.subscribe("light");
            Serial.println("Connected.");
        }
        else
        {

            Serial.print("failed with state ");
            Serial.print(mqttClient.state());
            delay(2000);
        }
    }
}

void setup()
{
    Serial.begin(9600);

    connectToWiFi();
    setupMQTT();

    // pinMode(relay, OUTPUT);

    pinMode(BUTTON_PIN, OUTPUT);

    if (!mqttClient.connected())
        reconnectMQTT();

    mqttClient.publish("setup", deviceId);

    Serial.println("Setup finished");
}

void loop()
{
    if (!mqttClient.connected())
        reconnectMQTT();

    mqttClient.loop();

    currentState = digitalRead(BUTTON_PIN);

    Serial.println(currentState);

    // if (lastState == LOW && currentState == HIGH)
    // {
    //   mqttClient.publish("button", "LOW to HIGH");
    //   Serial.println("The state changed from LOW to HIGH");
    // }
    // else if (lastState == HIGH && currentState == LOW)
    // {
    //   mqttClient.publish("button", "HIGH to LOW");
    //   Serial.println("The state changed from HOW to LOW");
    // }

    // save the last state
    lastState = currentState;

    delay(50);
}
