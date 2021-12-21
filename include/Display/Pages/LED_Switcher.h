#ifndef LED_Switcher_h
#define LED_Switcher_h

#include <Arduino.h>
#include "Display/Pages/Page.h"
#include "Adalight/Adalight.h"
#include "Util/Computer.h"

class LED_Switcher : public Page
{
public:
    LED_Switcher() {}
    LED_Switcher(Adafruit_ILI9341 *tft, Adalight *adalight, Computer *computer, uint8_t caller_page) : Page(tft, caller_page)
    {
        this->adalight = adalight;
        this->computer = computer;
    }
    // Draw
    void draw() override;
    void draw_internal();
    // Update
    uint16_t update(uint32_t ir) override;

private:
    Adalight *adalight;
    Computer *computer;
    bool in_inner_menu = false;
};

#endif