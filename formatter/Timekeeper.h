//
// Created by root on 8/9/2022.
//

#ifndef SCALEDTIMEKEEPER_TIMEKEEPER_H
#define SCALEDTIMEKEEPER_TIMEKEEPER_H

#include <cstdint>


class Timekeeper {
public:
    Timekeeper();
    void restart(uint32_t milliTime);
    uint32_t getElapsedTime(uint32_t milliTime);
    uint32_t getRoundedElapsedTime(uint32_t milliTime);
    uint32_t getAdjustedElapsedTime(uint32_t milliTime);

    static uint16_t packDown(uint32_t milliseconds, int multiple, int bits);
    uint32_t countTicks(uint32_t milliTime, uint32_t kbmsMultiplier, uint32_t millisPerTick, int bits);

    uint16_t getRawValue();
    int getTickDuration();

private:
    uint16_t rawValue;
    void startCountingQuarterMinutes(uint32_t milliTime);
    void startCountingMinutes(uint32_t milliTime);
    void startCountingQuarterHours(uint8_t period, uint32_t milliTime);
    void startCountingHours(uint8_t period, uint32_t milliTime);
    void noteOverflow();
    uint32_t countRoundedTicks(uint32_t milliTime, uint32_t kbmsMultiplier, uint32_t millisPerTick, int bits);




};


#endif //SCALEDTIMEKEEPER_TIMEKEEPER_H
