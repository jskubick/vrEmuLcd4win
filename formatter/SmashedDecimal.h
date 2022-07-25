//
// Created by root on 7/20/2022.
//

#ifndef FAKELCD_SMASHEDDECIMAL_H
#define FAKELCD_SMASHEDDECIMAL_H

#include <cstdint>
#define VR_LCD_EMU_STATIC 1
#include "../EmulatedLcdWindow.h"

class SmashedDecimal {

public:
    SmashedDecimal(EmulatedLcdWindow* lcd, int characterNumber, bool isUltraNarrow, int initialTenthsValue);

    static constexpr uint8_t hundred[8] {
        0b00000,
        0b00010,
        0b10101,
        0b10101,
        0b10101,
        0b10010,
        0b10000,
        0b00000
    };

    static constexpr uint8_t hundredTeen[8] {
        0b00000,
        0b00001,
        0b01011,
        0b11001,
        0b01001,
        0b01001,
        0b01000,
        0b00000

    };

    static constexpr uint8_t err[8] {
        0b01000,
        0b10100,
        0b00100,
        0b01001,
        0b00001,
        0b01001,
        0b00000,
        0b00001
    };

    static constexpr uint8_t charData[10][8] {
        {
            0b00111,
            0b00101,
            0b00101,
            0b00101,
            0b00111,
            0b00000,
            0b11000,
            0b11000
        },
        {
            0b00010,
            0b00010,
            0b00010,
            0b00010,
            0b00010,
            0b00000,
            0b11000,
            0b11000
        },
        {
          0b00111,
          0b00001,
          0b00111,
          0b00100,
          0b00111,
          0b00000,
          0b11000,
          0b11000
        },
        {
            0b00111,
            0b00001,
            0b00111,
            0b00001,
            0b00111,
            0b00000,
            0b11000,
            0b11000
        },
        {
            0b00101,
            0b00101,
            0b00111,
            0b00001,
            0b00001,
            0b00000,
            0b11000,
            0b11000
        },
        {
            0b00111,
            0b00100,
            0b00111,
            0b00001,
            0b00110,
            0b00000,
            0b11000,
            0b11000
        },
        {
            0b00010,
            0b00100,
            0b00110,
            0b00101,
            0b00010,
            0b00000,
            0b11000,
            0b11000
        },
        {
            0b01111,
            0b00001,
            0b00010,
            0b00100,
            0b00100,
            0b00000,
            0b11000,
            0b11000
        },
        {
            0b00111,
            0b00101,
            0b00111,
            0b00101,
            0b00111,
            0b00000,
            0b11000,
            0b11000
        },
        {
            0b00111,
            0b00101,
            0b00111,
            0b00001,
            0b00001,
            0b00000,
            0b11000,
            0b11000
        }
    };

    static constexpr uint8_t denseData[10][8] {
            {
                    0b11100,
                    0b10100,
                    0b10100,
                    0b10100,
                    0b11100,
                    0b00000,
                    0b00000,
                    0b00000
            },
            {
                    0b01000,
                    0b01000,
                    0b01000,
                    0b01000,
                    0b01000,
                    0b00000,
                    0b00000,
                    0b00000
            },
            {
                    0b11100,
                    0b00100,
                    0b11100,
                    0b10000,
                    0b11100,
                    0b00000,
                    0b00000,
                    0b00000
            },
            {
                    0b11100,
                    0b00100,
                    0b11100,
                    0b00100,
                    0b11100,
                    0b00000,
                    0b00000,
                    0b00000
            },
            {
                    0b10100,
                    0b10100,
                    0b11100,
                    0b00100,
                    0b00100,
                    0b00000,
                    0b00000,
                    0b00000
            },
            {
                    0b11100,
                    0b10000,
                    0b11100,
                    0b00100,
                    0b11100,
                    0b00000,
                    0b11000,
                    0b11000
            },
            {
                    0b00100,
                    0b01000,
                    0b01100,
                    0b01010,
                    0b00100,
                    0b00000,
                    0b11000,
                    0b11000
            },
            {
                    0b01110,
                    0b00010,
                    0b00100,
                    0b01000,
                    0b01000,
                    0b00000,
                    0b11000,
                    0b11000
            },
            {
                    0b01110,
                    0b01010,
                    0b01110,
                    0b01010,
                    0b01110,
                    0b00000,
                    0b11000,
                    0b11000
            },
            {
                    0b01110,
                    0b01010,
                    0b01110,
                    0b00010,
                    0b00010,
                    0b00000,
                    0b11000,
                    0b11000
            }
    };

    void setValue(int tenths);

   void setTenths(float wholeValue);

    static uint8_t* getCharData(int digit, bool isUltraNarrow);

    void animate(int millis);

private:
    EmulatedLcdWindow* lcd;
    bool isUltraNarrow;
    int characterNumber;
    int representedValue;
    bool isFlipped = false;

};


#endif //FAKELCD_SMASHEDDECIMAL_H
