//
// Created by root on 7/14/2022.
//

#include <cstdlib>
#include "CharacterDisplay.h"

CharacterDisplay::CharacterDisplay(const int rows, const int cols) : rows(rows), cols(cols), buf((char*)malloc(rows * (cols + 1))) {

}

char* CharacterDisplay::getRegion(int row, int col) {
    return &buf[row]+col;
}

char* CharacterDisplay::getRow(int row) {
    return &buf[row];
}

void CharacterDisplay::addFrame(BaseFrame* frame) {
    frames.push_back(frame);
}

bool CharacterDisplay::render(int millis) {
    bool isChanged = false;

    for (const auto& frame : frames) {
        isChanged |= frame->render(millis);
    }

    return isChanged;
}