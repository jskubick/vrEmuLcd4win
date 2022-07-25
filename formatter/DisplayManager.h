#ifndef FAKELCD_DISPLAYMANAGER_H
#define FAKELCD_DISPLAYMANAGER_H

#define LCD_ROWS 4
#define LCD_COLS 20

#include "BaseFrame.h"
#include <vector>

class DisplayManager {
public:
    DisplayManager();
    char* charAt(int row, int col);
    char* getRow(int row);
    void addFrame(BaseFrame* frame);
    bool render(int millis);
private:
    char buf[LCD_ROWS][LCD_COLS];
    std::vector<BaseFrame*> frames;
};


#endif //FAKELCD_DISPLAYMANAGER_H
