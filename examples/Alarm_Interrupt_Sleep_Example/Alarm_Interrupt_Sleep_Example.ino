/**
 **************************************************
 *
 * @file        Alarm_Interrupt_Sleep_Example.ino
 * @brief       Example for Alarm Interrupted Sleep on the AVRs
 *
 *
 *
 * @authors     @ Zvonimir Haramustek for soldered.com.com
 ***************************************************/

#include "PCF85063A-SOLDERED.h"
#ifdef __AVR__
#include <avr/sleep.h>
#endif

PCF85063A rtc;

int wakePin = 2; // pin used for waking up

#ifdef __AVR__
void wakeUpNow()
{
    sleep_disable(); // first thing after waking from sleep: disable sleep...
    detachInterrupt(digitalPinToInterrupt(
        0)); // disables interrupt 0 on pin 2 so the wakeUpNow code will not be executed during normal running time.
}
#endif

void setup()
{
    Serial.begin(115200);

    pinMode(wakePin, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);

    //  setTime(hour, minute, sec);
    rtc.setTime(6, 54, 00); // 24H mode, ex. 6:54:00
    //  setDate(weekday, day, month, yr);
    rtc.setDate(6, 16, 5, 2020); // 0 for Sunday, ex. Saturday, 16.5.2020.
    //  setAlarm(alarm_second, alarm_minute, alarm_hour, alarm_day, alarm_weekday);
    rtc.setAlarm(15, 54, 99, 99, 99); // use 99 if no alarm
    checkAlarm();

    Serial.print("Now is:");
}

void loop()
{
    printCurrentTime();
    Serial.println("Entering sleep mode in 1 second");
    delay(1000);

    sleepNow(); // sleep function called here

    Serial.print("Interrupt triggered on: ");
}

void printCurrentTime()
{
    switch (rtc.getWeekday())
    {
    case 0:
        Serial.print("Sunday , ");
        break;
    case 1:
        Serial.print("Monday , ");
        break;
    case 2:
        Serial.print("Tuesday , ");
        break;
    case 3:
        Serial.print("Wednesday , ");
        break;
    case 4:
        Serial.print("Thursday , ");
        break;
    case 5:
        Serial.print("Friday , ");
        break;
    case 6:
        Serial.print("Saturday , ");
        break;
    }

    Serial.print(rtc.getDay());
    Serial.print(".");
    Serial.print(rtc.getMonth());
    Serial.print(".");
    Serial.print(rtc.getYear());
    Serial.print(". ");
    Serial.print(rtc.getHour());
    Serial.print(":");
    Serial.print(rtc.getMinute());
    Serial.print(":");
    Serial.println(rtc.getSecond());
}

void checkAlarm()
{
    Serial.print("Alarm is set to match: ");
    switch (rtc.getAlarmWeekday())
    {
    case 0:
        Serial.print("Sunday , ");
        break;
    case 1:
        Serial.print("Monday , ");
        break;
    case 2:
        Serial.print("Tuesday , ");
        break;
    case 3:
        Serial.print("Wednesday , ");
        break;
    case 4:
        Serial.print("Thursday , ");
        break;
    case 5:
        Serial.print("Friday , ");
        break;
    case 6:
        Serial.print("Saturday , ");
        break;
    default:
        break; // for getAlarmWeekday=99 alarm is not set for a weekday, donnot print
    }

    if (rtc.getAlarmDay() != 99)
    {
        Serial.print("Date: ");
        Serial.print(rtc.getAlarmDay());
    }
    if (rtc.getAlarmHour() != 99)
    {
        Serial.print(" hour: ");
        Serial.print(rtc.getAlarmHour());
    }
    if (rtc.getAlarmMinute() != 99)
    {
        Serial.print(" minute: ");
        Serial.print(rtc.getAlarmMinute());
    }
    if (rtc.getAlarmSecond() != 99)
    {
        Serial.print(" second: ");
        Serial.print(rtc.getAlarmSecond());
    }

    Serial.println("");
}

void sleepNow()
{
#ifdef __AVR__                                                  // for Arduino AVRs
    sleep_enable();                                             // enables the sleep bit in the mcucr register
    attachInterrupt(digitalPinToInterrupt(0), wakeUpNow, HIGH); // use interrupt 0 (pin 2) and run function
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);                        // sleep mode is set here
    digitalWrite(LED_BUILTIN, LOW);
    sleep_cpu(); // activating sleep
#else
    while (digitalRead(wakePin) == HIGH)
        ; // wait for wakeup interrupt
#endif

    // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
    digitalWrite(LED_BUILTIN, HIGH);
}