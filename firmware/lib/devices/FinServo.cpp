#include "FinServo.hpp"

// Here we will convert airbrake area to angle that can be used

FinServo::FinServo() {}
FinServo::~FinServo() {}

bool FinServo::init(int servoPin, int initPos)
{
    ESP32PWM::allocateTimer(0);          // Allocation of timers
    myservo.setPeriodHertz(50);          // standard 50 hz servo
    myservo.attach(servoPin, 500, 2500); // set pin number and min/max pulse width
    zeroPos = initPos;
    send(0); // set to zero point. this might need to be changed.
    return true;
}

void FinServo::send(int pos)
{
    currentPos = pos + zeroPos;
    myservo.write(currentPos);
}

int FinServo::getPos(void)
{
    return currentPos - zeroPos;
}

int FinServo::getRealPos(void)
{
    return currentPos;
}
