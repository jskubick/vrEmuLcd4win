//
// Created by root on 7/11/2022.
//

#ifndef FAKELCD_EMULATEDLCDWINDOW_H
#define FAKELCD_EMULATEDLCDWINDOW_H

#include "VrEmuLcd.h"
#include <SFML/Graphics.hpp>


class EmulatedLcdWindow {
public:
    EmulatedLcdWindow(uint8_t rows, uint8_t cols, int scale);
    void render();
    bool isOpen();
    void handleWindowEvents();
    void printChar(uint8_t row, uint8_t col, uint8_t value);
private:
    uint8_t rows;
    uint8_t cols;
    VrEmuLcd* lcd;
    int pixelSize;
    sf::Color backlightColor;
    sf::Color activePixelColor;
    sf::Color inactivePixelColor;
    sf::RenderWindow window;
    sf::Image bitmap;
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif //FAKELCD_EMULATEDLCDWINDOW_H
