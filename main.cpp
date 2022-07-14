#define VR_LCD_EMU_STATIC 1
#include "EmulatedLcdWindow.h"
#include <SFML/Graphics.hpp>
#include <Windows.h>

#define LCD_ROWS 4
#define LCD_COLS 20
#define LCD_PIXEL_DIMENSIONS 4.0

int main() {
    EmulatedLcdWindow window = EmulatedLcdWindow(LCD_COLS, LCD_ROWS, LCD_PIXEL_DIMENSIONS);
    window.print(3,0, "Text on row 4 of LCD");
    uint8_t counter = 0;
    while (window.isOpen()) {
        window.print(0, counter % LCD_COLS, counter & 0x7f);
        window.print(1, counter % LCD_COLS, 128 + (counter & 0x7f));
        ++counter &=0x7f;
        window.handleWindowEvents();
        Sleep(200);
    }
}
