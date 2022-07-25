#include "StringFrame.h"

#include <iostream>

StringFrame::StringFrame(int row, int startCol, int width, char* content) : BaseFrame(row, startCol, width) {
    stringContent = (char*)malloc(width);
    for (int x=0; x<width; x++)
        stringContent[x] = content[x];
}

bool StringFrame::renderInto(char* target, int currentTime) {
    for (int x=0; x<width; x++) {
        *((target + (row * LCD_COLS) + startCol + x)) = stringContent[x];
    }
    return true;
}

void StringFrame::update(char* newContent) {
    for (int x=0; x<width; x++)
        stringContent[x] = newContent[x];
}
