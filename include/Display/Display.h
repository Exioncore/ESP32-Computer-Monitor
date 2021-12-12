#ifndef Display_h
#define Display_h

#include <Arduino.h>
#include <Display/Pages/Page.h>
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

class Display {
    public:
        // Constuctor
        Display();

        // Initialize
        void init(int8_t cs, int8_t dc, int8_t mosi, int8_t sck, int8_t rst, int8_t miso);
        // Draw
        void setPage(Page* page);
        // Update
        uint16_t update(uint32_t ir);
        
        // Getters
        Adafruit_ILI9341* getTFT() { return tft; }

    private:
        Adafruit_ILI9341* tft;
        Page* page;
        unsigned long t = 0;
};

#endif