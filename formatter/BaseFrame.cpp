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

bool BaseFrame::renderInto(char* buf, uint32_t currentTimeMillis) {
    return false;
}

int BaseFrame::strcopy(char* src, char* dest, int len, bool clearRemainder) {
    bool pastEnd = false;
    for (int x=0; x<len; x++) {
        pastEnd |= (*(src+x) == 0);
        if (pastEnd) {
            if (clearRemainder)
                *(dest+x) = 0x20;
            else
                return x+1;
        }
        else {
            *(dest+x) = *(src+x);
        }
    }
    return len;
}

int BaseFrame::strcopy(const char* src, char* dest, int len, bool clearRemainder) {
    bool pastEnd = false;
    for (int x=0; x<len; x++) {
        pastEnd |= (*(src+x) == 0);
        if (pastEnd) {
            if (clearRemainder)
                *(dest+x) = 0x20;
            else
                return x;
        }
        else {
            *(dest+x) = *(src+x);
        }
    }
    return len;
}
