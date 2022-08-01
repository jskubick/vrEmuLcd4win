#ifndef FAKELCD_TIMECOUNTER5CHARFRAME_H
#define FAKELCD_TIMECOUNTER5CHARFRAME_H


#include "BaseFrame.h"

class TimeCounter5charFrame : public BaseFrame {
public:
    TimeCounter5charFrame(int row, int startCol);
    void startCountingFromZero(uint32_t currentTimeMillis);
    void startCountdown(int minutes, int seconds, uint32_t currentTimeMillis);
    void setValue(int minutes, int seconds);
    bool renderInto(char* buffer, uint32_t currentTimeMillis);
private:
    /**
     * 000: counterState's low 13 bits hold seconds to render to mm:ss
     *      0x1ffe renders as "100m+"
     *      0x1fff renders as "(6h+)"
     * 001: counterState's low 13 bits hold time we're counting down to
     * 01x: reserved (probably for hh:mm count-up and count-down)
     * 1xx: counterState has start time and frame is counting up
     *      (max is 6h/21600 seconds, which displays as z9:59)
     */
    uint16_t counterState = 0x0000;
};


#endif //FAKELCD_TIMECOUNTER5CHARFRAME_H
