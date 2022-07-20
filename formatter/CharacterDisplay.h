//
// Created by root on 7/14/2022.
//

#ifndef FAKELCD_CHARACTERDISPLAY_H
#define FAKELCD_CHARACTERDISPLAY_H


#include "BaseFrame.h"
#include <vector>

class CharacterDisplay {
public:
    CharacterDisplay(int rows, int cols);
    char* getRegion(int row, int col);
    char* getRow(int row);
    void addFrame(BaseFrame* frame);
    bool render(int millis);
private:
    int rows;
    int cols;
    char* buf;
    std::vector<BaseFrame*> frames;
};


#endif //FAKELCD_CHARACTERDISPLAY_H
