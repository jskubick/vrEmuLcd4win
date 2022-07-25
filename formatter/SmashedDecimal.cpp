//
// Created by root on 7/20/2022.
//

#include "SmashedDecimal.h"

SmashedDecimal::SmashedDecimal(EmulatedLcdWindow* lcd, int characterNumber, bool isUltraNarrow, int initialTenthsValue) : lcd(lcd), isUltraNarrow(isUltraNarrow), characterNumber(characterNumber) {
    setValue(initialTenthsValue);
}

uint8_t* SmashedDecimal::getCharData(int digit, bool isUltraNarrow) {
    if (digit > 119)
        return const_cast<uint8_t *>(err);

    if (digit > 109)
        return const_cast<uint8_t *>(hundredTeen);

    if (digit > 99)
        return const_cast<uint8_t *>(hundred);

    if (isUltraNarrow)
        return const_cast<uint8_t *>(denseData[digit]);
    else
        return const_cast<uint8_t *>(charData[digit]);
}

void SmashedDecimal::setValue(int tenths) {
    representedValue = tenths;
    for (int x=0; x<8; x++) {
        lcd->createChar(characterNumber, getCharData(tenths, isUltraNarrow));
    }
}

void SmashedDecimal::animate(int millis) {
    if (representedValue < 45)
        return;
    if (representedValue > 55)
        return;

}

void SmashedDecimal::setTenths(float wholeValue) {
    int multipliedValue = (int)wholeValue * 100.0;
    // if pi = 3.1415927, multipliedPi would be 314
    int tenths = multipliedValue / 100; // if multipliedPi, 314 / 100 = 3
    int hundredths = multipliedValue % 10; // if multipliedPi, 314%10 = 4

    /* start with 71.397228
     render float to String, pad with '00'
     find decimal, look for first digit after decimal (3)
     find next digit after decimal. if it's 5, we're going to jiggle it.
     */


}
