#ifndef FAKELCD_TIMECOUNTER5CHARFRAME_H
#define FAKELCD_TIMECOUNTER5CHARFRAME_H


#include "BaseFrame.h"

class TimeCounter5charFrame : public BaseFrame {
public:
    TimeCounter5charFrame(int row, int startCol);
    void startCountingFromZero(uint32_t currentTimeMillis);
    void startSlowlyCountingFromZero(uint32_t currentTimeMillis);
    void startCountdown(int minutes, int seconds, uint32_t currentTimeMillis);
    void startLongCountdown(int hours, int minutes, uint32_t currentTimeMillis);
    void setValue(int minutes, int seconds);
    bool renderInto(char* buffer, uint32_t currentTimeMillis);
    void setRaw(uint16_t rawValue);
    uint16_t getRaw();
    uint32_t test(uint32_t currentTimeMillis, uint16_t totalSeconds);
    bool isSame(uint32_t currentTimeMillis, uint16_t goal, uint16_t timeRemaining);
private:
    /**
     * 000: counterState's low 13 bits hold seconds to render to mm:ss

     *      0x1fff renders as "(6h+)"
     *      0x1ffe renders as "100m+"
     *      0x1ffd renders as "--:--"
     *      0x1ffc renders as "100h+
     * 001: counterState's low 13 bits hold time we're counting down to
     * 01x: reserved (probably for hh:mm count-up and count-down)
     * 1xx: counterState has start time and frame is counting up
     *      (max is 6h/21600 seconds, which displays as z9:59)
     */
    uint16_t counterState = 0x0000;
};


#endif //FAKELCD_TIMECOUNTER5CHARFRAME_H
