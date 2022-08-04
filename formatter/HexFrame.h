//
// Created by root on 8/2/2022.
//

#ifndef FAKELCD_HEXFRAME_H
#define FAKELCD_HEXFRAME_H


#include "BaseFrame.h"

class HexFrame : public BaseFrame {
public:
    HexFrame(int row, int startCol, uint8_t totalDigits);
    void set(int64_t);
    bool renderInto(char* buf, uint32_t currentTimeMillis);
private:
    uint32_t rawValue;
    uint8_t totalDigits;
};


#endif //FAKELCD_HEXFRAME_H
