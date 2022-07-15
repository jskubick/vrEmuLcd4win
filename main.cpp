#define VR_LCD_EMU_STATIC 1
#include "EmulatedLcdWindow.h"
#include <SFML/Graphics.hpp>
#include <Windows.h>

#define LCD_ROWS 4
#define LCD_COLS 20
#define LCD_PIXEL_DIMENSIONS 3.0

#include "formatter/CharacterDisplay.h"
#include "formatter/BaseFrame.h"
#include "formatter/StringFrame.h"

/*
 * @ToDo: fix time from seconds to millis
 * @ToDo: finish implementing MultiStringFrame
 */

int main() {
    CharacterDisplay framebuf(LCD_ROWS, LCD_COLS);
    BaseFrame* frame[10];
    for (int x=0; x<10; x++)
        frame[x] = nullptr;

    frame[0] = new BaseFrame(framebuf.getRegion(2,0), 10);
    frame[0]->update("line three");
    frame[1] = new BaseFrame(framebuf.getRegion(2,17), 3);
    frame[1]->update("666");

    StringFrame *mid = new StringFrame(framebuf.getRegion(2, 12), 3);
    framebuf.addFrame(mid);
    mid->update("foo");




    EmulatedLcdWindow window = EmulatedLcdWindow(LCD_COLS, LCD_ROWS, LCD_PIXEL_DIMENSIONS);
    window.print(3,0, "Text on row 4 of LCD");
    uint8_t counter = 0;
    while (window.isOpen()) {
        window.print(0, counter % LCD_COLS, counter & 0x7f);
        window.print(1, counter % LCD_COLS, 128 + (counter & 0x7f));
        ++counter &=0x7f;

        framebuf.render(time(nullptr));

        char* row = framebuf.getRow(2);
        for (int x=0; x<20; x++) {
            char c = row[x];
            window.print(2,x, row[x]);
        }




        window.handleWindowEvents();
        Sleep(200);
    }
}
