#include "Display/Pages/Monitor_2.h"
#include "IRremoteCodes.h"
#include "Util/GFXHelper.h"

#define TIMEOUT 2500 // Timeout in ms

void Monitor_2::draw()
{
    tft->fillScreen(ILI9341_BLACK);
    int16_t _x, _y;
    uint16_t _w, _h;
    tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);

    // General
    tft->setTextSize(2);
    tft->getTextBounds("T", 0, 0, &_x, &_y, &_w, &_h);
    uint16_t y_curr = 0;
    m_height = _h;

    // CPU name
    tft->setCursor(0, y_curr);
    tft->setTextSize(2);
    tft->setTextColor(ILI9341_RED);
    if (computer->metrics.initialized)
    {
        tft->print(computer->metrics.cpu.name);
        initialized = true;
    }
    else
    {
        tft->print("CPU");
        initialized = false;
    }
    tft->setTextColor(ILI9341_WHITE);
    y_curr += m_height * 2 - 4;
    // CPU Clock
    metrics[0] = Metric(tft, 30, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[1] = Metric(tft, 90, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[2] = Metric(tft, 150, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[3] = Metric(tft, 280, y_curr, 2, "MHz", 1, ILI9341_BLACK);
    y_curr += m_height + 2;
    // CPU Temperature
    metrics[4] = Metric(tft, 30, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[5] = Metric(tft, 90, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[6] = Metric(tft, 150, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[7] = Metric(tft, 280, y_curr, 2, "C", 1, ILI9341_BLACK);
    y_curr += m_height + 2;
    // CPU Load
    metrics[8] = Metric(tft, 30, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[9] = Metric(tft, 90, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[10] = Metric(tft, 150, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[11] = Metric(tft, 280, y_curr, 2, "%", 1, ILI9341_BLACK);
    y_curr += m_height + 2;
    // CPU Max Thread Load
    metrics[12] = Metric(tft, 30, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[13] = Metric(tft, 90, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[14] = Metric(tft, 150, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[15] = Metric(tft, 280, y_curr, 2, "%", 1, ILI9341_BLACK);
    y_curr += m_height + 2;
    // CPU Power
    metrics[16] = Metric(tft, 30, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[17] = Metric(tft, 90, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[18] = Metric(tft, 150, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[19] = Metric(tft, 280, y_curr, 2, "W", 1, ILI9341_BLACK);
    y_curr += m_height - 2;

    // GPU name
    tft->setCursor(0, y_curr);
    tft->setTextSize(2);
    tft->setTextColor(ILI9341_DARKGREEN);
    if (computer->metrics.initialized)
        tft->print(computer->metrics.gpu.name);
    else
        tft->print("GPU");
    tft->setTextColor(ILI9341_WHITE);
    y_curr += m_height * 2 - 4;
    // GPU Clock
    metrics[20] = Metric(tft, 30, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[21] = Metric(tft, 90, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[22] = Metric(tft, 150, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[23] = Metric(tft, 280, y_curr, 2, "MHz", 1, ILI9341_BLACK);
    y_curr += m_height + 2;
    // GPU Temperature
    metrics[24] = Metric(tft, 30, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[25] = Metric(tft, 90, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[26] = Metric(tft, 150, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[27] = Metric(tft, 280, y_curr, 2, "C", 1, ILI9341_BLACK);
    y_curr += m_height + 2;
    // GPU Load
    metrics[28] = Metric(tft, 30, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[29] = Metric(tft, 90, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[30] = Metric(tft, 150, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[31] = Metric(tft, 280, y_curr, 2, "%", 1, ILI9341_BLACK);
    y_curr += m_height + 2;
    // GPU Power
    metrics[32] = Metric(tft, 30, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[33] = Metric(tft, 90, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[34] = Metric(tft, 150, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[35] = Metric(tft, 280, y_curr, 2, "W", 1, ILI9341_BLACK);
    y_curr += m_height + 2;
    // GPU VRAM
    metrics[36] = Metric(tft, 30, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[37] = Metric(tft, 90, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[38] = Metric(tft, 150, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[39] = Metric(tft, 280, y_curr, 2, "MB", 1, ILI9341_BLACK);
    y_curr += m_height + 2;
    // FPS
    metrics[40] = Metric(tft, 30, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[41] = Metric(tft, 90, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[42] = Metric(tft, 150, y_curr, 2, "", 1, ILI9341_BLACK);
    metrics[43] = Metric(tft, 280, y_curr, 2, "FPS", 1, ILI9341_BLACK);
    y_curr += m_height;

    draw_metrics();
}

uint16_t Monitor_2::update(uint32_t ir)
{
    switch (ir)
    {
    case IR_BUTTON_0:
        reset_stats();
        break;
    case IR_BUTTON_1:
        return 1; // Request switch to Audio_Switcher Page
    case IR_BUTTON_2:
        return 2; // Request switch to LED_Switcher Page
    case IR_BUTTON_LEFT:
        return 0; // Request Main Monitor Page
    default:
        break;
    }

    if (computer->metrics.initialized)
    {
        if (!initialized)
        {
            tft->setTextSize(2);
            tft->setTextColor(ILI9341_RED);
            tft->setCursor(0, 0);
            tft->fillRect(0, 0, 100, m_height, ILI9341_BLACK);
            tft->print(computer->metrics.cpu.name);
            tft->setTextColor(ILI9341_DARKGREEN);
            tft->setCursor(0, m_height * 2 - 4 + (m_height + 2) * 4 + m_height - 2);
            tft->fillRect(0, m_height * 2 - 4 + (m_height + 2) * 4 + m_height - 2, 100, m_height, ILI9341_BLACK);
            tft->print(computer->metrics.gpu.name);
            tft->setTextColor(ILI9341_WHITE);
            initialized = true;
        }
        unsigned long t = millis();
        if (t - computer->metrics.last_update < TIMEOUT)
        {
            if (computer->metrics.last_update != last_update)
            {
                draw_metrics();
                last_update = computer->metrics.last_update;
            }
        }
    }

    return UINT16_MAX;
}

void Monitor_2::draw_metrics()
{
    tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);

    // CPU Clock
    metrics[0].update((int)(computer->metrics.cpu.clock.get_min()));
    metrics[1].update((int)(computer->metrics.cpu.clock.get_avg()));
    metrics[2].update((int)(computer->metrics.cpu.clock.get_max()));
    metrics[3].update((int)(computer->metrics.cpu.clock.get()));
    // CPU Temperature
    metrics[4].update((int)(computer->metrics.cpu.temperature.get_min()));
    metrics[5].update((int)(computer->metrics.cpu.temperature.get_avg()));
    metrics[6].update((int)(computer->metrics.cpu.temperature.get_max()));
    metrics[7].update((int)(computer->metrics.cpu.temperature.get()));
    // CPU Load
    metrics[8].update((int)(computer->metrics.cpu.load.get_min()));
    metrics[9].update((int)(computer->metrics.cpu.load.get_avg()));
    metrics[10].update((int)(computer->metrics.cpu.load.get_max()));
    metrics[11].update((int)(computer->metrics.cpu.load.get()));
    // CPU Max Thread Load
    metrics[12].update((int)(computer->metrics.cpu.maxThreadLoad.get_min()));
    metrics[13].update((int)(computer->metrics.cpu.maxThreadLoad.get_avg()));
    metrics[14].update((int)(computer->metrics.cpu.maxThreadLoad.get_max()));
    metrics[15].update((int)(computer->metrics.cpu.maxThreadLoad.get()));
    // CPU Power
    metrics[16].update((int)(computer->metrics.cpu.power.get_min()));
    metrics[17].update((int)(computer->metrics.cpu.power.get_avg()));
    metrics[18].update((int)(computer->metrics.cpu.power.get_max()));
    metrics[19].update((int)(computer->metrics.cpu.power.get()));

    // GPU Clock
    metrics[20].update((int)(computer->metrics.gpu.coreClock.get_min()));
    metrics[21].update((int)(computer->metrics.gpu.coreClock.get_avg()));
    metrics[22].update((int)(computer->metrics.gpu.coreClock.get_max()));
    metrics[23].update((int)(computer->metrics.gpu.coreClock.get()));
    // GPU Temperature
    metrics[24].update((int)(computer->metrics.gpu.temperature.get_min()));
    metrics[25].update((int)(computer->metrics.gpu.temperature.get_avg()));
    metrics[26].update((int)(computer->metrics.gpu.temperature.get_max()));
    metrics[27].update((int)(computer->metrics.gpu.temperature.get()));
    // GPU Load
    metrics[28].update((int)(computer->metrics.gpu.coreLoad.get_min()));
    metrics[29].update((int)(computer->metrics.gpu.coreLoad.get_avg()));
    metrics[30].update((int)(computer->metrics.gpu.coreLoad.get_max()));
    metrics[31].update((int)(computer->metrics.gpu.coreLoad.get()));
    // GPU Power
    metrics[32].update((int)(computer->metrics.gpu.power.get_min()));
    metrics[33].update((int)(computer->metrics.gpu.power.get_avg()));
    metrics[34].update((int)(computer->metrics.gpu.power.get_max()));
    metrics[35].update((int)(computer->metrics.gpu.power.get()));
    // GPU Memory
    metrics[36].update((int)(computer->metrics.gpu.memUsage.get_min()));
    metrics[37].update((int)(computer->metrics.gpu.memUsage.get_avg()));
    metrics[38].update((int)(computer->metrics.gpu.memUsage.get_max()));
    metrics[39].update((int)(computer->metrics.gpu.memUsage.get()));

    // FPS
    metrics[40].update((int)(computer->metrics.framerate.get_min()));
    metrics[41].update((int)(computer->metrics.framerate.get_avg()));
    metrics[42].update((int)(computer->metrics.framerate.get_max()));
    metrics[43].update((int)(computer->metrics.framerate.get()));
}

void Monitor_2::reset_stats()
{
    computer->metrics.cpu.clock.reset();
    computer->metrics.cpu.load.reset();
    computer->metrics.cpu.maxThreadLoad.reset();
    computer->metrics.cpu.power.reset();
    computer->metrics.cpu.temperature.reset();
    computer->metrics.gpu.coreClock.reset();
    computer->metrics.gpu.coreLoad.reset();
    computer->metrics.gpu.memClock.reset();
    computer->metrics.gpu.memUsage.reset();
    computer->metrics.gpu.power.reset();
    computer->metrics.gpu.temperature.reset();
    computer->metrics.framerate.reset();
    computer->metrics.fanController.ambientTemperature.reset();
    computer->metrics.fanController.liquidTemperature.reset();
}