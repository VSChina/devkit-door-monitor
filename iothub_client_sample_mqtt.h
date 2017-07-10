#ifndef IOTHUB_CLIENT_SAMPLE_MQTT_H
#define IOTHUB_CLIENT_SAMPLE_MQTT_H

void iothubInit(void);
void iothubSendMessage(const unsigned char *);
void iothubLoop(void);
void iothubClose(void);

#endif