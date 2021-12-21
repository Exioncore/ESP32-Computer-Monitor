#ifndef Audio_Switcher_h
#define Audio_Switcher_h

#include <Arduino.h>
#include "Display/Pages/Page.h"
#include "Util/Computer.h"

class Audio_Switcher : public Page
{
public:
    Audio_Switcher() {}
    Audio_Switcher(Adafruit_ILI9341 *tft, Computer *computer, uint8_t caller_page) : Page(tft, caller_page) { this->computer = computer; }
    // Draw
    void draw() override;
    // Update
    uint16_t update(uint32_t ir) override;

private:
    Computer *computer;
};

#endif