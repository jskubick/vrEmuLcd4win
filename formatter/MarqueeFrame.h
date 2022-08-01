//
// Created by root on 7/25/2022.
//

#ifndef FAKELCD_MARQUEEFRAME_H
#define FAKELCD_MARQUEEFRAME_H


#include "BaseFrame.h"

class MarqueeFrame : public BaseFrame {
public:
    MarqueeFrame(int row, int startCol, int displayWidth, int totalChars, const char *content,
                 int decaMillisPerChar, int whitespacePauseMillis, int endPauseMillis);
    bool renderInto(char* target, uint32_t currentTimeMillis);
private:
    char* content;
    uint8_t totalChars;
    uint8_t maxChars;
    uint8_t decaMillisPerChar;
    uint8_t decaMillisPerWhitespace;
    uint8_t decaMillisPerEnd;
    short currentFirstChar = 0;
    uint16_t timeWhenCurrentCharBecameFirst = 0;
};


#endif //FAKELCD_MARQUEEFRAME_H
