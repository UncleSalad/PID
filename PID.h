#pragma once

#include "Arduino.h"

class PID
{
    private:
    // Minimum and maximum output range
    float _min, _max;

    // Sample rate in seconds
    float _dt_s;

    // Integral component
    float _integral;

    // Derivative component
    float _derivative;

    // Regulation error
    float _error, _prevError;

    public:
    // Constructor
    PID(float Kp, float Ki, float Kd, float dT = 100);

    // PID coefficients
    float kP, kI, kD;

    // Sample rate
    float dt;

    // Input
    float input;

    // Output
    float output;

    // Set point
    float setpoint;

    // Calculate
    float getResult();

    // Constrain output value
    void setLimits(float min, float max);
};
