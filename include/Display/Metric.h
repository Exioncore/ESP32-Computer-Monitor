#ifndef Metric_h
#define Metric_h

#include <Arduino.h>
#include <Adafruit_ILI9341.h>

class Metric
{
public:
    Metric(){};
    Metric(
        Adafruit_ILI9341 *tft,
        uint16_t x,
        uint16_t y,
        uint8_t metricSize,
        const char *unit,
        uint8_t unitSize,
        uint16_t bck);
    void update(int value);
    void update(float value, uint8_t decimal_places);

private:
    void draw(char txt[10]);

    Adafruit_ILI9341 *tft;
    uint16_t x;
    uint16_t y;
    uint8_t metricSize;
    const char *unit;
    uint8_t unitSize;
    uint16_t bck;

    uint16_t u_w, u_h;
    uint16_t last_w;

    char old_txt[10];
};

#endif