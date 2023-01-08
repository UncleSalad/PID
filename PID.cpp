#include "PID.h"

PID::PID(float Kp, float Ki, float Kd, float dT)
{
    kP = Kp;
    kI = Ki;
    kD = kD;
    dt = dT;
    _dt_s = dt / 1000;
}

float PID::getResult()
{
    _error = setpoint - input;
    _integral += _error * kI * _dt_s;
    _derivative = (_error - _prevError) / _dt_s;
    output = (_error * kP) + (_integral * kI) + (_derivative * kD);
    _prevError = _error;
    return constrain(output, _min, _max);
}

void PID::setLimits(float min, float max)
{
    _min = min;
    _max = max;
}
