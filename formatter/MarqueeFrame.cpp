//
// Created by root on 7/25/2022.
//

#include <cstdlib>
#include "MarqueeFrame.h"
#include "DisplayManager.h"

MarqueeFrame::MarqueeFrame(int row, int startCol, int displayWidth, int totalChars, const char *initialContent,
                           int decaMillisPerChar, int decaMillisPerWhitespace, int decaMillisPerEnd)
: BaseFrame(row, startCol, displayWidth),
    content((char*)malloc(totalChars)),
    totalChars(totalChars),
    maxChars(totalChars),
    decaMillisPerChar((uint8_t)decaMillisPerChar),
    decaMillisPerWhitespace((uint8_t)decaMillisPerWhitespace),
    decaMillisPerEnd((uint8_t)decaMillisPerEnd),
    currentFirstChar(-displayWidth){
    this->totalChars = strcopy(initialContent, content, totalChars, false);
}

/*
 * @ToDo: handle various scenarios:
 *      content is shorter than marquee window
 *      distance to first space is longer than marquee window
 */
bool MarqueeFrame::renderInto(char* target, int currentTimeMillis) {

    int totalTimeSinceCurrentFirstCharBecameFirst = (currentTimeMillis & 0xFFFF) - timeWhenCurrentCharBecameFirst;

    // @ToDo: doesn't handle most scenarios correctly
    char nextOffscreenChar = *(content + currentFirstChar + width -1);
    if (    (currentFirstChar + width -1 == totalChars) &&
            (totalTimeSinceCurrentFirstCharBecameFirst > 0) &&
            (totalTimeSinceCurrentFirstCharBecameFirst < (10 * decaMillisPerEnd))  )
        return false;

    if (    (nextOffscreenChar == 0x20) &&
            (totalTimeSinceCurrentFirstCharBecameFirst > 0) &&
            (totalTimeSinceCurrentFirstCharBecameFirst < (10 * decaMillisPerWhitespace))
            )
        return false;

    if (    (totalTimeSinceCurrentFirstCharBecameFirst > 0) &&
            (totalTimeSinceCurrentFirstCharBecameFirst < (10 * decaMillisPerChar))  )
        return false;

    timeWhenCurrentCharBecameFirst = currentTimeMillis & 0xFFFF;


    for (int x = 0; x < width; x++) {
        if ((currentFirstChar + x) < 0)
            *(target+(row * LCD_COLS) + startCol + x) = 0x20;
        else if (currentFirstChar + x < totalChars)
            *(target+(row * LCD_COLS) + startCol + x) = *(content + currentFirstChar + x);
        else
            *(target+(row * LCD_COLS) + startCol + x) = 0x20;
    }
    currentFirstChar += 1;
    if (currentFirstChar == (totalChars + width))
        currentFirstChar = -width;
    return true;
}