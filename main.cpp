#include "Arduino.h"
#include "PID.h"

#define dt 10

char key;
float val;
bool isOn = false;

uint32_t tmr;

PID pid(0, 0, 0, dt);

// Inertial process
#define LOW_VALUE 15    // к этому значению "остывает" система
#define SIGNAL_COEF 1 // сила сигнала
#define DELAY_AMOUNT 10 // задержка изменения
float value = 15;
float signal = 0;
float COEF = 0.1;

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
// Inertial process //

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(50);
    pidr.setpoint = 75;
    // pid.setLimits(0.0, 255.0);

    // pinMode(3, OUTPUT);
}

void loop()
{
    if (Serial.available() > 1)
    {
        key = Serial.read();
        val = Serial.parseFloat();
        switch(key)
        {
            case 'P': pid.kP = val; break;
            case 'I': pid.kI = val; break;
            case 'D': pid.kD = val; break;
            case 'S': pid.setpoint = val; break;
            case 'O': pid.output = val; break;
            case 'M': isOn = (bool)val; break;
        }
        Serial.print("kP = ");Serial.print(pid.kP, 5);Serial.print(", kI = ");Serial.print(pid.kI, 5);Serial.print(", kD = ");Serial.print(pid.kD, 5);Serial.print(", Power: ");Serial.println((String)(isOn ? "On" : "Off"));
    }
    if (millis() - tmr > dt)
    {
        // pid.input = map(analogRead(A0), 0, 1023, 0, 255);
        // analogWrite(3, pid.getResult());
        // analogWrite(3, pid.output);

        pid.input = value;
        if (isOn) signal = pid.getResult();

        Serial.print(">setpoint:");Serial.println(pid.setpoint);
        Serial.print(">input:");Serial.println(pid.input);
        Serial.print(">output:");Serial.println(pid.output);

        tmr = millis();
    }
    process();
}
