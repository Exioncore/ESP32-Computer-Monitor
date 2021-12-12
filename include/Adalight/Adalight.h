#ifndef Adalight_h
#define Adalight_h

#include <Arduino.h>
#include <HardwareSerial.h>
#include <FastLED.h>
#include <vector>

using namespace std;

class Adalight {
    public:
        Adalight() {};
        Adalight(uint16_t numLeds, HardwareSerial* serial);

        void setMode(uint8_t mode);
        void setColor(uint8_t r, uint8_t g, uint8_t b);
        void setBrightness(uint8_t value);
        void update();
    private:
        // Custom
        bool initialized = false;
        bool enabled = false;
        CRGB static_color = CRGB(51, 115, 255); // CRGB(51, 115, 255); BLUE
        float static_color_hsv[3];
        unsigned long t_anim = 0;
        uint8_t animation_type = 1;

        double anim_var_1 = 0.0;
        double anim_var_2 = 30;
        double anim_var_3 = 0.0;
        void animation(uint8_t animation_type);    

        // Source https://gist.github.com/fairlight1337/4935ae72bcbcc1ba5c72
        void RGBtoHSV(float fR, float fG, float fB, float& fH, float& fS, float& fV);
        void HSVtoRGB(float& fR, float& fG, float& fB, float fH, float fS, float fV);

        // Adalight
        vector<CRGB> leds;
        uint8_t* ledsRaw;
        uint8_t brightness = 255;
        uint8_t data_pin;
        HardwareSerial* serial;

        enum processModes_t {Header, Data} 
            mode = Header;
        uint8_t 
            magic[3];
        int16_t
            c;
        uint16_t
            outPos;
        uint32_t
            bytesRemaining;
        unsigned long
            t,
            lastByteTime;

        void headerMode();
        void dataMode();
        void dataSet();
        void serialFlush();
};

#endif