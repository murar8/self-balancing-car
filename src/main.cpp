#include "communication.h"
#include <Arduino.h>
#include <assert.h>

CommunicationHandler<16> handler;

char data[64] = "qwewrtyuiop";

char *onGetData()
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    return data;
}

bool onSetData(char *d)
{
    strcpy(data, d);
    return false;
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    handler.begin(&Serial);
    handler.attachProperty("t", onGetData, onSetData);
}

void loop()
{
    handler.tick();
}
