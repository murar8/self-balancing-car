#include <Arduino.h>
#include <assert.h>

#define SERIAL_FALLBACK_FREQUENCY 9600

#define BLINK_DELAY 500

void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp)
{
    if (!Serial)
    {
        Serial.begin(9600);
    }

    Serial.print(__file);
    Serial.print(':');
    Serial.print(__lineno, DEC);
    Serial.print(": In function \'");
    Serial.print(__func);
    Serial.print("\': Assertion failed: \'");
    Serial.print(__sexp);
    Serial.println('\'');

    Serial.flush();

    for (;;)
    {
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(BLINK_DELAY);
    }
}
