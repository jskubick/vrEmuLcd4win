#include <iostream>
#define VR_LCD_EMU_STATIC 1
#include "VrEmuLcd.h"
#include <SFML/Graphics.hpp>

#define LCD_ROWS 4
#define LCD_COLS 20
#define LCD_PIXEL_DIMENSIONS 10

/*
 * CP437 uses codepoints 0xDB=top+bottom, DC=bottom, DF=top
 * so does CP850, 852, 855, 866,
 * UTF8's Block Elements include U+2580 (upper-half ▀), U+2584 (lower-half ▄), and U+2588 (full-block █)
 */

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
    VrEmuLcd *lcd = vrEmuLcdNew(LCD_COLS, LCD_ROWS, EmuLcdRomA00);

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


    sf::Color darkPixel(20,20,20);
    sf::Color clearPixel(223,246,18);
    sf::Color noPixel(225,250,20);

    unsigned int windowWidth = (int)(LCD_COLS * 5 * 12);
    unsigned int windowHeight = (int)(LCD_ROWS * 8 * 12);
    auto window = sf::RenderWindow{ { windowWidth, windowHeight }, "CMake SFML Project" };
    window.setVerticalSyncEnabled(true);
    sf::Image bitmap;
    bitmap.create(windowWidth, windowHeight, sf::Color::Black);
    auto texture = sf::Texture();


    for (int y = 0; y < vrEmuLcdNumPixelsY(lcd); ++y) {
        for (int x = 0; x < vrEmuLcdNumPixelsX(lcd); ++x) {
            // do whatever you like with the pixel information. render it to a texture, output it to  a console, whatever
            // values returned are:  -1 = no pixel (character borders), 0 = pixel off, 1 = pixel on
            char pixel = vrEmuLcdPixelState(lcd, x, y);
            if (pixel == -1)
                setPixel(bitmap, x,y,noPixel);
            else if (pixel == 0)
                setPixel(bitmap, x,y,clearPixel);
            else if (pixel == 1)
                setPixel(bitmap, x,y,darkPixel);
        }
    }

    texture.loadFromImage(bitmap);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setOrigin(0,0);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }


    return 0;
}
