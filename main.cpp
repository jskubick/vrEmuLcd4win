#define VR_LCD_EMU_STATIC 1
#include "EmulatedLcdWindow.h"
#include <SFML/Graphics.hpp>
#include <Windows.h>

#define LCD_ROWS 4
#define LCD_COLS 20
#define LCD_PIXEL_DIMENSIONS 10

void setPixel(sf::Image& bitmap, int coordX, int coordY, sf::Color color) {
    int startY = coordY * LCD_PIXEL_DIMENSIONS;
    int startX = coordX * LCD_PIXEL_DIMENSIONS;
    for (int y = startY; y < (startY + LCD_PIXEL_DIMENSIONS); y++) {
        for (int x = startX; x < (startX + LCD_PIXEL_DIMENSIONS); x++) {
            bitmap.setPixel(x, y, color);
        }
    }
}

int main() {
    EmulatedLcdWindow window = EmulatedLcdWindow(LCD_COLS, LCD_ROWS, 10);
    int counter = 0x7f;
    while (window.isOpen()) {
        counter++;
        if (counter > 0x7f)
            counter = 0x30;
        window.printChar(0,0,counter);
        window.handleWindowEvents();
        Sleep(200);
    }
}
