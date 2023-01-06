#include "PID.h"

PID::PID(float kP, float kI, float kD, uint32_t dt, float min, float max)
{
    _kP = kP;
    _kI = kI;
    _kD = kD;
    _dt = dt;
    _min = min;
    _max = max;
}

float PID::getResult(float input)
{
    static float prevError;
    float error = _setpoint - input;
    _integral = constrain(_integral + (error * _dt * _kI), _min, _max);
    _diff = (error - prevError) / _dt;
    prevError = error;
    return constrain((_kP * error) + _integral + (_kD * _diff), _min, _max);
}
