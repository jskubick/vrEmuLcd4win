#include <iostream>
#include <cmath>
#include "TimeCounter5charFrame.h"
#include "DisplayManager.h"

TimeCounter5charFrame::TimeCounter5charFrame(int row, int startCol) : BaseFrame(row, startCol, 5) { }

/* reset counter and begin counting seconds from 00:00 to 99:59, then continue from a0:00 to z9:59 before "(~6h)"
 */
void TimeCounter5charFrame::startCountingFromZero(uint32_t currentTimeMillis) {
    // msb is 1
    // when downcast to uint32_t, currentTimeMillis rolls over every 49 days

    // when counterState's msb is set, the lower 15 bits represent the time when the counter started ticking.
    uint16_t startTime = (currentTimeMillis / 1000) & 0x7FFF;
    counterState = 0x8000 | startTime;
}

void TimeCounter5charFrame::startSlowlyCountingFromZero(uint32_t currentTimeMillis) {
    // top 3 bits are 010
    uint16_t startTime = (currentTimeMillis / 1000) & 0x1fff;
    counterState = 0x4000 | startTime;
}

void TimeCounter5charFrame::startCountdown(int minutes, int seconds, uint32_t currentTimeMillis) {
    // top 3 bits are 001
    /*
     * @ToDo: fix! right now, wrap-around isn't correctly handled.
     */
    uint32_t endTime = (((currentTimeMillis / 1000) & 0x1fff) + (minutes * 60) + seconds) & 0x1fff;
    counterState = 0x2000 | endTime;
}

void TimeCounter5charFrame::startLongCountdown(int hours, int minutes, uint32_t currentTimeMillis) {
    // top 3 bits are 011
    uint32_t minuteTime = (uint32_t)round(currentTimeMillis / 60000);
    minuteTime &= 0x1fff;
    uint32_t endTime = minuteTime + (hours * 60) + minutes;
    counterState = 0x6000 | endTime;
}

uint32_t TimeCounter5charFrame::test(uint32_t currentTimeMillis, uint16_t totalSeconds) {
    uint32_t value = currentTimeMillis / 1000;
    value += totalSeconds;
    value &=0x1fff;
    value |= 0x2000;

    return (uint16_t)value;
}

void TimeCounter5charFrame::setValue(int minutes, int seconds) {
    // top 3 bits are 000
    // when counterState's three top bits are clear, the bottom 13 bits hold a static value.
    // if the value is between 0 and 5999, it renders to the display as a value between 00:00 and 99:59
    // if the value is between 6000 and 21600, it renders to the display as a value between a0:00 and z9:59
    // if the value is 0x1fff, it renders to the display as "(6h+)"
    // if the value is 0x1ffe, it renders to the display as "100m+"
    counterState = ((minutes * 60) + seconds) & 0x3FFF;
}

void TimeCounter5charFrame::setRaw(uint16_t rawValue) {
    counterState = rawValue;
}

uint16_t TimeCounter5charFrame::getRaw() {
    return counterState;
}

bool display6h(char* buf) {
    *buf++='(';
    *buf++='6';
    *buf++='h';
    *buf++='+';
    *buf++=')';
    return true;
}

bool display100x(char* buf, char unit) {
    *buf++ = '1';
    *buf++ = '0';
    *buf++ = '0';
    *buf++ = unit;
    *buf++ = '+';
    return true;
}


bool showFinishedCountingDown(char* buf, bool colonVisible) {
    for (int x=0; x<5; x++)
        buf[x] = '-';
    buf[2] = colonVisible ? ':' : ' ';
    return true;
}

bool displayTime(char* buf, int elapsedSeconds, bool isColonVisible) {


    int elapsedMinutes = (int)elapsedSeconds / 60;
    elapsedSeconds %= 60;
    if (elapsedMinutes < 10)
        buf[0] = '0';
    else if (elapsedMinutes < 100)
        buf[0] = ((uint8_t)(elapsedMinutes / 10)) + 0x30;
    else
        // making this -10 before +0x41 put me in a series where the prefix was AD and AE
        buf[0] = ((uint8_t)(elapsedMinutes / 10)) -10 + 0x41;
    buf[1] = ((uint8_t)(elapsedMinutes % 10)) + 0x30;

    buf[2] = isColonVisible ? ':' : ' ';

    if (elapsedSeconds < 10)
        buf[3] = '0';
    else
        buf[3] = ((uint8_t)elapsedSeconds / 10) + 0x30;
    buf[4] = ((uint8_t)elapsedSeconds % 10) + 0x30;
    return true;
}

