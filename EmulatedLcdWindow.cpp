#define VR_LCD_EMU_STATIC 1
#include "EmulatedLcdWindow.h"
#include <SFML/Graphics/Transformable.hpp>

sf::RenderWindow createRenderWindow(const unsigned int cols, const unsigned int rows, const unsigned int scale, char* titlebarText) {
    return sf::RenderWindow{{cols * 6 * scale , rows * 9 * scale }, titlebarText};
}

EmulatedLcdWindow::EmulatedLcdWindow(const uint8_t cols, const uint8_t rows, float scale) :
            rows(rows),
            cols(cols),
            lcd(vrEmuLcdNew(cols, rows, EmuLcdRomA00)),
            pixelSize(scale),
            backlightColor(sf::Color(0xb0c01080)),
            activePixelColor(sf::Color(0x20202080)),
            inactivePixelColor(sf::Color(0xa0b00080)),
            window(createRenderWindow(cols, rows, scale, "HD44780")),
            bitmap(sf::Image()),
            texture(sf::Texture()),
            sprite(sf::Sprite()) {
    // send it commands:
    vrEmuLcdSendCommand(lcd, LCD_CMD_FUNCTION | LCD_CMD_FUNCTION_LCD_2LINE | 0x10);
    vrEmuLcdSendCommand(lcd, LCD_CMD_CLEAR);
    vrEmuLcdSendCommand(lcd, LCD_CMD_HOME);
    vrEmuLcdSendCommand(lcd, LCD_CMD_DISPLAY | LCD_CMD_DISPLAY_ON);

    // then periodically, render it.
    vrEmuLcdUpdatePixels(lcd);   // generates a snapshot of the pixels state

    window.setVerticalSyncEnabled(true);
    bitmap.create(cols * 5 * scale, rows * 8 * scale, sf::Color::Black);
    sprite.setOrigin(0,0);
}

void EmulatedLcdWindow::render() {
    // render the emulated hd44780's content to itself
    vrEmuLcdUpdatePixels(lcd);

    // now render it to the SFML window

    window.clear(backlightColor);

    for (int y=0; y<vrEmuLcdNumPixelsY(lcd); y++) {
        for (int x=0; x<vrEmuLcdNumPixelsX(lcd); x++) {
            char pixel = vrEmuLcdPixelState(lcd, x, y);
            sf::RectangleShape rect(sf::Vector2f(pixelSize,pixelSize));
            rect.setPosition((x * pixelSize), (y * pixelSize));
            if (pixel == 0)
                rect.setFillColor(inactivePixelColor);
            else if (pixel == 1)
                rect.setFillColor(activePixelColor);
            else
                rect.setFillColor(backlightColor);
            window.draw(rect);

        }
    }
    window.display();
}

bool EmulatedLcdWindow::isOpen() {
    return window.isOpen();
}

void EmulatedLcdWindow::handleWindowEvents() {
    for (auto event = sf::Event{}; window.pollEvent(event);) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    render();
}

void EmulatedLcdWindow::sendCommand(uint8_t command) {
    vrEmuLcdSendCommand(lcd, command);
}

void EmulatedLcdWindow::writeByte(uint8_t value) {
    vrEmuLcdWriteByte(lcd, value);
}

void EmulatedLcdWindow::writeString(char* value) {
    vrEmuLcdWriteString(lcd, value);
}

void EmulatedLcdWindow::clear() {
    vrEmuLcdSendCommand(lcd, LCD_CMD_CLEAR);
}

void EmulatedLcdWindow::moveToHome() {
    vrEmuLcdSendCommand(lcd, LCD_CMD_HOME);
}

void EmulatedLcdWindow::moveTo(uint8_t row, uint8_t col) {
    int ddramOffset = vrEmuLcdGetDataOffset(lcd, col, row);
    vrEmuLcdSendCommand(lcd, ((uint8_t)(LCD_CMD_SET_DRAM_ADDR & 0xff) | (uint8_t)(ddramOffset & 0x7f)) );
}


void EmulatedLcdWindow::print(int row, int col, uint8_t value) {
    moveTo(col, row);
    writeByte(value);
}

void EmulatedLcdWindow::print(int row, int col, char* stringValue) {
    moveTo(col, row);
    writeString(stringValue);
}



