#include "Display/Metric.h"

Metric::Metric(
    Adafruit_ILI9341* tft, 
    uint16_t x, 
    uint16_t y,
    uint8_t metricSize, 
    const char* unit, 
    uint8_t unitSize,
    uint16_t bck
) {
    this->tft = tft;
    this->x = x;
    this->y = y;
    this->metricSize = metricSize;
    this->unit = unit;
    this->unitSize = unitSize;
    this->bck = bck;
    this->last_w = 0;
    // Retrieve unit text width
    int16_t _x, _y;
    tft->setTextSize(unitSize);
    tft->getTextBounds(unit, 0, 0, &_x, &_y, &u_w, &u_h);
}

void Metric::update(int value) {
    char txt[10]; 
    sprintf(txt, "%d", value);
    if (strcmp(txt, old_txt) != 0) {
        memcpy(old_txt, txt, 10);
        draw(txt);
    }
}

void Metric::update(float value, uint8_t decimal_places) {
    char txt[10]; 
    sprintf(txt, "%d.%d", (int)value, (int)((value - (int)value) * powf(10.0f, decimal_places)));
    if (strcmp(txt, old_txt) != 0) {
        memcpy(old_txt, txt, 10);
        draw(txt);
    }
}

void Metric::draw(char txt[10]) {
    int16_t _x, _y;
    uint16_t m_w, m_h;

    // Get width of the metric text + unit text
    tft->setTextSize(metricSize);
    tft->getTextBounds(txt, 0, 0, &_x, &_y, &m_w, &m_h);
    uint16_t w = m_w + u_w;
    // Check if the width of the metric text is now smaller than previously
    if (w < last_w) {
        uint16_t clr_w =  ceil((last_w - w) / 2.0);
        uint16_t half_size = ceil(last_w / 2.0);  
        tft->fillRect(x - half_size, y - m_h / 2, clr_w, m_h, bck);
        tft->fillRect(x + half_size - clr_w - u_w, y - m_h / 2, clr_w + u_w, m_h, bck);
        // Un-optimized
        // tft->fillRect(x - last_w / 2, y - m_h / 2, last_w, m_h, bck);
    }

    // Draw metric
    tft->setTextSize(metricSize);
    tft->setCursor(x - w / 2, y - m_h / 2);
    tft->println(txt);
    // Draw metric unit
    if (w != last_w) {
        tft->setTextSize(unitSize);
        tft->setCursor(x + w / 2 - u_w, y - m_h / 2 + (m_h - u_h));
        tft->println(unit);
    }

    // Update internal tracking
    last_w = w;
}