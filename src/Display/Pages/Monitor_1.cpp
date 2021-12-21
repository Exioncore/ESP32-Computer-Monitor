#include "Display/Pages/Monitor_1.h"
#include "IRremoteCodes.h"
#include "Util/GFXHelper.h"
#include "Display/Image.h"

#define TIMEOUT 2500 // Timeout in ms

void Monitor_1::draw()
{
    tft->fillScreen(ILI9341_BLACK);
    if (millis() - computer->metrics.last_update >= TIMEOUT || !computer->metrics.initialized)
    {
        GFXHelper::drawImage(tft, 320 / 2 - ROG_RedImgSize[1] / 2, 25, ROG_RedImgSize, ROG_RedImgData, ILI9341_RED);
        GFXHelper::drawImage(tft, 320 / 2 - ROG_GreyImgSize[1] / 2, 25, ROG_GreyImgSize, ROG_GreyImgData, ILI9341_DARKGREY);
    }
    else
    {
        int16_t _x, _y;
        uint16_t _w, _h;
        tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        // CPU and GPU
        uint16_t r = 66;
        uint16_t bkg = tft->color565(50, 50, 50);
        arcProgressBar[0] = ArcProgressBar(tft, make_tuple<uint16_t, uint16_t>(20, 9), r, 13, 180, 360, make_tuple<uint16_t, uint16_t>(0, 100), ILI9341_RED, bkg);
        metrics[0] = Metric(tft, r + 20, 10 + r - 25, 2, "C", 1, ILI9341_BLACK);
        metrics[1] = Metric(tft, r + 20, 10 + r, 2, "MHz", 1, ILI9341_BLACK);
        metrics[2] = Metric(tft, r + 20, 10 + r + 25, 2, "W", 1, ILI9341_BLACK);
        metrics[3] = Metric(tft, 22, 10, 2, "%", 1, ILI9341_BLACK);
        metrics[4] = Metric(tft, 22, 10 + 2 * r, 2, "%", 1, ILI9341_BLACK);

        arcProgressBar[1] = ArcProgressBar(tft, make_tuple<uint16_t, uint16_t>(320 - 20 - 2 * r, 9), r, 13, 180, 360, make_tuple<uint16_t, uint16_t>(0, 100), bkg, ILI9341_DARKGREEN);
        metrics[5] = Metric(tft, 320 - 20 - r, 10 + r - 25, 2, "C", 1, ILI9341_BLACK);
        metrics[6] = Metric(tft, 320 - 20 - r, 10 + r, 2, "MHz", 1, ILI9341_BLACK);
        metrics[7] = Metric(tft, 320 - 20 - r, 10 + r + 25, 2, "W", 1, ILI9341_BLACK);
        metrics[8] = Metric(tft, 320 - 22, 10 + 2 * r, 2, "%", 1, ILI9341_BLACK);

        // FPS
        metrics[10] = Metric(tft, 165, 175, 3, "FPS", 1, ILI9341_BLACK);
        tft->drawRoundRect(110, 154, 100, 40, 5, ILI9341_DARKGREY);
        // Fan Controller
        //// Liquid
        tft->setTextSize(1);
        tft->getTextBounds("Liquid", 0, 0, &_x, &_y, &_w, &_h);
        tft->setCursor(40 - _w / 2, 230);
        tft->println("Liquid");
        metrics[11] = Metric(tft, 60 - _w / 2, 220, 2, "C", 1, ILI9341_BLACK);
        //// System Power
        tft->setTextSize(1);
        tft->getTextBounds("Power", 0, 0, &_x, &_y, &_w, &_h);
        tft->setCursor(165 - _w / 2, 230);
        tft->println("Power");
        metrics[12] = Metric(tft, 180 - _w / 2, 220, 2, "W", 1, ILI9341_BLACK);
        //// Ambient
        tft->setTextSize(1);
        tft->getTextBounds("Ambient", 0, 0, &_x, &_y, &_w, &_h);
        tft->setCursor(280 - _w / 2, 230);
        tft->println("Ambient");
        metrics[13] = Metric(tft, 300 - _w / 2, 220, 2, "C", 1, ILI9341_BLACK);

        //// Display Refresh Time
        metrics[14] = Metric(tft, 160, 6, 1, "ms", 1, ILI9341_BLACK);

        update(UINT32_MAX);
    }
}

uint16_t Monitor_1::update(uint32_t ir)
{
    switch (ir)
    {
    case IR_BUTTON_1:
        return 1; // Request switch to Audio_Switcher Page
    case IR_BUTTON_2:
        return 2; // Request switch to LED_Switcher Page
    case IR_BUTTON_RIGHT:
        return 3; // Request Main Monitor Page
    default:
        break;
    }

    if (!idle_screen)
    {
        if (computer->metrics.initialized)
        {
            unsigned long t = millis();
            if (t - computer->metrics.last_update < TIMEOUT)
            {
                if (computer->metrics.last_update != last_update)
                {
                    tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);

                    // Cpu Stats
                    arcProgressBar[0].update((int)(computer->metrics.cpu.load.get()), 15);
                    metrics[0].update((int)(computer->metrics.cpu.temperature.get()));
                    metrics[1].update((int)(computer->metrics.cpu.clock.get()));
                    metrics[2].update((int)(computer->metrics.cpu.power.get()));
                    metrics[3].update((int)(computer->metrics.cpu.maxThreadLoad.get()));
                    metrics[4].update((int)(computer->metrics.cpu.load.get()));

                    // Gpu Stats
                    arcProgressBar[1].update(100 - (int)(computer->metrics.gpu.coreLoad.get()), 15);
                    metrics[5].update((int)(computer->metrics.gpu.temperature.get()));
                    metrics[6].update((int)(computer->metrics.gpu.coreClock.get()));
                    metrics[7].update((int)(computer->metrics.gpu.power.get()));
                    metrics[8].update((int)(computer->metrics.gpu.coreLoad.get()));

                    // FrameRate
                    metrics[10].update(computer->metrics.framerate.get() > 999 ? 999 : (int)(computer->metrics.framerate.get()));

                    // Liquid
                    metrics[11].update(computer->metrics.fanController.liquidTemperature.get(), 1);

                    // System Power
                    uint16_t sys_power = (uint16_t)round(computer->metrics.cpu.power.get() + computer->metrics.gpu.power.get());
                    metrics[12].update(sys_power);

                    // Ambient
                    metrics[13].update(computer->metrics.fanController.ambientTemperature.get(), 1);

                    // Display Refresh Time
                    metrics[14].update(millis() - t);

                    last_update = computer->metrics.last_update;
                }
                else
                {
                    arcProgressBar[0].update();
                    arcProgressBar[1].update();
                }
            }
            else
            {
                idle_screen = true;
                last_update = computer->metrics.last_update;
                draw();
            }
        }
    }
    else
    {
        if (computer->metrics.initialized && computer->metrics.last_update != last_update)
        {
            idle_screen = false;
            draw();
            return update(ir);
        }
    }

    return UINT16_MAX;
}