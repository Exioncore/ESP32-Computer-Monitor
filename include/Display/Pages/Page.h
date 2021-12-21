#ifndef Page_h
#define Page_h

#include <Arduino.h>
#include <Adafruit_ILI9341.h>

class Page
{
public:
    Page() {}
    Page(Adafruit_ILI9341 *tft) { this->tft = tft; }
    Page(Adafruit_ILI9341 *tft, uint8_t caller_page)
    {
        this->tft = tft;
        this->caller_page = caller_page;
    }
    // Draw
    virtual void draw() = 0;
    // Update
    virtual uint16_t update(uint32_t ir) = 0;
    void set_caller_page(uint8_t page) { this->caller_page = page; }

protected:
    Adafruit_ILI9341 *tft;
    uint8_t caller_page = 0;
};

#endif