#ifndef FAKELCD_MULTISTRINGFRAME_H
#define FAKELCD_MULTISTRINGFRAME_H


#include <vector>
#include "BaseFrame.h"

class MultiStringFrame : public BaseFrame {
public:
    MultiStringFrame(int row, int startCol, int width, int quantity);
    void setValue(int whichOne, int millisToShow, char* newValue);
    bool renderInto(char* target, int currentTimeMillis);
private:
    uint8_t howMany;
    char* pValues;
    uint16_t* millisPerFrame;
    uint16_t lastUpdate = 0;
    uint8_t currentOne = 0;
};


#endif //FAKELCD_MULTISTRINGFRAME_H
