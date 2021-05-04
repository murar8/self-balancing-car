#include <Arduino.h>
#include <assert.h>

static const uint32_t SERIAL_FALLBACK_FREQUENCY = 9600;
static const uint32_t BLINK_DELAY = 500;

void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp)
{
    if (!Serial)
    {
        Serial.begin(SERIAL_FALLBACK_FREQUENCY);
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
