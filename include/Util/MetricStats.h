#ifndef MetricStats_h
#define MetricStats_h

#include <Arduino.h>

class MetricStats
{
public:
    MetricStats();

    void set(double x);
    double get();
    double get_avg();
    double get_min();
    double get_max();

    void reset();

private:
    double min, curr, avg, max;
    long count;
};

#endif