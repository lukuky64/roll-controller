#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include <Arduino.h>

class Analysis
{
public:
    Analysis();
    void start();
    void stop();
    unsigned long getElapsedTime() const;

private:
    unsigned long startTime;
    unsigned long endTime;
    bool running;
};

#endif // ANALYSIS_HPP