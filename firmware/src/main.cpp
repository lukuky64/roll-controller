#include <Arduino.h>
#include "SD.hpp"
#include "PID_v1.hpp"
#include "Devices.hpp"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Debug.hpp"

// #include "Analysis.hpp"
// Analysis analysis;

Devices devices;

bool ledState = false;
const int controlFreq = 100; // Looping frequency in Hz. 40Hz is fast as we can go while logging
const int logFreq = 100;     // Looping frequency in Hz. 40Hz is fast as we can go while logging

const int logIntervalUs = 1000000 / logFreq;
const int controlIntervalUs = 1000000 / controlFreq;

double Setpoint, Input, Output;
double Kp = 3, Ki = 0, Kd = 0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

struct TimeData
{
  unsigned long startTime;
  unsigned long lastLogTime;
  unsigned long elapsedTime;
  char timeString[10]; // To store formatted time
};

TimeData timer;

void errorBlink(int numBlinks);
void toggleLED();
void logTask(void *parameter);
void updateDataTask(void *parameter);
void controlTask(void *parameter);

// Task handles
TaskHandle_t updateDataTaskHandle = NULL;
TaskHandle_t controlTaskHandle = NULL;
TaskHandle_t logTaskHandle = NULL;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  toggleLED();

  INITIALISE_DBG(115200);
  delay(1000);
  toggleLED();

  deviceStatus status = devices.init();
  while (!(status.imu && status.barometer && status.servo))
  {
    errorBlink(3);
    delay(1000);
    status = devices.init();
  }

  Setpoint = 0; // !!! outsetpoint should be 0 degrees (facing down)

  // Turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-45, 45); // !!! Change so we operate between -45 and 45 deg

  timer.startTime = micros();
  timer.lastLogTime = timer.startTime;

  // Create tasks
  xTaskCreatePinnedToCore(updateDataTask, "UpdateDataTask", 2048, NULL, 1, &updateDataTaskHandle, 0);
  xTaskCreatePinnedToCore(controlTask, "ControlTask", 2048, NULL, 1, &controlTaskHandle, 0);
  xTaskCreatePinnedToCore(logTask, "logTask", 4096, NULL, 1, &logTaskHandle, 0);
}

void loop()
{
  // Empty loop as tasks are handled by FreeRTOS
}

void updateDataTask(void *parameter)
{
  while (1)
  {
    devices.updateData();
    vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks
  }
}

void controlTask(void *parameter)
{
  while (1)
  {
    // Read sensors and compute control
    Input = devices.getFilteredRollRate(); // !!! ensure correct roll axis is set
    myPID.Compute();

    // Send output to servo and log data
    devices.servo.send(Output);

    vTaskDelay(pdMS_TO_TICKS(controlIntervalUs / 1000)); // Convert microseconds to milliseconds for FreeRTOS delay
  }
}

void logTask(void *parameter)
{
  while (1)
  {
    timer.elapsedTime = micros() - timer.startTime;
    snprintf(timer.timeString, sizeof(timer.timeString), "%.4f", timer.elapsedTime / 1000000.0);

    char dataEntry[256];
    snprintf(dataEntry, sizeof(dataEntry), "%s,%.2f,%.2f,%.2f,%.2f,%.2f,%d\n",
             timer.timeString,
             devices.getFilteredAltitude(),
             devices.getFilteredPressure(),
             devices.getFilteredTemperature(),
             devices.getFilteredVelocity(),
             devices.getFilteredRollRate(),
             devices.servo.getPos());

    DBG(dataEntry);
    devices.sd.writeToBuffer(dataEntry);

    devices.sd.flushBuffer();
    vTaskDelay(pdMS_TO_TICKS(logIntervalUs / 1000)); // Yield to other tasks
  }
}

void errorBlink(int numBlinks)
{
  for (int i = 0; i < numBlinks; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      toggleLED();
      delay(200);
    }
    delay(1000);
  }
}

void toggleLED()
{
  digitalWrite(LED_PIN, ledState ? LOW : HIGH);
  ledState = !ledState;
}
