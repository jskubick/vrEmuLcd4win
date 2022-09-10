//
// Created by root on 8/4/2022.
//

#ifndef FAKELCD_TIMER5CHARFRAME_H
#define FAKELCD_TIMER5CHARFRAME_H


#include "BaseFrame.h"

class Timer5charFrame : public BaseFrame {
public:
    Timer5charFrame(int row, int startCol);
    void set(int left, int right);
    void clearAndStart(uint32_t currentTimeMillis);

    void countFrom100m(uint32_t currentTimeMillis);
    void continueCountingMinutes(uint32_t currentTimeMillis);
    void switchToCountingQuarterHours(uint32_t currentTimeMillis);
private:
    uint16_t state = 0x1FFF;
};


#endif //FAKELCD_TIMER5CHARFRAME_H
