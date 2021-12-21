#include "Display/Pages/Audio_Switcher.h"
#include "IRremoteCodes.h"
#include "Util/GFXHelper.h"

void Audio_Switcher::draw()
{
    int i;
    int16_t _x, _y;
    uint16_t _w, _h;
    tft->fillScreen(ILI9341_BLACK);
    tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft->setTextSize(2);
    tft->getTextBounds("Back", 0, 0, &_x, &_y, &_w, &_h);
    uint16_t back_w = _w;
    uint16_t y_center = 240 / 2;
    uint16_t t_height = _h + 8;
    uint16_t y_start = y_center - t_height * (computer->audio.presets.size() / 2.0);
    // Title
    tft->getTextBounds("Select Audio Preset", 0, 0, &_x, &_y, &_w, &_h);
    tft->setCursor(160 - _w / 2, 0);
    tft->println("Select Audio Preset");
    // Draw Presets
    uint16_t min_x = back_w;
    for (i = 0; i < computer->audio.presets.size(); i++)
    {
        tft->getTextBounds(computer->audio.presets[i], 0, 0, &_x, &_y, &_w, &_h);
        min_x = min_x < _w ? _w : min_x;
        tft->setCursor(160 - _w / 2, y_start + t_height * i);
        tft->println(computer->audio.presets[i]);
    }
    min_x = 160 - (min_x / 2);
    // Back Button
    tft->setCursor(160 - back_w / 2, 240 - t_height);
    tft->println("Back");
    // Draw Indeces
    char txt[4];
    for (i = 0; i < computer->audio.presets.size(); i++)
    {
        tft->setCursor(min_x - 50, y_start + t_height * i);
        sprintf(txt, "%d.", i + 1);
        tft->println(txt);
    }
    tft->setCursor(min_x - 50, 240 - t_height);
    tft->println("0.");
}

uint16_t Audio_Switcher::update(uint32_t ir)
{
    switch (ir)
    {
    case IR_BUTTON_0:
        return caller_page; // Request switch to Monitor Page
    case IR_BUTTON_1:
        Serial.println("AE0");
        return caller_page; // Request switch to Monitor Page
    case IR_BUTTON_2:
        Serial.println("AE1");
        return caller_page; // Request switch to Monitor Page
    case IR_BUTTON_3:
        Serial.println("AE2");
        return caller_page; // Request switch to Monitor Page
    case IR_BUTTON_4:
        Serial.println("AE3");
        return caller_page; // Request switch to Monitor Page
    case IR_BUTTON_5:
        Serial.println("AE4");
        return caller_page; // Request switch to Monitor Page
    default:
        break;
    }
    return UINT16_MAX;
}