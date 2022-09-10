//
// Created by root on 8/5/2022.
//

#ifndef FAKELCD_TIMERFRAME_H
#define FAKELCD_TIMERFRAME_H

#include <cstdint>
#include "BaseFrame.h"

#define RES_1_SEC 1
#define RES_15_SEC 15
#define RES_1_MIN 60
#define RES_15_MIN 900
#define RES_1_HOUR 3600

// renders to "     "
#define TIME_NONE 0x1fff
// renders to "??:??"
#define TIME_INVALID 0x1ffe

class TimerFrame : public BaseFrame {
public:
    TimerFrame(int row, int startCol);
    bool setValue(int minutes, int seconds);
    void startTimer(uint32_t currentTimeMillis);
    void setRaw(uint16_t);
    bool renderInto(char* buf, uint32_t currentTimeMillis) override;

private:
    uint16_t rawValue = TIME_NONE; // 0x1fff, renders to "  :  " with steady colon
    bool renderMinuteSecond(char* buf, int seconds, uint32_t currentTimeMillis);
    bool renderHourMinute(char* buf, int seconds, uint32_t currentTimeMillis);
    bool renderTimeNone(char* buf);
    bool renderTimeInvalid(char* buf, bool showColon);
    bool blink1hz(uint32_t currentTimeMillis);
    bool blink2hz(uint32_t currentTimeMillis);
    bool blink4hz(uint32_t currentTimeMillis);
    static char getTens(int value);
    static char getOnes(int value);
    static void update(char* buf, char newValue, bool& isDifferent);
    void startCountingQuarterMinutes(uint32_t currentTimeMillis);
    void startCountingWholeMinutes(uint32_t currentTimeMillis);
};


#endif //FAKELCD_TIMERFRAME_H
