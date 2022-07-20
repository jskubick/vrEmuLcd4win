//
// Created by root on 7/20/2022.
//

#ifndef FAKELCD_SMASHEDDECIMAL_H
#define FAKELCD_SMASHEDDECIMAL_H

#include <cstdint>

class SmashedDecimal {

    const uint8_t decimals[2][8] {
        {
            0b01110,
            0b01010,
            0b01010,
            0b01010,
            0b01110,
            0b00000,
            0b11000,
            0b11000
        },
        {
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b00000,
            0b11000,
            0b11000
        }
    };

};


#endif //FAKELCD_SMASHEDDECIMAL_H
