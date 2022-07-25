#include "BaseFrame.h"

BaseFrame::BaseFrame(int row, int startCol, int width) : row((uint8_t)row), startCol((uint8_t)startCol), width((uint8_t)width) {}

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

bool BaseFrame::renderInto(char* buf, int currentTimeMillis) {
    return false;
}
