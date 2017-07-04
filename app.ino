#include "LIS2MDLSensor.h"
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

void setup()
{
    i2c = new DevI2C(D14, D15);
    lis2mdl = new LIS2MDLSensor(*i2c);
    // init
    lis2mdl->init(NULL);
    Screen.print(0, "Initializing...");
}

void loop()
{
    // read id
    lis2mdl->readId(&id);
    Serial.printf("Id: %d\n", id);
    // getMAxes
    lis2mdl->getMAxes(axes);
    Serial.printf("Axes: x - %d, y - %d, z - %d\n", axes[0], axes[1], axes[2]);

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

void checkMagnetometerStatus()
{
    if (initialized)
    {
        int delta = 30;
        if (abs(base_x - axes[0]) < delta && abs(base_y - axes[1]) < delta && abs(base_z - axes[2]) < delta)
        {
            Screen.print(0, "Door closed");
        }
        else
        {
            Screen.print(0, "Door opened");
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
                count = 0;
                base_x = axes[0];
                base_y = axes[1];
                base_z = axes[2];
            }
        }
    }
}