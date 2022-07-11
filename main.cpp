#include <iostream>
#define VR_LCD_EMU_STATIC 1
#include "VrEmuLcd.h"

/*
 * CP437 uses codepoints 0xDB=top+bottom, DC=bottom, DF=top
 * so does CP850, 852, 855, 866,
 * UTF8's Block Elements include U+2580 (upper-half ▀), U+2584 (lower-half ▄), and U+2588 (full-block █)
 */

int main() {
    VrEmuLcd *lcd = vrEmuLcdNew(20, 4, EmuLcdRomA00);

    // send it commands:
    vrEmuLcdSendCommand(lcd, LCD_CMD_FUNCTION | LCD_CMD_FUNCTION_LCD_2LINE | 0x10);
    vrEmuLcdSendCommand(lcd, LCD_CMD_CLEAR);
    vrEmuLcdSendCommand(lcd, LCD_CMD_HOME);
    vrEmuLcdSendCommand(lcd, LCD_CMD_DISPLAY | LCD_CMD_DISPLAY_ON);

    // send it data
    vrEmuLcdWriteByte(lcd, 'H');
    vrEmuLcdWriteByte(lcd, 'e');
    vrEmuLcdWriteByte(lcd, 'l');
    vrEmuLcdWriteByte(lcd, 'l');
    vrEmuLcdWriteByte(lcd, 'o');

    // or cheat
    vrEmuLcdWriteString(lcd, " world!");

    // then periodically, render it.
    vrEmuLcdUpdatePixels(lcd);   // generates a snapshot of the pixels state

    for (int y = 0; y < vrEmuLcdNumPixelsY(lcd); ++y) {
        for (int x = 0; x < vrEmuLcdNumPixelsX(lcd); ++x) {
            // do whatever you like with the pixel information. render it to a texture, output it to  a console, whatever
            // values returned are:  -1 = no pixel (character borders), 0 = pixel off, 1 = pixel on
            char pixel = vrEmuLcdPixelState(lcd, x, y);
            if (pixel == -1)
                std::cout<<" ";
            else if (pixel == 0)
                std::cout << " ";
            else if (pixel == 1)
                std::cout << "*";
        }
        std::cout << '\n';
    }

    std::cout << "done";
    return 0;
}
