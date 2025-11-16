#ifndef BAROMETER_HPP
#define BAROMETER_HPP

#include <SparkFun_BMP581_Arduino_Library.h>
#include <Wire.h>

class Barometer
{
public:
    Barometer(TwoWire &wire);
    ~Barometer();

    bool init(int addr);

    void setGroundPressure(int sampleCount, int delayBetweenSamples);
    void printBarometer();

    bmp5_sensor_data getBaroData();
    float getPressure();
    float getTemperature();
    float getRawAltitude(float pressure);
    float getAltitudeAGL(float pressure);

    void update();

    bmp5_sensor_data getLatest(void);

private:
    BMP581 pressureSensor;
    float groundPressure;
    bmp5_sensor_data data;
    TwoWire &Wire;
};

#endif