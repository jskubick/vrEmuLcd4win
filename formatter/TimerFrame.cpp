#include <cmath>
#include "TimerFrame.h"
#include "DisplayManager.h"

/*
 *
 * 13 bits = 2:16:31 with 1-second resolution
 * 13 bits = 34:07:45 with 15-second resolution
 * 13 bits = 136h + 31m with 1-minute resolution
 * 12 bits = 42 days with 15-second resolution (remainder discarded)
 * 10 bits = 42 days with 1-minute resolution (remainder discarded)
 *
 *
 *   Suppose I have 24 bits
 *      48 days @ 1sec needs 22 bits: (16.58 million vs 16.77 million)
 *      that leaves me with 3 more sets of 22 bits
 *
 *      I can have:
 *          4 x 48 days @ 1-second resolution (each 48-day chunk needs 22 bits)
 *
 *          3 x 48 days @ 1-second resolution PLUS
 *          16 x 45 days @ 15sec
 *
 *      48 days @ 16sec needs 18 bits
 *      45 days @ 15sec needs 18 bits
 *
 */

/*
 * 24-bit register:
 *      3 x 48 days with 1-second resolution (3 x 22 bits)
 *      16 x 45 days with 15-second resolution (16 x 12 bits)
 *          45 x 24 x 60 x 4 = 259,200 (262,144 = 2^18)
 *      144 + 720 days total
 *
 * 16-bit register: *
 *      000 + 13: 00:00 to 99:99 fixed, plus 2191 additional fixed values
 *      001 + 13: 2h15m counter (enough for 99:99), 1-second resolution
 *      010 + 13: 34h8m counter, 15-second resolution
 *      011 + 13: 136h31m counter, 1-minute resolution
 *      1nnn (nnn < 111) + 12: 7 x 42 days, 15-minute resolution (294 days total)
 *      1111 nn + 10: 4 x 42 day, 1-hour resolution (168 days total)
 *
 *  000 + 13: 00:00 to 99:59 fixed, plus 2191 additional fixed values
 *  001 + 13: 2:15:00  @ 1 second resolution (1m31s surplus)
 *  010 + 13: 34:00:00 @ 15-second resolution (7m42s surplus)
 *  011 + 13: 136h @ 1-minute resolution (31m surplus)
 *      might officially switch to 15-minute resolution at exactly 7 days, which is around 132h into this counter
 *  1nnn (nnn < 111) + 12: 7 x 42 days @ 15-minute resolution (294 days total)
 *  1111 nn + 10: 4 x 42 days @ 1-hour resolution (168 days total)
 */

TimerFrame::TimerFrame(int row, int startCol) : BaseFrame(row, startCol, 5) {}

/**
 * Sets timer's value to 00:00 to 99:59.
 * Class cares only that the total number of seconds < 6000.
 * Setting value to (0,5999) is equivalent to (99,59), because 99*60 + 59 == 5999
 * If value > 5999, sets to
 * @param minutes value between 0 and 99 to display for 'minutes'
 * @param seconds value between 0 and 59 to display for 'seconds'
 * @return true if 0 <= (minutes * 60 + seconds) < 6000
 */
bool TimerFrame::setValue(int minutes, int seconds) {
    int total = (minutes * 60) + seconds;
    if (total < 6000) {
        rawValue = total;
        return true;
    }
    rawValue = TIME_INVALID;
    return false;
}

void TimerFrame::startTimer(uint32_t currentTimeMillis) {
    rawValue = (((uint32_t)round(currentTimeMillis / 1000)) & 0x1fff) | 0x2000;
}

/**
 * Sets raw value to any arbitrary value between 0x0000 and 0xffff
 * Included primarily for use by unit tests and debugging the class itself.
 * @param newValue
 */
void TimerFrame::setRaw(uint16_t newValue) {
    rawValue = newValue;
}

void TimerFrame::startCountingQuarterMinutes(uint32_t currentTimeMillis) {
    uint32_t time = (uint32_t)(currentTimeMillis / 1000) / 15;
    rawValue = (time & 0x1fff) | 0x4000;
}

void TimerFrame::startCountingWholeMinutes(uint32_t currentTimeMillis) {
    uint32_t time = (uint32_t)(currentTimeMillis / 1000) / 60;
    rawValue = (time & 0x1fff) | 0x6000;
}



// ===============================================================================================================

bool TimerFrame::renderMinuteSecond(char* buf, int seconds, uint32_t currentTimeMillis) {
    if (seconds > 5999)
        return renderHourMinute(buf, seconds, currentTimeMillis);

    uint16_t min = seconds / 60;
    uint16_t sec = seconds % 60;
    bool changed = false;

    update(buf+0, getTens(min), changed);
    update(buf+1, getOnes(min), changed);
    update(buf+2, ':', changed);
    update(buf+3, getTens(sec), changed);
    update(buf+4, getOnes(sec), changed);
    return changed;
}

