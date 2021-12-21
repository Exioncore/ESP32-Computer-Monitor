#include <SPI.h>
#include "Display/Display.h"

#define BCK ILI9341_BLACK //ILI9341_BLACK
/**
 * Constructor
 */
Display::Display()
{
    page = nullptr;
}

/**
 * Initialize display 
 */
void Display::init(int8_t cs, int8_t dc, int8_t mosi, int8_t sck, int8_t rst, int8_t miso)
{
    tft = new Adafruit_ILI9341(cs, dc, rst); // mosi & miso & sck not passed in order to ensure usage of hardware spi
    // Initialize tft screen
    tft->begin(80000000); // 80 MHz
    tft->fillScreen(BCK);
    tft->setRotation(1);
}

// Draw
void Display::setPage(Page *page)
{
    this->page = page;
    tft->fillScreen(BCK);
    page->draw();
}
// Update
uint16_t Display::update(uint32_t ir)
{
    if (page != nullptr)
    {
        if (ir != UINT32_MAX)
        {
            // Valid IR code immediately process
            return page->update(ir);
        }
        else
        {
            // Limit screen framerate when no commands are present
            if (millis() - t >= 33)
            {
                t = millis();
                return page->update(ir);
            }
        }
    }
    return UINT16_MAX;
}