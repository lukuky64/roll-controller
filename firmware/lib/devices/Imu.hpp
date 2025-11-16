#ifndef IMU_HPP
#define IMU_HPP

#include <Wire.h>
#include <SparkFun_BNO08x_Arduino_Library.h>

struct GyroData
{
    float x;
    float y;
    float z;
};

class Imu
{
public:
    Imu(TwoWire &wire);
    ~Imu();

    bool init(int BNO08X_ADDR);
    GyroData readSensor();
    void update(void);
    GyroData getLatest(void);

private:
    bool setReports();
    bool checkData();

    BNO08x bno08x;
    GyroData gyroData;
    TwoWire &Wire;
};

#endif // IMU_HPP