bool TimerFrame::renderHourMinute(char* buf, int seconds, uint32_t currentTimeMillis) {
    bool isChanged = false;
    uint16_t min = (uint16_t) (seconds / 60);
    uint16_t hour = (uint16_t) (seconds / 3600);
    min -= (60 * hour);
    if (hour <= 99) {
        update(buf + 0, getTens(hour), isChanged);
        update(buf + 1, getOnes(hour), isChanged);
        update(buf + 2, blink1hz(currentTimeMillis) ? ':' : ' ', isChanged);
        update(buf + 3, getTens(min), isChanged);
        update(buf + 4, getOnes(min), isChanged);
        return isChanged;
    }

    // ok, for hours > 99, we have to split it into two parts... days, and hh:mm
    if (((currentTimeMillis / 1000) % 4) == 0) {
        // show the number of days with 2hz colon
        uint16_t days = (uint16_t)(((seconds / 15) / 60) / 24);
        bool isChanged = false;
        update(buf+0, getTens(days), isChanged);
        update(buf+1, getOnes(days), isChanged);
        update(buf+2, 'd', isChanged);
        update(buf+3, 'a', isChanged);
        update(buf+4, 'y', isChanged);
        return isChanged;
    }
    else {
        // show 99h23 or 99H23
        uint16_t days = (uint16_t)(((seconds / 15) / 60) / 24);
        uint16_t hours = (uint16_t)(((seconds - (24 * 60 * 15 * days)) / 60) / 60);
        uint16_t minutes = (uint16_t)((seconds - (days * 24 * 60 * 60) - (hours * 60 * 60)) / 60);
        bool isChanged = false;
        update(buf+0, getTens(hours), isChanged);
        update(buf+1, getOnes(hours), isChanged);
        update(buf+2, blink1hz(currentTimeMillis) ? ':' : ' ', isChanged);
        update(buf+3, getTens(minutes), isChanged);
        update(buf+4, getOnes(minutes), isChanged);
        return isChanged;
    }
}

bool TimerFrame::renderTimeNone(char* buf) {
    bool isChanged = false;
    for (int x=0; x<4; x++) {
        isChanged |= (buf[x] != ' ');
        buf[x] = 'x'; // @ToDo: change to space
    }
    return isChanged;
}

bool TimerFrame::renderTimeInvalid(char* buf, bool showColon) {
    bool isChanged = false;
    for (int x : {0,1,3,4}) {
        isChanged |= (buf[x] != '?');
        buf[x] = '?';
    }
    buf[2] = showColon ? ':' : ' ';
    return isChanged;
}

bool TimerFrame::blink1hz(uint32_t currentTimeMillis) {
    return ((currentTimeMillis / 500) % 2) == 0;
}

bool TimerFrame::blink2hz(uint32_t currentTimeMillis) {
    return ((currentTimeMillis / 250) % 2) == 0;
}

bool TimerFrame::blink4hz(uint32_t currentTimeMillis) {
    return ((currentTimeMillis / 125) % 2) == 0;
}

char TimerFrame::getTens(int value) {
    if (value > 99)
        return '?';
    if (value < 0)
        return '?';
    return (char)(0x30 + (value / 10));
}

char TimerFrame::getOnes(int value) {
    if (value < 0)
        return '?';
    return (char)(0x30 + (value % 10));
}

void TimerFrame::update(char* buf, char newValue, bool& isDifferent) {
    isDifferent |= (*buf == newValue);
    *buf = newValue;
}

/** Renders the timer into the furnished buffer, and indicates whether the buffer actually changed.
 *
 * @param buf
 * @param currentTimeMillis
 * @return
 */
bool TimerFrame::renderInto(char* buf, uint32_t currentTimeMillis) {
    buf += (LCD_COLS * row) + startCol;

    if (rawValue == 0x1fff)
        return renderTimeNone(buf);
    if (rawValue == 0x1ffe)
        return renderTimeInvalid(buf, blink1hz(currentTimeMillis));



    // 000 + 13 renders as static value
    if (rawValue < 0x2000) {
        return renderMinuteSecond(buf, rawValue & 0x1fff, 1000);
    }

    // 001 + 13 represents time between 0 and 2h16m
    if (rawValue < 0x4000) {
        uint16_t currentTime = ((uint16_t)(currentTimeMillis / 1000)) & 0x1fff;
        uint16_t startTime = rawValue & 0x1fff;
        uint16_t elapsedTime = (currentTime > startTime) ? currentTime - startTime : (0x2000 - startTime) + currentTime;

        // 8160 is 2:16:00 (31 seconds before rollover)
        if (elapsedTime < 8160) {
            if (currentTime > startTime)
                return renderMinuteSecond(buf, currentTime - startTime, currentTimeMillis);
            else
                return renderMinuteSecond(buf, (0x2000 - startTime) + currentTime, currentTimeMillis);
        }
        else
            startCountingQuarterMinutes(currentTimeMillis);
    }

    // 010 + 13 represents up to 8191 quarter-minutes past 02:16:00
    if (rawValue < 0x6000) {
        // divide by 1000 to get seconds, then 15 to get quarter-minutes
        uint16_t currentTime = ((uint16_t)(currentTimeMillis / 15000)) & 0x1fff;
        uint16_t startTime = rawValue & 0x1fff;
        uint16_t elapsedTime = (currentTime > startTime) ? currentTime - startTime : (0x2000 - startTime) + currentTime;

        // 8160 is 34 hours (8160 quarter-minutes) past 2:16:00
        if (elapsedTime < 8160) {
            return renderHourMinute(buf, elapsedTime * 15, currentTimeMillis);
        }
        else
            startCountingWholeMinutes(currentTimeMillis);
    }

    return renderTimeInvalid(buf, blink4hz(currentTimeMillis));

}

// sunday night:
// seems to work until it wraps around from quarter-minutes to whole-minutes, then needs to be implemented.


