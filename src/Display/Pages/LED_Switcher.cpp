#include "Display/Pages/LED_Switcher.h"
#include "IRremoteCodes.h"
#include "Util/GFXHelper.h"

void LED_Switcher::draw()
{
    int16_t _x, _y;
    uint16_t _w, _h;
    tft->fillScreen(ILI9341_BLACK);
    tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft->setTextSize(2);
    tft->getTextBounds("Back", 0, 0, &_x, &_y, &_w, &_h);
    uint16_t back_w = _w;
    // Title
    tft->getTextBounds("Select LED mode", 0, 0, &_x, &_y, &_w, &_h);
    tft->setCursor(160 - _w / 2, 0);
    tft->println("Select LED mode");
    // Variables
    uint16_t y_center = 240 / 2;
    uint16_t t_height = _h + 8;
    uint16_t y_start = y_center - t_height * (4 / 2.0);
    // Draw Modes
    char modes[4][13] = {{'A', 'd', 'a', 'l', 'i', 'g', 'h', 't', '\0'}, {'A', 'd', 'a', 'l', 'i', 'g', 'h', 't', ' ', 'R', 'G', 'B', '\0'}, {'M', 'o', 'o', 'd', ' ', 'l', 'i', 'g', 'h', 't', '\0'}, {'O', 'f', 'f', '\0'}};
    uint16_t min_x = back_w;
    for (int i = 0; i < 4; i++)
    {
        tft->getTextBounds(modes[i], 0, 0, &_x, &_y, &_w, &_h);
        min_x = min_x < _w ? _w : min_x;
        tft->setCursor(160 - _w / 2, y_start + t_height * i);
        tft->println(modes[i]);
    }
    min_x = 160 - (min_x / 2);
    // Back Button
    tft->setCursor(160 - back_w / 2, 240 - t_height);
    tft->println("Back");
    // Draw Indeces
    char txt[2];
    for (int i = 0; i < 4; i++)
    {
        tft->setCursor(min_x - 50, y_start + t_height * i);
        sprintf(txt, "%d.", i + 1);
        tft->println(txt);
    }
    tft->setCursor(min_x - 50, 240 - t_height);
    tft->println("0.");
}

void LED_Switcher::draw_internal()
{
    int16_t _x, _y;
    uint16_t _w, _h;
    tft->fillScreen(ILI9341_BLACK);
    tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft->setTextSize(2);
    // Back button
    tft->getTextBounds("0.  Back", 0, 0, &_x, &_y, &_w, &_h);
    tft->setCursor(0, 240 - _h);
    tft->println("0.  Back");
    // Confirm Button
    tft->getTextBounds("OK.  Confirm", 0, 0, &_x, &_y, &_w, &_h);
    tft->setCursor(320 - _w, 240 - _h);
    tft->println("OK.  Confirm");
    // Variables
    uint16_t t_height = _h + 8;
    // Color Title
    tft->getTextBounds("Select Color", 0, 0, &_x, &_y, &_w, &_h);
    tft->setCursor(160 - _w / 2, 0);
    tft->println("Select Color");
    // Draw Colors
    uint16_t y_start = t_height + 5;
    char colors[12][7] = {
        {'R', 'e', 'd', '\0'}, {'G', 'r', 'e', 'e', 'n', '\0'}, {'B', 'l', 'u', 'e', '\0'}, {'\0'}, {'\0'}, {'\0'}, {'\0'}, {'W', 'h', 'i', 't', 'e', '\0'}, {'P', 'u', 'r', 'p', 'l', 'e', '\0'}, {'A', 'q', 'u', 'a', '\0'}, {'Y', 'e', 'l', 'l', 'o', 'w', '\0'}, {'\0'}};
    //// Draw columns
    uint8_t counter = 1;
    for (uint8_t col = 0; col < 3; col++)
    {
        uint16_t min_x = 0;
        for (int i = 0; i < 4; i++)
        {
            tft->getTextBounds(colors[i + 4 * col], 0, 0, &_x, &_y, &_w, &_h);
            min_x = min_x < _w ? _w : min_x;
            tft->setCursor((80 + 80 * col) - _w / 2, y_start + t_height * i);
            tft->println(colors[i + 4 * col]);
        }
        min_x = (80 + 80 * col) - (min_x / 2);
        // Draw Indeces
        char txt[2];
        for (int i = 0; i < 4; i++)
        {
            if (strlen(colors[i + 4 * col]) != 0)
            {
                tft->setCursor(min_x - 50, y_start + t_height * i);
                sprintf(txt, "%d.", col == 1 ? 7 : counter);
                tft->println(txt);
                counter = col == 1 ? counter : counter + 1;
            }
        }
    }
    // Brigthness title
    y_start = y_start + t_height * 4 + 15;
    tft->getTextBounds("Select Brightness", 0, 0, &_x, &_y, &_w, &_h);
    tft->setCursor(160 - _w / 2, y_start);
    tft->println("Select Brightness");
    // Draw brightness levels
    y_start += t_height;
    char vals[4][8] = {{'8', '.', '2', '5', '%', '\0'}, {'9', '.', '5', '0', '%', '\0'}, {'*', '.', '7', '5', '%', '\0'}, {'#', '.', '1', '0', '0', '%', '\0'}};
    for (int i = 0; i < 4; i++)
    {
        tft->getTextBounds(vals[i], 0, 0, &_x, &_y, &_w, &_h);
        tft->setCursor((33 + 83 * i) - _w / 2, y_start);
        tft->println(vals[i]);
    }
}

