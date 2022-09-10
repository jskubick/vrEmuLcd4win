//
// Created by root on 8/9/2022.
//

#include <cmath>
#include <iostream>
#include "Timekeeper.h"

Timekeeper::Timekeeper() {}

void Timekeeper::startCountingQuarterMinutes(uint32_t milliTime) {
    rawValue = ((milliTime / 6000) & 0x1fff) | 0x4000;
}

void Timekeeper::startCountingMinutes(uint32_t milliTime) {

}

void Timekeeper::startCountingQuarterHours(uint8_t period, uint32_t milliTime) {

}

void Timekeeper::startCountingHours(uint8_t period, uint32_t milliTime) {

}

void Timekeeper::noteOverflow() {

}


uint16_t Timekeeper::packDown(uint32_t milliseconds, int multiplier, int bits){
    uint32_t kibiMillis = (uint32_t)(milliseconds / (1024 * multiplier));
    uint32_t mask = (uint32_t)(pow(2, bits)-1);
    return (uint16_t)(kibiMillis & mask);
}





/** Starts the timer from 0d 00:00:00, 1-second per tick.
     *
     * @param milliTime
     */
void Timekeeper::restart(uint32_t milliTime) {
    auto kbms = round(milliTime / 1024.0);
    auto binSeconds = (uint32_t)kbms;
    rawValue = binSeconds & 0x1fff;
}

uint32_t Timekeeper::countTicks(uint32_t milliTime, uint32_t kbmsMultiplier, uint32_t millisPerTick, int bits) {
    uint32_t scale = kbmsMultiplier * 1024;
    uint32_t mask = pow(2,bits)-1;

    auto scaledKbmsTime =milliTime / (1024 * kbmsMultiplier);
    auto kbmsTime = ((uint64_t)scaledKbmsTime) & mask;

    uint64_t startTime = rawValue & mask;
    if (kbmsTime == startTime)
        return 0;
    else if (kbmsTime > startTime)
        return (uint32_t)(((kbmsTime - startTime) * scale) / millisPerTick);
    else
        return (uint32_t)((((pow(2,bits) - startTime) + kbmsTime)*scale) / millisPerTick);
}

uint32_t Timekeeper::countRoundedTicks(uint32_t milliTime, uint32_t kbmsMultiplier, uint32_t millisPerTick, int bits) {
    float scale = kbmsMultiplier * 1024.0;
    uint32_t mask = pow(2,bits)-1;

    auto scaledKbmsTime = round(milliTime / (1024.0 * kbmsMultiplier));
    auto kbmsTime = ((uint64_t)scaledKbmsTime) & mask;

    uint64_t startTime = rawValue & mask;
    if (kbmsTime == startTime)
        return 0;
    else if (kbmsTime > startTime)
        return (uint32_t)(((kbmsTime - startTime) * scale) / millisPerTick);
    else
        return (uint32_t)((((pow(2,bits) - startTime) + kbmsTime)*scale) / millisPerTick);
}



/**
 * Only intended for unit tests.
 * If you're using this for something else, I guarantee there's a better method to use!
 * @return
 */
uint16_t Timekeeper::getRawValue() {
    return rawValue;
}

/**
 * Only intended for unit tests.
 * Make sure you call this method BEFORE calling anything that might change the underlying raw value (like getElapsedTime(millis)).
 * @return if positive, number of seconds per tick. If negative, the timer isn't running & it's just the negated lower 10-bit value.
 */
int Timekeeper::getTickDuration() {
    if (rawValue < 0x2000)
        return 1; // one second per tick
    if (rawValue < 0x4000)
        return 15; // 15 seconds per tick
    if (rawValue < 0x8000)
        return 60; // 1 minute per tick
    if (rawValue < 0xE000)
        return 900; // 15 minutes per tick
    if (rawValue < 0xFC00)
        return 3600; // 1 hour per tick
    return -1 * (rawValue & 0x3ff); // negative means timer isn't running, and value is absolute
}

