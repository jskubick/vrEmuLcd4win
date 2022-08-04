//
// Created by root on 8/2/2022.
//

#include <cstdlib>
#include "HexFrame.h"
#include "DisplayManager.h"

HexFrame::HexFrame(int row, int col, uint8_t digits) : BaseFrame(row, col, 8), totalDigits(digits) { }

void HexFrame::set(int64_t newValue) {
    rawValue = (uint32_t)(newValue & 0xffffffff);
}

bool HexFrame::renderInto(char* buf, uint32_t currentTimeMillis) {
    char buffer[10];
    itoa(rawValue, buffer, 16);


    for (int x=0; x<totalDigits; x++)
        *(buf + (LCD_COLS * row) + startCol + x) = buffer[x];
    return true;
}