uint16_t LED_Switcher::update(uint32_t ir)
{
    if (!in_inner_menu)
    {
        switch (ir)
        {
        case IR_BUTTON_0: // Request switch to Monitor Page
            return caller_page;
        case IR_BUTTON_1: /// Adalight mode and request switch to Monitor Page
            adalight->setMode(1);
            computer->prismatik.enabled = true;
            Serial.println("LC1");
            return caller_page;
        case IR_BUTTON_2: /// Adalight RGB mode and request switch to Monitor Page
            adalight->setMode(1);
            computer->prismatik.enabled = true;
            Serial.println("LC2");
            return caller_page;
        case IR_BUTTON_3: // Mood light mode and switch to internal menu
            adalight->setMode(2);
            computer->prismatik.enabled = false;
            Serial.println("LC0");
            in_inner_menu = true;
            draw_internal();
            break;
        case IR_BUTTON_4: // LED off and request switch to Monitor Page
            adalight->setMode(0);
            computer->prismatik.enabled = false;
            Serial.println("LC0");
            return caller_page;

        default:
            break;
        }
    }
    else
    {
        switch (ir)
        {
        case IR_BUTTON_0: // Go to normal menu
            in_inner_menu = false;
            draw();
            break;
        case IR_BUTTON_1: //
            adalight->setColor(255, 0, 0);
            break;
        case IR_BUTTON_2: //
            adalight->setColor(0, 255, 0);
            break;
        case IR_BUTTON_3: //
            adalight->setColor(0, 0, 255);
            break;
        case IR_BUTTON_4: //
            adalight->setColor(255, 0, 255);
            break;
        case IR_BUTTON_5: //
            adalight->setColor(51, 204, 204);
            break;
        case IR_BUTTON_6: //
            adalight->setColor(255, 255, 0);
            break;
        case IR_BUTTON_7: //
            adalight->setColor(255, 255, 255);
            break;
        case IR_BUTTON_8: //
            adalight->setBrightness(255 / 100.0 * 25.0);
            break;
        case IR_BUTTON_9: //
            adalight->setBrightness(255 / 100.0 * 50.0);
            break;
        case IR_BUTTON_STAR: //
            adalight->setBrightness(255 / 100.0 * 75.0);
            break;
        case IR_BUTTON_HASHTAG: //
            adalight->setBrightness(255);
            break;
        case IR_BUTTON_OK: // LED off and request switch to Monitor Page
            in_inner_menu = false;
            return caller_page;

        default:
            break;
        }
    }
    return UINT16_MAX;
}