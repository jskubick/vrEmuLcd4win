//
// Created by root on 8/4/2022.
//

#include <cmath>
#include "Timer5charFrame.h"

/*
 * 000 + 13: timerState is fixed value
 *      0 to 5999 renders as 00:00 to 99:59
 *      values between 6000 and 8191 are reserved.
 * 001 + 13: timer is counting seconds and rendering them as 00:00 to 99:59 (steady colon)
 * 010 + 13: timer is counting minutes and rendering them as 00:00 to 99:59 (1hz colon)
 * 011 + 13: timer is counting minutes and rendering them as "99day" and "23:59" (2hz colon)
 * 1xxx + 12: (x == 000 to 110)
 *          timer is counting 15-minute intervals in ~42-day chunks (~336 days)
 *          and rendering them as "999d+"  and "23h0-" (0/3/6/9)
 * 1111x + 11: timer is counting 30-minute intervals in ~42-day chunks
 *  Due to round-off/truncation,
 *      will lose up to 1 minute when transitioning from counting seconds to minutes
 *      will lose up to 15 minutes when transitioning from counting minutes to quarterHours
 *      will lose up to 30 minutes when transitioning from counting quarterHours to halfHours
 */

Timer5charFrame::Timer5charFrame(int row, int startCol) : BaseFrame(row, startCol, 5) {}

/** Set timer/counter to hh:mm or mm:ss.
 *
 * @param left value between 0 and 99
 * @param right value between 0 and 59
 */
void Timer5charFrame::set(int left, int right) {
    state = ((left * 60) + right) & 0x1fff;
}

/** sets timer to 00:00 and begins counting to 99:59 by second
 *
 * @param currentTimeMillis
 */
void Timer5charFrame::clearAndStart(uint32_t currentTimeMillis) {
    uint32_t currentTimeSeconds = (uint32_t) round(currentTimeMillis / 1000.0);
    state = 0x2000 | (currentTimeSeconds & 0x1fff);
}

/**
 * sets timer to 01:40 and begins counting up to 99:59 by minute, then
 * continues from 1h40 to 5d18h11m, rendered as "5day+" and "18:11" (2hz colon)
 * @param currentTimeMillis
 */
void Timer5charFrame::countFrom100m(uint32_t currentTimeMillis) {
    uint32_t currentTimeMinutes = (uint32_t)round(currentTimeMillis / 60000.0);
    state = 0x4000 | (currentTimeMinutes & 0x1fff);
}

/**
 * continues counting minutes from 136h11m (5d18h11m) to 274h42m
 * rendered as "11day" and "10:42" (2hz colon)
 * @param currentTimeMillis
 */
void Timer5charFrame::continueCountingMinutes(uint32_t currentTimeMillis) {
    uint32_t currentTimeMinutes = (uint32_t)round(currentTimeMillis / 60000.0);
    state = 0x6000 | (currentTimeMinutes & 0x1fff);
}

/**
 * continues counting quarter-hours from 11d10h45m (gains 3m, then loses up to 15m 0-15m later)
 * @param currentTimeMillis
 */
void Timer5charFrame::switchToCountingQuarterHours(uint32_t currentTimeMillis) {
    uint32_t currentTimeQuarterHours = (uint32_t)round(currentTimeMillis / 900000.0);
    state = 0x8000 | (currentTimeQuarterHours & 0x0FFF);
}


