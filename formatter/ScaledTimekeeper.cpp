/**
 * A ScaledTimekeeper keeps (approximate) track of the passage of time for more than a year using only 2 bytes of RAM
 * to maintain its state between calls.
 *
 * OK, obviously, the class itself takes RAM if you're using it on a platform that doesn't execute directly from flash
 * (I developed it primarily with 8-bit AVR Arduino in mind), and it uses up to a few dozen bytes of RAM at a time
 * during execution... but as far as I know, all of THAT RAM comes off the stack, not the heap, so it won't contribute
 * to long-term heap fragmentation. If you happen to stumble over something in my code that DOES result in heap allocation,
 * PLEASE file a highest-priority bug report.
 *
 * The diminishing resolution is my best attempt to strike a balance between "needing to know how much time has elapsed"
 * and "being able to keep track of potentially long periods of time". My rationale was, "the more time that's elapsed
 * since the event occurred, the less the RESOLUTION of the timestamp really matters". Not to mention, if you're using
 * millis() on an Arduino without a crystal, you'll be lucky to approach anything CLOSE to my scaled resolution after
 * a few months, anyway.
 *
 *
 *  000+13: 0:00 to 99:59 @ 1s (100 minutes)
 *  001+13: 1:40 to 23:59:50 @ 10s (22h20m, gets us to exactly 1 day
 *  01 +14: 1d 00:00:00 to 12d 23:59:59 @ 1m (exactly 11 days, ends exactly 12d 23:59:59 after starting)
 *  1xxz+12: 7 periods, each exactly 42 days long @ 15m resolution
 *  1111xz+10: 3 periods, each exactly 42 days long @ 1h resolution
 *  111111+10: static value
 *      can do 00:00 to 99:55 in 5-second increments
 *      can do 00:00 to 00:59 in 1-second increments
 *      6 additional values
 *          0xffff = "no time" renders to "     "
 *          0xfffe = "invalid time" renders to "??:??"
 *          0xfffd = "out of time" renders to "432d+" and "ovflw"
 *          0xfffc
 *          0xfffb
 *          0xfffa
 *
 *      to test Arduino millis-overflow handling, do this:
 *          noInterrupts();
 *          timer0_millis = 4294901760; // set timer counter to 45 seconds before overflow
 *          interrupts();
*/

#include <cstdint>



class DeprecatedTimekeeper {
public:
    DeprecatedTimekeeper() {}

    /** Starts the timer from 0d 00:00:00, 1-second per tick.
     *
     * @param milliTime
     */
    void restart(uint32_t milliTime) {
        value = ((milliTime / 1000) & 0x1fff);
    }

    void update(uint32_t milliTime) {
        if (value >= 0xFC00)
            return;

        if (value < 0x2000) {
            uint16_t secondsElapsed = getSecondsElapsed(milliTime);
            if (secondsElapsed >= 6000) // count up to exactly 1h40m
                startCountingQuarterMinutes(milliTime);
            return;
        }

        if (value < 0x4000) {
            // we're counting tenSeconds (1/6 minute)
            uint16_t tensecTime = (uint16_t)((milliTime / 6000) & 0x1fff);
            uint16_t tensecElapsed = (tensecTime > (value & 0x1fff)) ? tensecTime - (value & 0x1fff) : (0x2000 - (value & 0x1fff) + tensecTime);
            // 22h x 60m x 6s = 7920
            if (tensecElapsed >= 7920)
                startCountingMinutes(milliTime);
            return;
        }

        if (value < 0x8000) {
            // we're counting  minutes, from 0x4000 to 7FFF
            // 1 minute = 1000ms/sec x 60 sec/minute = 60000
            uint16_t minuteTime = (uint16_t)((milliTime / 60000) & 0x0fff);
            uint16_t minutesElapsed = (minuteTime > (value &0x0fff)) ? minuteTime - (value & 0x0fff) : (0x4000 - (value & 0x0fff) + minuteTime);
            // 11d x 24h x 60m = 0x3de0 = 15,840
            if (minutesElapsed >= 15840)
                startCountingQuarterHours(0, milliTime);
            return;
        }

        if (value < 0xf000) {
            // we're counting seven 42-day periods of 4032 quarter-hours, from n000 to nfc0
            uint8_t period = value & 0x7000;
            period /= 0x1000;
            // 15 minutes = 1000ms/sec * 60sec/min * 15min/period = 900,000
            uint16_t quarterHourTime = (uint16_t) ((milliTime /  900000) & 0x3ff);
            // @ToDo: 0x3ff is almost certainly the wrong value to use below
            uint16_t quarterHoursElapsed = (quarterHourTime > (value & 0x3ff)) ? quarterHourTime - (value & 0x3ff) : (0x400 - (value & 0x3ff) + quarterHoursElapsed);
            if (quarterHoursElapsed >= 0xfc0) {
                if (period == 7)
                    startCountingHours(0, milliTime);
                else
                    startCountingQuarterHours(period+1, milliTime);
            }
            return;
        }

        if (value <0xfc00) {
            // we're counting three 42-day periods of 24 hours, from 0 to 0x3f0 (1008)
            uint8_t period = value & 0xC00;
            period /= 0x400;
            // 1 hour = 1000ms/sec * 60 sec/min * 60 min/hour = 3,600,000
            uint16_t hourTime = (uint16_t) ((milliTime / 3600000) & 0x3ff);
            uint16_t hoursElapsed = (hourTime > (value & 0x3ff)) ? hourTime - (value & 0x3ff) : (0x400 -
                                                                                                 (value & 0x3ff) +
                                                                                                 hourTime);
            if (hoursElapsed >= 0x3f0) {
                if (period == 3)
                    noteOverflow();
                else
                    startCountingHours(period + 1, milliTime);
            }
        }
    }

    uint32_t getElapsedTime(uint32_t milliTime) {
        update(milliTime);

        if (value < 0x2000) {
            return getSecondsElapsed(milliTime);
        }

        return (uint32_t) 0xfffe;
    }



private:
    uint16_t value;
    void startCountingQuarterMinutes(uint32_t milliTime) {
        value = ((milliTime / 6000) & 0x1fff) | 0x4000;
    }

    void startCountingMinutes(uint32_t milliTime) {

    }

    void startCountingQuarterHours(uint8_t period, uint32_t milliTime) {

    }

    void startCountingHours(uint8_t period, uint32_t milliTime) {

    }

    void noteOverflow() {

    }

    uint16_t getSecondsElapsed(uint32_t milliTime) {
        // we're counting seconds, format is 000+13
        uint16_t startTime = value & 0x1fff;
        uint16_t secondTime = (uint16_t)((milliTime / 1000) & 0x1fff);
        if (secondTime == startTime)
            return 0;
        if (secondTime > startTime)
            return secondTime - startTime;
        return 0x2000 - startTime + secondTime;
    }
};


