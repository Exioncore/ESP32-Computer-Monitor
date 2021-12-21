#ifndef Monitor_1_h
#define Monitor_1_h

#include <Arduino.h>
#include "Display/Pages/Page.h"
#include "Display/ArcProgressBar.h"
#include "Display/Metric.h"
#include "Util/Computer.h"

class Monitor_1 : public Page
{
public:
    Monitor_1() {}
    Monitor_1(Adafruit_ILI9341 *tft, Computer *computer) : Page(tft) { this->computer = computer; }
    // Draw
    void draw() override;
    // Update
    uint16_t update(uint32_t ir) override;

private:
    bool idle_screen = true;
    Computer *computer;
    Metric metrics[15];
    ArcProgressBar arcProgressBar[2];
    unsigned long last_update = -1;
};

#endif