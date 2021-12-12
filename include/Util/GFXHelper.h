#ifndef GFXHelper_h
#define GFXHelper_h

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

class GFXHelper {
    public:
        static void drawImage(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, const uint16_t details[], const byte data[], uint16_t fg);
        static void fillArc(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t start_angle, uint16_t seg_count, uint16_t rx, uint16_t ry, int w, uint16_t color);
        static void writeTxtCentered(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, char* txt, uint8_t size);
        static void writeMetricWithUnit(
            Adafruit_ILI9341* tft,
            uint16_t x, uint16_t y,
            char* metric, uint8_t metricSize,
            const char* unit, uint8_t unitSize);
    private:
};

#endif