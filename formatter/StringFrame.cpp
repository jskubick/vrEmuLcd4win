//
// Created by root on 7/15/2022.
//

#include "StringFrame.h"
StringFrame::StringFrame(char* pBuf, int length) : BaseFrame(pBuf, length) {
}

bool StringFrame::render(int millis) {
    //@ToDo: Temporary hack to alternate StringFrame's value between two strings
    if (millis - lastChange < 1)
        return false;
    lastChange = millis;
    if (buf[0] == 'f')
        copy("bar", buf, 3);
    else
        copy("foo", buf, 3);
    return true;
}
