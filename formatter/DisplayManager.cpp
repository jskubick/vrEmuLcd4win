#include "DisplayManager.h"

DisplayManager::DisplayManager() {
    for (int y=0; y<LCD_ROWS; y++) {
        for (int x=0; x<LCD_COLS; x++) {
            buf[y][x]=0x20;
        }
    }
}

char* DisplayManager::charAt(int row, int col) {
    return &buf[row][col];
}

char* DisplayManager::getRow(int row) {
    return &buf[row][0];
}

BaseFrame* DisplayManager::addFrame(BaseFrame* frame) {
    frames.push_back(frame);
    return frame;
}

bool DisplayManager::render(int millis) {
    bool isChanged = false;

    for (const auto& frame : frames) {
        isChanged |= frame->renderInto(&buf[0][0], millis);
    }

    return isChanged;
}

