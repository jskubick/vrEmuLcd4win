//
// Created by root on 7/28/2022.
//

#ifndef FAKELCD_NUMBERFRAME_H
#define FAKELCD_NUMBERFRAME_H


#include "BaseFrame.h"
#include "BaseNumberRenderer.h"

class NumberFrame : public BaseFrame {
public:
    NumberFrame(int row, int startCol, int width, BaseNumberRenderer* pRenderer);
    void set(float newValue);
    bool renderInto(char* buf, int currentTimeMillis);
private:
    BaseNumberRenderer* pRenderer;
};


#endif //FAKELCD_NUMBERFRAME_H
