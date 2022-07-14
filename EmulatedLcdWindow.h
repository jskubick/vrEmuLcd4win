/**
 * This class Encapsulates the emulated HD44780 LCD and its presentation to the user as a SFML window.
 */
#ifndef FAKELCD_EMULATEDLCDWINDOW_H
#define FAKELCD_EMULATEDLCDWINDOW_H

#include "VrEmuLcd.h"
#include <SFML/Graphics.hpp>


class EmulatedLcdWindow {
public:
    /**
     *
     * @param rows number of rows on the character-mode LCD.
     * @param cols number of columns on the character-mode LCD.
     * @param scale number of PC-pixels that should be used to render each LCD-pixel.
     *          For example, if scale=10, each pixel on the character LCD will be rendered as 10 pixels on screen.
     *          10 pixels is a decent size for a 4k display, but you might want to use something like 4 or 5 if you're
     *          running at 1920 x {whatever}.
     */
    EmulatedLcdWindow(uint8_t rows, uint8_t cols, float scale);
    void render();
    bool isOpen();
    void handleWindowEvents();

    void sendCommand(uint8_t);
    void writeByte(uint8_t value);
    void writeString(char* value);

    void clear(); // LCD_CMD_CLEAR
    void moveToHome(); // LCD_CMD_HOME
    void moveTo(uint8_t row, uint8_t col);

    /** prints character at (col, row)
     *  WARNING: absolutely no sanity checking is performed.
     *  row and col are 0-based.
     */
    void print(int row, int col, uint8_t value);

    /** Prints 'stringValue' on row {row}, starting from column {col}.
     * Warning: absolutely no sanity-checking is performed.
     * row and col are 0-based.
     * wrapping follows behavior of HD44780, which probably isn't what you're expecting unless you're aware of
     * how DRAM addresses map to character positions on a HD44780. And just to repeat, there's no sanity-checking.
     */
    void print(int row, int col, char* stringValue);

private:
    uint8_t rows;
    uint8_t cols;
    VrEmuLcd* lcd;
    float pixelSize;
    sf::Color backlightColor;
    sf::Color activePixelColor;
    sf::Color inactivePixelColor;
    sf::RenderWindow window;
    sf::Image bitmap;
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif //FAKELCD_EMULATEDLCDWINDOW_H
