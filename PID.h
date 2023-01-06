#ifndef PID_H
#define PID_H

#include "Arduino.h"

class PID
{
    // Private
    private:
    float _kP, _kI, _kD, _setpoint;
    float _integral, _diff;
    float _min, _max;
    uint32_t _dt;

    // Public
    public:
    PID(float kP, float kI, float kD, uint32_t dt, float min, float max);

    void setP(float kP) { _kP = kP; }
    void setI(float kI) { _kI = kI; }
    void setD(float kD) { _kI = kD; }
    void setDT(float dt) { _dt = dt; }
    void setPoint(float setpoint) { _setpoint = setpoint; }

    float getP() { return _kP; }
    float getI() { return _kI; }
    float getD() { return _kD; }
    uint32_t getDT() { return _dt; }
    float getPoint() { return _setpoint; }

    float getResult(float input);
};

// Как юзать
/*
    1. Создай объект
    PID твоё_имя
    (или)
    PID твоё_имя(kP, kI, kD, dt, minOut, maxOut);

    2. Задай, к чему привести систему
    твоё_имя.setPoint(значение);

    3. Заставь считать через каждый период dt
    static uint32_t tmr = 0;
    for (millis() - tmr > dt)
    {
        out = твоё_имя.getResult(значение_с_датчика_температуры_допустим);
    }

    4. Расскажи своей мамочке, какой ты молодец-холодец))
*/

#endif