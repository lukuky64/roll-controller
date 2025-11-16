#include <Arduino.h>
#include <Wire.h>

#include "SparkFun_BNO08x_Arduino_Library.h" // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_BNO08x
BNO08x myIMU;
void setReports(void);

void setup()
{
  Serial.begin(115200);

  while (!Serial)
    delay(10); // Wait for Serial to become available.
  // Necessary for boards with native USB (like the SAMD51 Thing+).
  // For a final version of a project that does not need serial debug (or a USB cable plugged in),
  // Comment out this while loop, or it will prevent the remaining code from running.

  Serial.println();
  Serial.println("BNO08x Read Example");

  Wire.begin(I2C_SDA, I2C_SCL);

  // if (myIMU.begin() == false) {  // Setup without INT/RST control (Not Recommended)
  if (myIMU.begin(BNO08X_I2C_ADDR, Wire, -1, BNO08X_NRST) == false)
  {
    Serial.println("BNO08x not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
    while (1)
      ;
  }
  Serial.println("BNO08x found!");

  // Wire.setClock(400000); //Increase I2C data rate to 400kHz

  setReports();

  Serial.println("Reading events");
  delay(100);
}

// Here is where you define the sensor outputs you want to receive
void setReports(void)
{
  Serial.println("Setting desired reports");
  if (myIMU.enableAccelerometer() == true)
  {
    Serial.println(F("Accelerometer enabled"));
    Serial.println(F("Output in form x, y, z, in m/s^2"));
  }
  else
  {
    Serial.println("Could not enable accelerometer");
  }
}

void loop()
{
  delay(10);

  if (myIMU.wasReset())
  {
    Serial.print("sensor was reset ");
    setReports();
  }

  // Has a new event come in on the Sensor Hub Bus?
  if (myIMU.getSensorEvent() == true)
  {

    // is it the correct sensor data we want?
    if (myIMU.getSensorEventID() == SENSOR_REPORTID_ACCELEROMETER)
    {

      float x = myIMU.getAccelX();
      float y = myIMU.getAccelY();
      float z = myIMU.getAccelZ();

      Serial.print(x, 2);
      Serial.print(F(","));
      Serial.print(y, 2);
      Serial.print(F(","));
      Serial.print(z, 2);

      Serial.println();
    }
  }
}