bool TimeCounter5charFrame::renderInto(char* buf, uint32_t currentTimeMillis) {
    buf += (LCD_COLS * row) + startCol;
    uint16_t secondTime = (uint16_t)(currentTimeMillis / 1000.0);

    // 3 msb = 000, so it's a static value. 13LSB = 1fff, so we overflowed after 6 hours at some point in the past.
    if (counterState == 0x1fff) {
        display6h(buf);
        return false;
    }

    // 3 msb = 000, so it's a static value. 13LSB = 1ffe, so we overflowed after 99:59 at some point in the past.
    if (counterState == 0x1ffe) {
        display100x(buf, (secondTime % 2 == 0) ? 'm' : 'M');
        return false;
    }

    if (counterState == 0x1ffd) {
        showFinishedCountingDown(buf, (secondTime % 2 == 1));
        return true;
    }

    if (counterState == 0x1ffc) {
        display100x(buf, (secondTime % 2 == 0) ? 'h' : 'H');
        return true;
    }

    if (counterState < 0x2000) {
        // three top bits are 000, so lower 13 bits represent a value to render to the display
        if (counterState >= 6000)
            return display100x(buf, 'm');
        return displayTime(buf, counterState, true);
    }

    if (counterState < 0x4000) {
        secondTime &= 0x1fff;
        // three top bits are 001, so lower 13 represent the (potentially rolled-over) time we're counting down towards
        int timeRemaining = (counterState & 0x1fff) - secondTime;
        if (timeRemaining == 0) {
            setValue(0,0x1ffd);
            return showFinishedCountingDown(buf, true);
        }
        if (timeRemaining > 0)
            return displayTime(buf, timeRemaining, true);
        else
            return displayTime(buf, 0x2000 - (secondTime & 0x1fff) + (counterState & 0x1fff), false);
    }

    // top three bits are 011, so lower 13 represent the hh:mm we're counting down towards
    if (counterState < 0x6000) {
            int elapsedMinutes = ((currentTimeMillis / 60000) & 0x1fff) - (counterState & 0x1fff);
            if (elapsedMinutes > 0) {
                if (elapsedMinutes > 21600) {
                    setRaw(0x1ffc);
                    return display100x(buf, 'h');
                }
                return displayTime(buf, elapsedMinutes, (secondTime % 2 == 0));
            }
            else {
                elapsedMinutes = 0x2000 - (counterState & 0x1fff) + ((currentTimeMillis / 60000) & 0x1fff);
                return displayTime(buf, elapsedMinutes, (secondTime % 2) == 0);
            }
            return true;
    }

    if (counterState < 0x8000) {
        int minuteTime = (currentTimeMillis / 60000) & 0x1fff;
        int timeRemaining = (counterState & 0x1fff) - minuteTime;

       if (timeRemaining == 0) {
           setValue(0, 0x1ffd);
           return showFinishedCountingDown(buf, true);
       }
       if (timeRemaining > 0)
           return displayTime(buf, timeRemaining, (secondTime % 2 == 0));
       else
           return displayTime(buf, 0x2000 - ((secondTime / 60) & 0x1fff) + (counterState & 0x1fff), (secondTime % 2 == 0));
    }

    // if 3 msb == 010 or 011, it's reserved... but could potentially be used for something like counting hh:mm instead of mm:ss

    // if msb == 1, the lower 15 bits represent the low 15 bits of the Unixtime (in seconds) we're counting up from (max = 6h / 21600 seconds)
    if (counterState >= 0x8000) {
        int elapsedSeconds = (secondTime & 0x7fff) - (counterState & 0x7fff);
        if (elapsedSeconds > 0) {
            if (elapsedSeconds > 21600) {
                elapsedSeconds = 0x1fff;
                return display6h(buf);
            }
            return displayTime(buf, elapsedSeconds, true);
        }
        else {
            // we rolled over
            elapsedSeconds = 0x7fff - (counterState & 0x7fff) + (secondTime & 0x7fff);
            return displayTime(buf, elapsedSeconds, true);
        }
    }

    // i

    for (int x=0; x<5; x++)
        buf[x] = '?';
    return true;

}

bool TimeCounter5charFrame::isSame(uint32_t currentTimeMillis, uint16_t goal, uint16_t timeRemaining) {
    uint32_t currentTimeSeconds13 = (currentTimeMillis / 1000) & 0x1fff;
    goal &= 0x1fff;
    uint16_t calculatedTimeRemaining = 0;
    if (currentTimeSeconds13 < goal)
        calculatedTimeRemaining = (goal - currentTimeSeconds13);
    else
        calculatedTimeRemaining = 0x2000 - currentTimeSeconds13 + goal;
    return (timeRemaining == calculatedTimeRemaining);
}
