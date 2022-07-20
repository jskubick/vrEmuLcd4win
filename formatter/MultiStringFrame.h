//
// Created by root on 7/15/2022.
//

#ifndef FAKELCD_MULTISTRINGFRAME_H
#define FAKELCD_MULTISTRINGFRAME_H


#include "BaseFrame.h"

class MultiStringFrame : public BaseFrame {
public:
    MultiStringFrame(char* pBuf, int length, int totalStrings);
private:
    char* pStringValueList;
};


#endif //FAKELCD_MULTISTRINGFRAME_H
