#ifndef SERVO_H_
#define SERVO_H_

#include <ESP32Servo.h>

class FinServo
{
public:
    struct Config
    {
        float calibrationGradient;
        float calibrationConstant;
        float airbrakeWidthMm; /** distance from centre of airbrake to the edge of
                                  the disc or paddles (so the length we see that
                                  pops out)*/
        float maxExtensionMm;
    };

    FinServo();
    ~FinServo();

    bool init(int servoPin, int initPos);
    void send(int pos);
    float calibrateAngle(float desiredAngle);
    float compensateBacklash(float angle);
    int getPos(void);
    int getRealPos(void);

    // void actuateAirbrakes(float airbrakeCrossSectionalArea);

private:
    Servo myservo;
    int servoPin;
    int currentPos;
    int zeroPos;
};

#endif
