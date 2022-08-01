//
// Created by root on 7/31/2022.
//

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
    // if msb is set, it means we've overflowed & are no longer counting.
    uint16_t counterState = 0x0000;
};


#endif //FAKELCD_TIMECOUNTER5CHARFRAME_H
