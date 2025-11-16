#include <Arduino.h>
#include <Wire.h>

void setup()
{
  Serial.begin(115200);

  // Initialize I2C on the custom pins
  if (!Wire.begin(I2C_SDA, I2C_SCL))
  {
    Serial.println("Failed to initialize I2C");
    while (1)
      ; // Stop if initialization fails
  }

  Serial.println("I2C Scanner starting...");
}

void loop()
{
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning for I2C devices...");

  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000); // Wait 5 seconds before scanning again
}