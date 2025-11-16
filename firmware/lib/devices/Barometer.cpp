#include "Barometer.hpp"

Barometer::Barometer(TwoWire &wire) : Wire(wire) {}
Barometer::~Barometer() {}

bool Barometer::init(int addr)
{
    bool success = true;

    uint attempts = 0;

    while (attempts < 10)
    {
        if (pressureSensor.beginI2C(addr) == BMP5_OK)
        {
            success = true;
            break;
        }
        else
        {
            success = false;
            attempts++;
            delay(100);
        }
    }

    // Variable to track errors returned by API calls
    int8_t err = BMP5_OK;

    bmp5_iir_config config = {
        .set_iir_t = BMP5_IIR_FILTER_COEFF_127, // Set filter coefficient
        .set_iir_p = BMP5_IIR_FILTER_COEFF_127, // Set filter coefficient
        .shdw_set_iir_t = BMP5_ENABLE,          // Store filtered data in data registers
        .shdw_set_iir_p = BMP5_ENABLE,          // Store filtered data in data registers
        .iir_flush_forced_en = BMP5_DISABLE     // Flush filter in forced mode
    };
    err = pressureSensor.setFilterConfig(&config);
    if (err == BMP5_OK)
    {
        setGroundPressure(500, 10);
    }
    else
    {
        success = false;
    }

    return success;
}

// Updates groundPressure global variable in Pa
void Barometer::setGroundPressure(int sampleCount, int delayBetweenSamples)
{
    float totalPressure = 0;
    int8_t err;

    for (int i = 0; i < sampleCount; i++)
    {
        bmp5_sensor_data data = {0, 0};
        err = pressureSensor.getSensorData(&data);

        // Check whether data was acquired successfully
        if (err == BMP5_OK)
        {
            totalPressure += data.pressure;
        }

        delay(delayBetweenSamples); // Delay between samples (in milliseconds)
    }

    // Calculate the average pressure
    groundPressure = totalPressure / sampleCount;
}

bmp5_sensor_data Barometer::getBaroData()
{
    // Get measurements from the sensor
    bmp5_sensor_data data = {0, 0};
    int8_t err = pressureSensor.getSensorData(&data);
    static float P_sta;

    // Check whether data was acquired successfully
    if (err == BMP5_OK)
    {
        // Acquisistion succeeded, print temperature and pressure
    }
    return data;
}

float Barometer::getPressure()
{
    return getBaroData().pressure;
}

float Barometer::getTemperature()
{
    return getBaroData().temperature;
}

float Barometer::getRawAltitude(float pressure)
{
    float pressure_hPa =
        pressure / 100; // conversion from Pascals to hecto Pascals
    float rawAltitude = (1 - powf(pressure_hPa / 1013.25, 0.190284)) *
                        44307.69; // 44330, 44307.69
    return rawAltitude;
}

float Barometer::getAltitudeAGL(float pressure)
{
    // Don't need to convert from Pascals to hecto Pascals because it scales the same.
    // float groundPressure_hPa = groundPressure * 0.01f;
    // float pressure_hPa = pressure * 0.01f;

    // Calculate altitude above ground level in meters
    float altitudeAGL = (1.0f - powf(pressure / groundPressure, 0.190284f)) * 44307.69f;

    return altitudeAGL;
}

void Barometer::update()
{
    data = getBaroData();
}

bmp5_sensor_data Barometer::getLatest(void)
{
    return data;
}