#include "Analysis.hpp"

Analysis::Analysis() : startTime(0), endTime(0), running(false) {}

void Analysis::start()
{
    startTime = micros();
    running = true;
}

void Analysis::stop()
{
    if (running)
    {
        endTime = micros();
        running = false;
    }
}

unsigned long Analysis::getElapsedTime() const
{
    if (running)
    {
        return micros() - startTime;
    }
    else
    {
        return endTime - startTime;
    }
}