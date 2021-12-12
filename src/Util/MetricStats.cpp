#include "Util/MetricStats.h"

MetricStats::MetricStats() {
    min = curr = avg = max = 0;
    count = -5;
}

void MetricStats::set(double x) {
    if (count > 0) {
        min = x < min ? x : min;
        max = x > max ? x : max;
        avg += x;
    } else if (count == 0) {
        min = max = x;
        avg = x;
    }
    curr = x; 
    count += 1;
}

double MetricStats::get() {
    return curr;
}

double MetricStats::get_avg() {
    if (count > 0) return avg / count;
    else return 0;
}

double MetricStats::get_min() {
    return min;
}

double MetricStats::get_max() {
    return max;
}

void MetricStats::reset() {
    if (count > 0) {
        count = 1;
        max = avg = min = curr;
    }
}