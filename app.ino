#include "LIS2MDLSensor.h"
#include "AZ3166WiFi.h"
#include "iothub_client_sample_mqtt.h"
#include <ArduinoJson.h>
#include <stdlib.h>

DevI2C *i2c;
LIS2MDLSensor *lis2mdl;
int axes[3];
int16_t raw[3];
uint8_t id;

int base_x;
int base_y;
int base_z;
int count = 0;
int expectedCount = 5;
bool initialized = false;
bool hasWifi = false;
bool preOpened = false;

void setup()
{
    Serial.begin(115200);
    initWifi();

    if (!hasWifi)
    {
        return;
    }

    i2c = new DevI2C(D14, D15);
    lis2mdl = new LIS2MDLSensor(*i2c);
    // init
    lis2mdl->init(NULL);

    iothubInit();
    Screen.print(0, "Initializing...");
}

void loop()
{
    if (!hasWifi)
    {
        delay(1000);
        return;
    }

    // read id
    lis2mdl->readId(&id);
    //Serial.printf("Id: %d\n", id);

    // getMAxes
    lis2mdl->getMAxes(axes);
    //Serial.printf("Axes: x - %d, y - %d, z - %d\n", axes[0], axes[1], axes[2]);

    char buffer[50];

    sprintf(buffer, "x:  %d", axes[0]);
    Screen.print(1, buffer);

    memset(buffer, 0, 50);
    sprintf(buffer, "y:  %d", axes[1]);
    Screen.print(2, buffer);

    memset(buffer, 0, 50);
    sprintf(buffer, "z:  %d", axes[2]);
    Screen.print(3, buffer);

    checkMagnetometerStatus();

    delay(1000);
}

void initWifi()
{
    Screen.print("IoT DevKit\r\n \r\nConnecting...\r\n");

    if (WiFi.begin() == WL_CONNECTED)
    {
        IPAddress ip = WiFi.localIP();
        Screen.print(1, ip.get_address());
        hasWifi = true;
        Screen.print(2, "Running...\r\n");
    }
    else 
    {
        Screen.print(1, "No Wi-Fi\r\n");
    }
}

void checkMagnetometerStatus()
{
    if (initialized)
    {
        char *message;
        int delta = 30;
        bool curOpened = false;
        if (abs(base_x - axes[0]) < delta && abs(base_y - axes[1]) < delta && abs(base_z - axes[2]) < delta)
        {
            Screen.print(0, "Door closed");
            message = "Door closed";
            curOpened = false;
        }
        else
        {
            Screen.print(0, "Door opened");
            message = "Door opened";
            curOpened = true;
        }
        //send message when status change
        if (curOpened != preOpened){
            iothubSendMessage((const unsigned char *)message);
            iothubLoop();
            preOpened = curOpened;
        }
    }
    else
    {
        if (base_x == 0 && base_y == 0 && base_z == 0)
        {
            base_x = axes[0];
            base_y = axes[1];
            base_z = axes[2];
        }
        else
        {
            int delta = 10;
            if (abs(base_x - axes[0]) < delta && abs(base_y - axes[1]) < delta && abs(base_z - axes[2]) < delta)
            {
                char buffer[50];
                sprintf(buffer, "Initialize %d", expectedCount - count);
                Screen.print(0, buffer);

                count++;
                if (count >= expectedCount)
                {
                    initialized = true;
                }
            }
            else
            {
                Screen.print(0, "Initializing...");
                count = 0;
                base_x = axes[0];
                base_y = axes[1];
                base_z = axes[2];
            }
        }
    }
}

/*void generateMessage(char* message)
{
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["deviceId"] = DEVICE_ID;
    root["x"] = axes[0];
    root["y"] = axes[1];
    root["z"] = axes[2];
    root.printTo(message, MESSAGE_MAX_LEN);
}*/