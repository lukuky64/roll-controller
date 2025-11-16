#include "Devices.hpp"

Devices::Devices()
    : baro(Wire), imu(Wire), servo(),
      kfAltitude(0.1, 0.1, 1.0, 0.0),
      kfTemperat(0.1, 0.1, 1.0, 0.0),
      kfRollRate(0.1, 0.1, 1.0, 0.0),
      kfPressure(0.1, 0.1, 1.0, 0.0),
      kfVelocity(0.1, 0.1, 1.0, 0.0),
      previousAltitude(0.0), previousTime(0) {}

Devices::~Devices() {}

deviceStatus Devices::init()
{
    DBG("Initializing devices...");

    // setup wire on custom I2C pins
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(100000);

    delay(2000);

    status.imu = imu.init(BNO08X_I2C_ADDR);
    DBG("imu success: " + String(status.imu));

    status.barometer = baro.init(BMP581_I2C_ADDR);
    DBG("baro success: " + String(status.barometer));

    String startMsg = "Roll Controller\n" + String("Time(s), Filtered_Altitude(m), Filtered_Pressure(Pa), Filtered_Temperature(°C), Filtered_Vertical_Velocity(m/s), Filtered_RollRate(rad/s), Servo_Angle(°) \n");
    status.sd = sd.init(SD_CS, SD_MOSI, SD_MISO, SD_SCK, (startMsg));
    DBG("sd success: " + String(status.sd));

    status.servo = servo.init(SERVO_PIN, 90);
    DBG("servo success: " + String(status.servo));

    previousTime = millis();
    previousAltitude = getAltitude();

    return status;
}

float Devices::getAltitude()
{
    return baro.getAltitudeAGL(baro.getLatest().pressure);
}

float Devices::getPressure()
{
    return baro.getLatest().pressure;
}

float Devices::getTemperature()
{
    return baro.getLatest().temperature;
}

float Devices::getAirDensity()
{
    float R_specific = 287.05;                                   // Specific gas constant for dry air in J/(kg·K)
    float pressure_Pa = baro.getLatest().pressure * 100;         // hPa to Pa
    float temperature_K = baro.getLatest().temperature + 273.15; // Convert temperature to Kelvin

    return pressure_Pa / (R_specific * temperature_K); // Calculate air density
}

float Devices::getFilteredAirDensity()
{
    float R_specific = 287.05;                            // Specific gas constant for dry air in J/(kg·K)
    float pressure_Pa = kfPressure.getValue() * 100;      // hPa to Pa
    float temperature_K = kfTemperat.getValue() + 273.15; // Convert temperature to Kelvin

    return pressure_Pa / (R_specific * temperature_K); // Calculate air density
}

GyroData Devices::getGyro()
{
    return imu.getLatest();
}

float Devices::getRollRate()
{
    return imu.getLatest().y; // !!! change to the actual axis
}

float Devices::getFilteredRollRate()
{
    return kfRollRate.getValue();
}

float Devices::getFilteredAltitude()
{
    return kfAltitude.getValue();
}

float Devices::getFilteredTemperature()
{
    return kfTemperat.getValue();
}

float Devices::getFilteredPressure()
{
    return kfPressure.getValue();
}

float Devices::getVelocity()
{
    unsigned long currentTime = millis();
    float currentAltitude = getAltitude();
    float dt = (currentTime - previousTime) / 1000.0; // Convert to seconds

    float velocity = (currentAltitude - previousAltitude) / dt;

    previousAltitude = currentAltitude;
    previousTime = currentTime;

    return velocity;
}

float Devices::getFilteredVelocity()
{
    return kfVelocity.getValue();
}

void Devices::updateData()
{
    imu.update();
    baro.update();
    kfAltitude.update(getAltitude());
    kfTemperat.update(getTemperature());
    kfRollRate.update(getRollRate());
    kfPressure.update(getPressure());

    // Calculate velocity and update the velocity Kalman filter
    float velocity = getVelocity();
    kfVelocity.update(velocity);
}