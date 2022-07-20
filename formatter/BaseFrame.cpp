//
// Created by root on 7/14/2022.
//

#include "BaseFrame.h"

BaseFrame::BaseFrame(char* pBuf, int len) : buf(pBuf), maxLength(len) {

}

void BaseFrame::update(char* newValue) {
    int offset = 0;
    while ((offset < maxLength) && (newValue[offset] > 0)) {
        buf[offset] = newValue[offset];
        offset++;
    }
}

bool BaseFrame::render(int millis) {
    return false;
}

bool BaseFrame::copy(char* src, char* dest, int len) {
    int x=0;
    bool isChanged = false;
    while ((x<len) && (src[x] > 0)) {
        isChanged |= (dest[x] == src[x]);
        dest[x] = src[x];
        x++;
    }
    return isChanged;
}