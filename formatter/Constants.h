//
// Created by root on 7/26/2022.
//

#ifndef FAKELCD_CONSTANTS_H
#define FAKELCD_CONSTANTS_H


#define TWELVE_CHAR 0xED

// 0xD7 is Katakana "Ra", which vaguely resembles "minus sign with top of '2' below it"
#define MINUSTWO_CHAR 0xC3

// if CUSTOM_FIFTHS_FROM is defined, the 5 custom characters from the defined value are used to render
// .0, .2, .4, .6, and .8 respectively
#define CUSTOM_FIFTHS_FROM 0
#define TEN_CHAR 5
#define ELEVEN_CHAR 6

// the following can be used as a custom character:
//  SPACE_MINUS is space + tiny minus sign
//  COLON_MINUS is colon + tiny minus sign
//  EQUALS_MINUS is equals + tiny minus sign
#define SPACE_MINUS 7

#define MINUS_ONE_CHAR 0xE9
// 0xd7 is Katakana "Ra", which vaguely resembles a minus sign with the top portion of a "2" below it
#define MINUS_TWO_CHAR 0xD7
// 0xb1 is Katakana that vaguely resembles a - sign with a twisted & broken '3' below it
#define MINUS_THREE_CHAR 0xb1
// 0xD1 is Katakana that vaguely resembles a very broken '4'
#define MINUS_FOUR_CHAR 0xd1
// 0xab is Katakana that vaguely resembles a minus sign with upside-down lowercase "V" (think Roman Numeral "5")
#define MINUS_FIVE_CHAR 0xab
// 0xf6 is greek Sigma. OK, I'm really grasping at straws now to associate it with a negative digit.
#define MINUS_SIX_CHAR 0xf6
// 0xa7 is Katakana that looks kind of like a minus sign combined with '7'
#define MINUS_SEVEN_CHAR 0xa7





#endif //FAKELCD_CONSTANTS_H
