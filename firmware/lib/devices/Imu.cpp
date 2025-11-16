#include "Imu.hpp"

Imu::Imu(TwoWire &wire) : Wire(wire) {}
Imu::~Imu() {}

bool Imu::init(int addr)
{
    const uint maxAttempts = 10;
    uint attempts = 0;

    while (attempts < maxAttempts)
    {
        if (bno08x.begin(addr, Wire, -1, BNO08X_NRST))
        {
            if (setReports()) // && checkData())
            {
                return true;
            }
        }
        attempts++;
        delay(100);
    }

    return false;
}

bool Imu::setReports(void)
{
    bool success = true;
    if (!bno08x.enableGyro()) // (!bno08x.enableReport(SH2_GYROSCOPE_UNCALIBRATED))
    {
        success = false;
    }

    return success;
}

void Imu::update(void)
{
    if (!bno08x.getSensorEvent())
    {
        return;
    }

    switch (bno08x.getSensorEventID())
    {
    case SENSOR_REPORTID_GYROSCOPE_CALIBRATED:
        gyroData.x = bno08x.getGyroX();
        gyroData.y = bno08x.getGyroY();
        gyroData.z = bno08x.getGyroZ();
        break;
    }
    return;
}

GyroData Imu::getLatest(void)
{
    return gyroData;
}

bool Imu::checkData()
{
    bool success = true;
    if (!bno08x.getSensorEvent())
    {
        success = false;
    }

    return success;
}