uint32_t Timekeeper::getAdjustedElapsedTime(uint32_t milliTime) {
    if (milliTime < 10)
        std::cout << "break";
    if (rawValue < 0x2000) {

        uint32_t ticks = countTicks(milliTime, 1, 1000, 13);


        int bluntedStartTime = (rawValue & 0x1fff) * 1024;
        int32_t subMilliTime = milliTime % 0x800000;
        int elapsed = (subMilliTime > bluntedStartTime) ? subMilliTime - bluntedStartTime : (0x800000 - bluntedStartTime) + subMilliTime;

        int tickTime = ticks * 1000;
        int diff = elapsed-tickTime;
        //if ((ticks==1) || (ticks==41) || (ticks==42) || (ticks == 43))
        //  std::cout << std::hex << milliTime << "\t" << ticks << "\t" << bluntedStartTime << "\t" << subMilliTime << "\t" << diff << "\n";

        /*
         * If milliTime can range from 0..2^32-1
         * if kibiMilli10bitTime is (milliTime * 1024) & 0x1fff
         *
         * 8191 * 1024 = 8,387,594
         * 8192 * 1024 = 8,388,608 (1,014)
         * 8388608 / 1024 = 8192 = 0x2000
         *
         * so... divide milliseconds by 8388608
         */


        if (ticks > 8160) {
            uint32_t newStartTime = (milliTime / 16384) & 0x1fff;
            rawValue = 0x2000 | newStartTime;
            std::cout << "\n************** switched gears to " << rawValue << "\n";
        }

        return (diff < 1000) ? ticks : ticks+1;
    }

    if (rawValue < 0x4000) {
        uint32_t ticks = countTicks(milliTime, 16, 15000, 13);
        if (ticks > 8190) {
            uint32_t newStartTime = (milliTime / 65536) & 0x3fff;
            rawValue = 0x4000 | newStartTime;
            std::cout << "\n************ switched gears to " << rawValue << " at " << milliTime << "\n";
        }
        return 8160 + (15 * ticks);

    }

    return (uint32_t) 0xfffe;
}

uint32_t Timekeeper::getElapsedTime(uint32_t milliTime) {
    if (rawValue < 0x2000) {
        uint32_t ticks = countTicks(milliTime, 1, 1000, 13);

        int bluntedStartTime = (rawValue & 0x1fff) * 1024;
        int32_t subMilliTime = milliTime % 0x800000;
        int elapsed = subMilliTime - bluntedStartTime;
        int tickTime = ticks * 1000;
        int diff = elapsed-tickTime;
        //if ((ticks==1) || (ticks==41) || (ticks==42) || (ticks == 43))
          //  std::cout << std::hex << milliTime << "\t" << ticks << "\t" << bluntedStartTime << "\t" << subMilliTime << "\t" << diff << "\n";
        if (diff > 1000)
            std::cout << ticks << "\n";
        /*
         * If milliTime can range from 0..2^32-1
         * if kibiMilli10bitTime is (milliTime * 1024) & 0x1fff
         *
         * 8191 * 1024 = 8,387,594
         * 8192 * 1024 = 8,388,608 (1,014)
         * 8388608 / 1024 = 8192 = 0x2000
         *
         * so... divide milliseconds by 8388608
         */


        if (ticks > 8160) {
            uint32_t newStartTime = (milliTime / 16384) & 0x1fff;
            rawValue = 0x2000 | newStartTime;
            std::cout << "\n************** switched gears to " << rawValue << "\n";
        }

        return (diff < 1000) ? ticks : ticks+1;
    }

    if (rawValue < 0x4000) {
        uint32_t ticks = countTicks(milliTime, 16, 15000, 13);
        if (ticks > 8190) {
            uint32_t newStartTime = (milliTime / 65536) & 0x3fff;
            rawValue = 0x4000 | newStartTime;
            std::cout << "\n************ switched gears to " << rawValue << " at " << milliTime << "\n";
        }
        return 8160 + (15 * ticks);

    }

    return (uint32_t) 0xfffe;
}

uint32_t Timekeeper::getRoundedElapsedTime(uint32_t milliTime) {
    if (rawValue < 0x2000) {
        uint32_t ticks = countRoundedTicks(milliTime, 1, 1000, 13);

        if (ticks > 8160) {
            uint32_t newStartTime = (milliTime / 16384) & 0x1fff;
            rawValue = 0x2000 | newStartTime;
            std::cout << "\n************** switched gears to " << rawValue << "\n";
        }
        return ticks;
    }

    if (rawValue < 0x4000) {
        uint32_t ticks = countRoundedTicks(milliTime, 16, 15000, 13);
        if (ticks > 8190) {
            uint32_t newStartTime = (milliTime / 65536) & 0x3fff;
            rawValue = 0x4000 | newStartTime;
            std::cout << "\n************ switched gears to " << rawValue << " at " << milliTime << "\n";
        }
        return 8160 + (15 * ticks);

    }

    return (uint32_t) 0xfffe;
}
