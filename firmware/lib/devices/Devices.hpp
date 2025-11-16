// Devices.hpp

#ifndef DEVICES_HPP
#define DEVICES_HPP

#include <Arduino.h>

#include "Barometer.hpp"
#include "Imu.hpp"
#include "Sd.hpp"
#include "finServo.hpp"
#include "KalmanFilter.hpp"
#include <Wire.h>
#include "Debug.hpp"

struct deviceStatus
{
    bool barometer;
    bool imu;
    bool sd;
    bool servo;
};

class Devices
{
public:
    Devices();
    ~Devices();

    deviceStatus init();

    GyroData getGyro();
    float getAltitude();
    float getRollRate();
    float getTemperature();
    void updateData();
    float getAirDensity();
    float getPressure();
    float getVelocity();

    float getFilteredAltitude();
    float getFilteredTemperature();
    float getFilteredRollRate();
    float getFilteredAirDensity();
    float getFilteredPressure();
    float getFilteredVelocity();

    Barometer baro;
    Sd sd;
    Imu imu;
    FinServo servo;

    // Kalman filter setup for each measurement type
    KalmanFilter kfAltitude;
    KalmanFilter kfTemperat;
    KalmanFilter kfRollRate;
    KalmanFilter kfPressure;
    KalmanFilter kfVelocity;

    float previousAltitude;
    unsigned long previousTime;

    TwoWire wire = Wire;

private:
    deviceStatus status;
};

#endif // DEVICES_HPP