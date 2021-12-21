#include "Util/GFXHelper.h"

#define DEG2RAD 0.0174532925

void GFXHelper::drawImage(Adafruit_ILI9341 *tft, uint16_t x, uint16_t y, const uint16_t details[], const byte data[], uint16_t fg)
{
    uint16_t count = 0;

    if (details[2] == 255)
    {
        for (uint16_t i = 0; i < details[0]; i += 2)
        {
            if (data[i] == 1)
            {
                if (count + data[i + 1] >= details[1])
                {
                    uint16_t remaining = data[i + 1];
                    while (remaining > 0)
                    {
                        uint16_t lSize = count + remaining >= data[i + 1] ? details[1] - count : remaining;
                        tft->drawFastHLine(x + count, y, lSize, fg);
                        remaining -= lSize;
                        count = remaining >= details[1] ? 0 : count + lSize;
                        y = remaining > 0 ? y + 1 : y;
                    }
                }
                else
                {
                    tft->drawFastHLine(x + count, y, data[i + 1], fg);
                    count += data[i + 1];
                }
            }
            else
            {
                count += data[i + 1];
                while (count >= details[1])
                {
                    count -= details[1];
                    y++;
                }
            }
        }
    }
    else if (details[2] == 0 || details[2] == 1)
    {
        uint8_t color = details[2];
        for (uint16_t i = 0; i < details[0]; i++)
        {
            if (color)
            {
                if (count + data[i] >= details[1])
                {
                    uint16_t remaining = data[i];
                    while (remaining > 0)
                    {
                        uint16_t lSize = count + remaining >= data[i] ? details[1] - count : remaining;
                        tft->drawFastHLine(x + count, y, lSize, fg);
                        remaining -= lSize;
                        count = remaining >= details[1] ? 0 : count + lSize;
                        y = remaining > 0 ? y + 1 : y;
                    }
                }
                else
                {
                    tft->drawFastHLine(x + count, y, data[i], fg);
                    count += data[i];
                }
            }
            else
            {
                count += data[i];
                while (count >= details[1])
                {
                    count -= details[1];
                    y++;
                }
            }
            color = color == 1 ? 0 : 1;
        }
    }
}

void GFXHelper::fillArc(Adafruit_ILI9341 *tft, uint16_t x, uint16_t y, uint16_t start_angle, uint16_t seg_count, uint16_t rx, uint16_t ry, int w, uint16_t color)
{
    // Draw color blocks every inc degrees
    for (uint16_t i = start_angle; i < start_angle + seg_count; i += 1)
    {
        // Calculate pair of coordinates for segment start
        float sx = cos((i - 90) * DEG2RAD);
        float sy = sin((i - 90) * DEG2RAD);
        uint16_t x0 = sx * (rx - w) + x;
        uint16_t y0 = sy * (ry - w) + y;
        uint16_t x1 = sx * rx + x;
        uint16_t y1 = sy * ry + y;

        // Calculate pair of coordinates for segment end
        float sx2 = cos((i + 1 - 90) * DEG2RAD);
        float sy2 = sin((i + 1 - 90) * DEG2RAD);
        uint16_t x2 = sx2 * (rx - w) + x;
        uint16_t y2 = sy2 * (ry - w) + y;
        uint16_t x3 = sx2 * rx + x;
        uint16_t y3 = sy2 * ry + y;
        // FAIL AREA
        tft->fillTriangle(x0, y0, x1, y1, x2, y2, color);
        tft->fillTriangle(x1, y1, x2, y2, x3, y3, color);
    }
}

void GFXHelper::writeTxtCentered(Adafruit_ILI9341 *tft, uint16_t x, uint16_t y, char *txt, uint8_t size)
{
    int16_t _x, _y;
    uint16_t w, h;
    tft->setTextSize(size);
    tft->getTextBounds(txt, 0, 0, &_x, &_y, &w, &h);
    tft->setCursor(x - w / 2, y - h / 2);
    tft->println(txt);
}

void GFXHelper::writeMetricWithUnit(
    Adafruit_ILI9341 *tft, uint16_t x, uint16_t y,
    char *metric, uint8_t metricSize, const char *unit, uint8_t unitSize)
{
    int16_t _x, _y;
    uint16_t m_w, m_h;
    uint16_t u_w, u_h;
    uint16_t w;

    tft->setTextSize(metricSize);
    tft->getTextBounds(metric, 0, 0, &_x, &_y, &m_w, &m_h);
    tft->setTextSize(unitSize);
    tft->getTextBounds(unit, 0, 0, &_x, &_y, &u_w, &u_h);
    w = m_w + u_w;

    tft->setTextSize(metricSize);
    tft->setCursor(x - w / 2, y - m_h / 2);
    tft->println(metric);

    tft->setTextSize(unitSize);
    tft->setCursor(x + m_w / 2, y);
    tft->println(unit);
}