#include "Display/ArcProgressBar.h"
#include "Util/GFXHelper.h"

ArcProgressBar::ArcProgressBar() {}

ArcProgressBar::ArcProgressBar(
            Adafruit_ILI9341* tft,
            tuple<uint16_t, uint16_t> position,
            uint16_t radius,
            uint8_t thickness,
            uint16_t startAngle,
            uint16_t maxAngle,
            tuple<uint16_t, uint16_t> bounds,
            uint16_t foreground,
            uint16_t background
        ) {
    this->tft = tft;
    this->position = position;
    this->radius = radius;
    this->thickness = thickness;
    this->startAngle = this->lastAngle = this->target_angle = startAngle;
    this->maxAngle = maxAngle;
    this->bounds = bounds;
    this->range = get<1>(bounds) - get<0>(bounds);
    // Colors
    this->fg = foreground;
    this->bg = background;
}

void ArcProgressBar::update(uint16_t value, uint8_t steps) {
    // Ensure value is within bounds
    value = value < get<0>(bounds) ? get<0>(bounds) : (value > get<1>(bounds) ? get<1>(bounds) : value);
    // Handle drawing
    uint16_t angle = round(this->maxAngle / (double)this->range * (value - get<0>(bounds)));

    if (redraw) {
        GFXHelper::fillArc(tft, get<0>(position) + radius, get<1>(position) + radius, startAngle, angle, radius, radius, thickness, fg);
        GFXHelper::fillArc(tft, get<0>(position) + radius, get<1>(position) + radius, startAngle + angle, maxAngle - angle, radius, radius, thickness, bg);
        this->target_angle = this->lastAngle = startAngle + angle;
        redraw = false;
    } else {
        // Angle steps
        angle += startAngle;
        if (target_angle != angle) {
            target_angle = angle;
            float anglet_step_f = ((float)(target_angle - lastAngle)) / steps;
            angle_step = anglet_step_f < 0.0 ? floor(anglet_step_f) : ceil(anglet_step_f);
            // Drawing
            update();
        }
    }
}

void ArcProgressBar::update() {
    // Drawing
    if (target_angle != lastAngle) {
        if (angle_step > 0) {
            if (lastAngle + angle_step > target_angle) angle_step = target_angle - lastAngle;
            GFXHelper::fillArc(tft, get<0>(position) + radius, get<1>(position) + radius, lastAngle, angle_step, radius, radius, thickness, fg);
        } else if (angle_step < 0) {
            if (lastAngle + angle_step < target_angle) angle_step = target_angle - lastAngle;
            GFXHelper::fillArc(tft, get<0>(position) + radius, get<1>(position) + radius, lastAngle + angle_step, -angle_step, radius, radius, thickness, bg);
        }
        this->lastAngle += angle_step;
    }
}