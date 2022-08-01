#include <cstdlib>
#include "MultiStringFrame.h"
#include "DisplayManager.h"

MultiStringFrame::MultiStringFrame(int row, int startCol, int width, int quantity)
    : BaseFrame(row, startCol, width),
        howMany((uint8_t)quantity),
        pValues((char*) malloc(width * quantity)),
        millisPerFrame((uint16_t*) malloc(quantity)){
}

void MultiStringFrame::setValue(int whichOne, int millisToShow, const char* newValue) {
    strcopy(newValue, pValues + (whichOne * width), width, true);
    *(millisPerFrame + whichOne) = (uint16_t)millisToShow;
}

bool MultiStringFrame::renderInto(char* target, uint32_t currentTimeMillis) {
    uint16_t timeToShowCurrentOne = *(millisPerFrame + currentOne);
    int timeSinceLastUpdate = (currentTimeMillis & 0xffff) - lastUpdate;

    if ((timeSinceLastUpdate < 0) || (timeSinceLastUpdate > timeToShowCurrentOne)) {
        lastUpdate = currentTimeMillis & 0xFFFF;
        currentOne = (currentOne + 1) % howMany;
        for (int x=0; x<width; x++) {
            *(target + (row * LCD_COLS) + startCol + x) = *(pValues + (currentOne * width) + x);
        }
        return true;
    }
    return false;
}

