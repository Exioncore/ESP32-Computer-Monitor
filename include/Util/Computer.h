#ifndef Computer_h
#define Computer_h

#include <Arduino.h>
#include "Util/MetricStats.h"

struct Computer
{
    struct Metrics
    {
        struct CPU
        {
            char *name;
            MetricStats clock;
            MetricStats temperature;
            MetricStats load;
            MetricStats maxThreadLoad;
            MetricStats power;
        } cpu;
        struct GPU
        {
            char *name;
            MetricStats coreClock;
            MetricStats memClock;
            MetricStats temperature;
            MetricStats coreLoad;
            MetricStats memUsage;
            MetricStats power;
        } gpu;
        struct RAM
        {
            MetricStats memUsage;
            uint16_t memCapacity;
        } ram;
        struct Network
        {
            uint16_t downloadSpeedKBs;
            uint16_t uploadSpeedKBs;
        } network;
        struct FanController
        {
            MetricStats liquidTemperature;
            MetricStats ambientTemperature;
        } fanController;
        MetricStats framerate;

        bool initialized = false;
        unsigned long last_update = ULONG_MAX;
    } metrics;

    struct Audio
    {
        std::vector<char *> presets;
        bool initialized = false;
    } audio;

    struct Prismatik
    {
        bool enabled = false;
    } prismatik;
};

#endif