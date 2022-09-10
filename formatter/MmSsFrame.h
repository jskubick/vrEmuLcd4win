//
// Created by root on 8/15/2022.
//

#ifndef FAKELCD_MMSSFRAME_H
#define FAKELCD_MMSSFRAME_H

#include "BaseFrame.h"

class MmSsFrame : public BaseFrame {
public:
    MmSsFrame(int row, int startCol);
    void setValue(uint32_t seconds);
    bool renderInto(char* buffer, uint32_t currentTimeMillis);
private:
    uint32_t seconds = 0;
};


#endif //FAKELCD_MMSSFRAME_H
