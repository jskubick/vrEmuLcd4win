//
// Created by root on 7/15/2022.
//

#ifndef FAKELCD_STRINGFRAME_H
#define FAKELCD_STRINGFRAME_H


#include "BaseFrame.h"

class StringFrame : public BaseFrame {
public:
    StringFrame (char* pointer, int size);
    bool render(int millis) override;
private:
    int lastChange = 0;
};


#endif //FAKELCD_STRINGFRAME_H
