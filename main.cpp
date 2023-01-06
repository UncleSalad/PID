#include <Arduino.h>
#include "PID.h"

// Inertial process
#define LOW_VALUE 15    // к этому значению "остывает" система
#define SIGNAL_COEF 0.1 // сила сигнала
#define DELAY_AMOUNT 10 // задержка изменения
#define dt 30
float value = 15;
float signal = 0;
float COEF = 0.1;
float setVal;
uint32_t tmr = 0;

// For parse
float val;
char key[2];

// PID
bool isOn = false;
PID regul(0.0, 0.0, 0.0, dt, 0, 450);

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(50);
}

// Inertial process
void process()
{
    static float valueSpeed;
    static float signalSpeed;
    static bool firstFlag = false;
    static float delayArray[DELAY_AMOUNT];
    // сигнал == скорость нагрева
    // ограничивает сигнал его же значением и плавно к нему стремится
    signalSpeed += (signal - signalSpeed) * 0.003;
    // складываем скорость сигнала и скорость охлаждения
    // скорость охлаждения получаем как разность "температуры" и её нулевого значения
    valueSpeed = signalSpeed * SIGNAL_COEF + (LOW_VALUE - value) * COEF;
    if (!firstFlag)
    {
        firstFlag = true;
        for (int i = 0; i < DELAY_AMOUNT; i++)
            delayArray[i] = valueSpeed;
    }
    for (int i = 0; i < DELAY_AMOUNT - 1; i++)
        delayArray[i] = delayArray[i + 1];
    delayArray[DELAY_AMOUNT - 1] = valueSpeed;
    // прибавляем скорость (интегрируем)
    value += /*valueSpeed*/ delayArray[0];
}

void loop()
{
    // Every 30 ms (dt = 30 ms)
    if (millis() - tmr > dt)
    {
        // 1. Parse
        if (Serial.available() > 1)
        {
            Serial.readBytes(key, 2);
            val = Serial.parseFloat();
            switch (key[0])
            {
                case 'k':
                switch (key[1])
                {
                    case 'P': regul.setP(val); break;
                    case 'I': regul.setI(val); break;
                    case 'D': regul.setD(val); break;
                }
                break;

                case 'S':
                switch (key[1])
                {
                    case 'T': isOn = (bool)val; break;
                    case 'I': regul.setPoint(val); break;
                }
                break;
            }
            Serial.println("kP: " + (String)regul.getP() + ", kI: " + (String)regul.getI() + ", kD: " + (String)regul.getD() + ", SetVal: " + regul.getPoint() + ",PowerState: " + (String)((isOn) ? "On" : "Off"));
        }
        // 2. Process
        // input = value
        // output = signal
        if (isOn) signal = regul.getResult(value);
        process();

        // 3. Plot
        Serial.println(">setpoint:" + (String)regul.getPoint());
        Serial.println(">signal:" + (String)signal);
        Serial.println(">value:" + (String)value);
        tmr = millis();
    }
}
