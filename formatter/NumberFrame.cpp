//
// Created by root on 7/28/2022.
//

#include "NumberFrame.h"
#include "DisplayManager.h"

NumberFrame::NumberFrame(int row, int startCol, int width, BaseNumberRenderer* pRenderer) : BaseFrame(row, startCol, width), pRenderer(pRenderer) {

}

bool NumberFrame::renderInto(char* target, int currentTimeMillis) {
    pRenderer->renderValue(target + (row * LCD_COLS) + startCol);
    return true;
}

void NumberFrame::set(float newValue) {
    pRenderer->setValue(newValue);
}