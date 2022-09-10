//
// Created by root on 8/15/2022.
//

#include "MmSsFrame.h"
#include "DisplayManager.h"
MmSsFrame::MmSsFrame(int row, int startCol) : BaseFrame(row, startCol, 5) {}

void MmSsFrame::setValue(uint32_t newValue) {
    seconds = newValue;
}

bool MmSsFrame::renderInto(char* buffer, uint32_t currentTimeMillis) {
    buffer += (LCD_COLS * row) + startCol;
    int secondTime = seconds % 60;
    int minuteTime = seconds / 60;
    buffer[0] = (minuteTime > 9) ? (char)(0x30 + (minuteTime / 10)) : '0';
    buffer[1] = (minuteTime > 0) ? (char)(0x30 + (minuteTime % 10)) : '0';
    buffer[2] = (((currentTimeMillis / 1000) % 2) == 0) ? ':' : ' ';
    buffer[3] = (secondTime > 9) ? (char)(0x30 + (secondTime / 10)) : '0';
    buffer[4] = (secondTime > 0) ? (char)(0x30 + (secondTime % 10)) : '0';
    return true;
}