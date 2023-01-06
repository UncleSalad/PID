# PID
Just PID

Use VSCode + PlatformIO

Как юзать
1. Создай объект<br>
<br>
PID твоё_имя<br>
(или)<br>
PID твоё_имя(kP, kI, kD, dt, minOut, maxOut);<br>
2. Задай, к чему привести систему<br>
твоё_имя.setPoint(значение);<br>
3. Заставь считать через каждый период dt<br>
static uint32_t tmr = 0;<br>
for (millis() - tmr > dt)<br>
{<br>
    out = твоё_имя.getResult(значение_с_датчика_температуры_допустим);<br>
}<br>
4. Расскажи своей мамочке, какой ты молодец-холодец))
