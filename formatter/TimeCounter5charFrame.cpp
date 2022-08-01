#include <iostream>
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

// @ToDo: implement startCountdown()
    // top 3 bits are 001

void TimeCounter5charFrame::setValue(int minutes, int seconds) {
    // top 3 bits are 000
    // when counterState's three top bits are clear, the bottom 13 bits hold a static value.
    // if the value is between 0 and 5999, it renders to the display as a value between 00:00 and 99:59
    // if the value is between 6000 and 21600, it renders to the display as a value between a0:00 and z9:59
    // if the value is 0x1fff, it renders to the display as "(6h+)"
    // if the value is 0x1ffe, it renders to the display as "100m+"
    counterState = ((minutes * 60) + seconds) & 0x3FFF;
}

bool display6h(char* buf) {
    *buf++='(';
    *buf++='6';
    *buf++='h';
    *buf++='+';
    *buf++=')';
    return true;
}

bool display100m(char* buf) {
    *buf++ = '1';
    *buf++ = '0';
    *buf++ = '0';
    *buf++ = 'm';
    *buf++ = '+';
    return true;
}



bool TimeCounter5charFrame::renderInto(char* buf, uint32_t currentTimeMillis) {
    buf += (LCD_COLS * row) + startCol;

    // 3 msb = 000, so it's a static value. 13LSB = 1fff, so we overflowed after 6 hours at some point in the past.
    if (counterState == 0x1fff) {
        display6h(buf);
        return false;
    }

    // 3 msb = 000, so it's a static value. 13LSB = 1ffe, so we overflowed after 99:59 at some point in the past.
    if (counterState == 0x1ffe) {
        display100m(buf);
        return false;
    }

    if (counterState < 0x1fff) {
        // three top bits are 000, so lower 13 bits represent a value to render to the display
        int minutes = counterState / 60;
        if (minutes < 10) {
            buf[0] = '0';
        }
        else if (minutes < 100) {
            buf[0] = (uint8_t)((minutes/10) + 0x30);
        }
        else if (minutes < 360) {
            buf[0] = (uint8_t)((minutes / 10) - 10 + 0x41);
        }
        buf[1] = (uint8_t)(minutes % 10) + 0x30;

        buf[2] = ':';

        int seconds = counterState % 60;
        if (seconds < 10) {
            buf[3] = '0';
        }
        else {
            buf[3] = (uint8_t)(seconds / 10) + 0x30;
        }
        buf[4] = (uint8_t)(seconds % 10) + 0x30;
        return true;
    }

    // if 3 msb == 001, lower 13 bits represent the (potentially rolled-over) time we're counting down towards

    // if 3 msb == 010 or 011, it's reserved... but could potentially be used for something like counting hh:mm instead of mm:ss

    // if msb == 1, the lower 15 bits represent the low 15 bits of the Unixtime (in seconds) we're counting up from (max = 6h / 21600 seconds)

    for (int x=0; x<5; x++)
        buf[x] = '?';
    return true;

}
