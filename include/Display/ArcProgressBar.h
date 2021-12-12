#ifndef ArcProgressBar_h
#define ArcProgressBar_h

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#include <tuple>
using namespace std;

class ArcProgressBar {
    public:
        ArcProgressBar();
        ArcProgressBar(
            Adafruit_ILI9341* tft,
            tuple<uint16_t, uint16_t> position,
            uint16_t radius,
            uint8_t thickness,
            uint16_t startAngle,
            uint16_t maxAngle,
            tuple<uint16_t, uint16_t> bounds,
            uint16_t foreground,
            uint16_t background
        );
        void update(uint16_t value, uint8_t steps);
        void update();
        
        int16_t angle_step;
        uint16_t target_angle;
        uint16_t lastAngle;
    private:
        Adafruit_ILI9341* tft;
        tuple<uint16_t, uint16_t> position, bounds;
        uint16_t radius;
        int8_t thickness;
        uint16_t startAngle, maxAngle;
        uint16_t fg, bg;

        uint16_t range; // Computed from bounds (Stored to save cpu time)   
        
        bool redraw = true;
};

#endif