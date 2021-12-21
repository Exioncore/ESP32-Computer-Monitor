#ifndef Monitor_2_h
#define Monitor_2_h

#include <Arduino.h>
#include "Display/Pages/Page.h"
#include "Display/Metric.h"
#include "Util/Computer.h"

class Monitor_2 : public Page
{
public:
    Monitor_2() {}
    Monitor_2(Adafruit_ILI9341 *tft, Computer *computer) : Page(tft) { this->computer = computer; }
    // Draw
    void draw() override;
    // Update
    uint16_t update(uint32_t ir) override;

private:
    void draw_metrics();
    void reset_stats();

    bool initialized = false;
    uint16_t m_height;
    Computer *computer;
    Metric metrics[15 * 4];
    unsigned long last_update = -1;
};

#endif