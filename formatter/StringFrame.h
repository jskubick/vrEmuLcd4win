#ifndef FAKELCD_STRINGFRAME_H
#define FAKELCD_STRINGFRAME_H

#include <cstdint>
#include "BaseFrame.h"
#include "DisplayManager.h"

class StringFrame : public BaseFrame {
public:
    StringFrame(int row, int startCol, int width, const char* content);
    bool renderInto(char* target, int currentTime);
    void update(char* newContent);
    //static StringFrame* create(DisplayManager* pCharacterDisplay, int row, int col, int len, char* pContentString);
private:
    char* stringContent;
};


#endif //FAKELCD_STRINGFRAME_H
