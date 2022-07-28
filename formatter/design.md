Number Scenarios:

1. Custom character pool:
   1. {.0 .2 .4 .6 .8}
   2. 10
   3. 11
   4. -1
   5. (space)-
2. 1 custom character allocated to frame
3. 2 custom characcters allocated to frame
4. 1 or 2 custom characters available to frame from pool if available, with attempts made to reuse existing characters.
5. No custom characters used.

can use 0xE9 for "-1"

CustomCharacterPool:
* even decimals = .0 .2 .4 .6 .8
* half-char minus-signs = (' '-), (:-), (,-)
* (-.)
* -1


                     "-1" custom char  no custom char    (space)- :- ,- =-
2-char int, range=   -19 to 99,        -9 to 99          -99 to 99
3-char int, range=   -199 to 999,      -99 to 99         -999 to 999
4-char int, range=   -1999 to 9999,    -999 to 9999      -9999 to 9999
5-char int, range=   -19999 to 99999,  -9999 tot 99999   -99999 to 99999



can have:
   prefixPositiveCharacter (defaults to none, likely to be space, colon, or +)
   prefixNegativeCharacter (defaults to none, restricts negative range to 1/10 positive range)
   tenChar: shared "10" character
   elevenChar: shared "11" character
   minusOneChar: shared "-1" character
   fifthsChars: shared characters for .0 .2 .4 .6 .8
   tooNegString: shown to indicate error if value is too negative to render
   tooBigString: shown to indicate error if value is too big to render
   ownChar: custom character exclusively owned by this frame that can be redefined at will

   formatting:
      000 = right-align
      001 = decimal-align if possible (1 decimals)
      010 = decimal-align if possible (2 decimals)
      011 = decimal-align if possible (3 decimals)
      100 = left-align
      101 = decimal-align (1 digit)
      110 = decimal-align, with hundredths (2 digits)
      111 = decimal-align, with thousandths (3 digits)


if user allocates custom characters for -1, fifths, 10, and 11, range is:
   2charInt, no prefix: -19 to 119
   2charInt, prefixes: -9 to 9 or 99
   3charInt, no prefix: -199 to 1199
   3charInt, prefixes: -99 to 99 or 999
   4charInt, no prefix: -1999 to 11999



5 characters .0 .2 .4 .6 .8
    -9 to 99    -9 to -1
                -.8 to 9.8
                10 to 99

    -99 to 999  -99 to -10
                -9.8 to 99.8
                100 to 999

    -999 to 9999    -9999 to -100
                    -99.8 to 999.8
                    1000 to 9999

if -1 defined:, or using "-1"(ish) char:
    -19 to 99   -19 to -2
                -1.8 to 9.8
                10 to 99

if ]- defined into prefix:
    -99 to 99   -99 to -10
                -9.8 to 9.8
                10 to 99



The highest temperature ever recorded anywhere on Earth is 135F
... but anything higher than 129F is freakish

-19F is within the norm for 99.8% of actual cities in the US & Canada 

-49 is enough range to cover "99.9%" of low temperatures (-40 is the same in both celsius and fahrenheit)

Minimum temperatures:
    Longyearbyen, Svalbard: -16C
    Norilsk, Russia         -31C
    McMurdo                 -29C, -19F
    Murmansk, Russia        -13C
    South Pole              -64C, -83F

    Fairbanks, Alaska
    Yellowknife, Canada -44C (record -52C)
    Fargo, ND   -48F coldest, ever

    Edmonton's cold is typically -15C
    Quebec City's cold is typically -19C (record -38C)
    Winnipeg's cold is typically -19C
        12+ days per year when temperature falls below -30C

    The temperature in Fargo, ND has fallen below -29F at least once in 45 of the past 100 years
    The temperature in Fargo, ND has fallen below -19F at least once per year for 98 of the past 100 years

    Fargo, ND:  temperature has fallen below -29F at least once roughly 50% of past century
                temperature has fallen below -19F at least once for all but one or two of the past hundred years.

    with 3 digits, we can go to -99
    with 3 digits, leading space, and fifths:



To represent full range of likely outdoor temperatures in Fargo, ND or Winnipeg,
you need:
    3 digits for integer
    4 char (3 digits + lead) for decimalFifth

To do it with 2 characters, you need
    * a dedicated custom character that can represent -{digit}, to give you range down to -99
    or
    ... if you're willing to be awkward and use letters, you can represent temperatures
    from -1 to -36 using -1 to -Z, and extend range to -61 by adding lowercase



2charNumber:
    -a to -z: -36 to -62
    -A to -Z: -10 to -35
    -1 to -9: -1 to -9
    00 to 99: 0 to 99

3charNumber, with PREFIX_NEG and PREFIX defined (acts kind of like 2-char)
    -99 to 99

3charNumber (3 characters, all 3 available for use as digits)
    -99 to 999
    -9.8 to 99.8 (if decimalFifths enabled)

3charNumber + decimalFifths + TEN_CHAR
    -109 to 1099
    -10.8 to 109.8

3charNumber + decimalFifths + TEN_CHAR + ELEVEN_CHAR
    -119 to 1199
    -11.8 to 119.8

3charNumber + decimalFifths + TEN_CHAR + ELEVEN_CHAR + TWELVE_CHAR
    -129 to 1299
    -12.8 to 129.8

---
3charNumber + decimalFifths + ownChar
    -199 to 1999
    -19.8 to 199.8
---

3charTempHum:
    alternates between temperature and humidity, or temp/dewpoint/humidity
        temp & dewpoint: -99 to -10, -9.8 to 99.8, 100 to 999
        humidity: 00% to 99% or HUNDREDPCT_CHAR0 and HUNDREDPCT_CHAR1

4charNumber w/PREFIX and PREFIX_NEG defined + decimalFifths
    -999 to 999
    -99.8 to 99.8

4charNumber (5 decimalFifths + PREFIX_NEG + TEN_CHAR + ELEVEN_CHAR)
    -1199 to 1199
    -11.8 to 119.8


Scenario 1: shared custom characters:
    0 = .0
    1 = .2
    2 = .4
    3 = .6
    4 = .8
    5 = 10
    6 = 11 or >- or {other}
    7 = 12 or ]- or {other}

Scenario 2: 
    0 = 10
    1 = 11
    2 = 12
    3..7 = 5 available

2 uint8_t values define suffix
    [0] and [1] are both between 9..15: 8 bits that can be used for something else (3/ea + 2 configbits)
    [0] between 9..15, [1] has normal value: at least 8 possibilities x format bits 
    [0] has normal value, [1] between 9..15: at least 8 possibilities x format bits
    [0] and [1] have normal values: behavior defined by format bits
        00  [0] defines suffix for 0, [1] defines suffix for 1
        01  [0] defines suffix for 0, [1] defines suffix for 1 and 2
        10  [0] defines suffix for 0 and 1
        11  [0] defines suffix for 0 and 1, [1] defines suffix for remainder


CustomCharacterManager
    we never refer to characters 0 through 7
        the HD44780 considers them to be one and the same with 8 through F
        C uses 00 as string-terminator and null-pointer
        so... if we ever see a reference to characters 0 through 7 in the display array, it's a flag.


    if enabled, decimalFifths (.0 .2 .4 .6 .8) use characters 8 through C
    predefined character definitions:
        PREFIX_NEG_SPACE
        PREFIX_NEG_COLON
        PREFIX_NEG_EQUALSIGN
        PAIR_10
        PAIR_11
        PAIR_12
        SUPERSCRIPT_F
        SUPERSCRIPT_C

For indoor temp & humidity:
    we don't have to worry about negative temperatures. Really.
    on the freak chance humidity exceeds 99%, we'll call it 


Configuration:
    isDecimalFifth enabled
        * if it is, values are implicitly right-aligned, with decimalFifth in last column (using ".0" if appropriate) 
    
    To round to DecimalFifth:
        multiply by 10 and round
            if animating uncertainty and last digit is odd, alternate between rounding up and down
            if NOT animating uncertainty, round last digit away from zero

    To render as integer:
        round to nearest tenth.
            if animating uncertainty

        rounding rule:
            value is rounded to nearest tenth:
                intValue = round(floatValue * 10.0)
                if intValue is odd, we do one of two things:
                    if animated, we alternate between rounding up and down
                    otherwise, we 
    animateUncertainty
        * decimal values fall into one of three ranges:
            * at most 0.08 more than nearest-lower even tenth (ex: 72.07 vs 72.0). Rounds down to nearest even tenth.
            * at most 0.08 less than nearest-higher even tenth (ex: 72.12 vs 72.2). Rounds up to neareat even tenth.
            * nearest tenth is odd, and value is within 0.02 of it. Ex: 72.08 or 72.12 to 72.0/72.2
        otherwise, we round away from zero and break ties towards greater magnitude



// round away from zero, then multiply by 100
int value = (int)((floatValue + ((floatValue >= 0) ? 0.005 : -0.005)))
// get just the ones and tens digits
value %= 100;
// get the modulus of 20
value %= 20;

if (value <= 7)) // round down
else if (value >= 13)) // round up
else (alternate between rounding down and up)

0, no rounding necessary
1..7, round down
8..12, alternate
13..19, round up


4 rendering and rounding scenarios:
    decimalFifth, animated:
        intValue = std::round(decimalValue * 10); // 70.11 -> 701
        if (isOdd(intValue % 10))
            alternate between rendering as nearest-fifth down and nearest-fifth up

    decimalFifth, static:
        7190 to 7199
        7200 to 7209
        7210 to 7219
        7220

    71.90 to 71.99  rounds to 72.0
    72.00 to 72.09  rounds to 72.0
    72.10 to 72.19 rounds to 72.2
    72.20 to 72.29 rounds to 72.2

    integer, animated:

    integer, static:



rounding methods:
    symmetric round half-up (ie, round to nearest neighbor away from zero)
    round half-even (banker's rounding)
        -3.5 and -4.5 both round to -4. 3.5 and 4.5 round to 4.
    round half-alternate:
        alternate between rounding half up and half down
    round half-random:
        randomly choose whether to round half up or down
    
    round half-up: what I've personally (and unknowingly) done since middle school...
        add +5, then truncate the result.
        works fine for positive values, "deviates" for negative values

    4-bit 2s complent of -3 
    -1 is 1111
    -2 is 1110
    -3 is 1101
        so... -3.75 would be 1101.1100
        add 0000.1000 to get
            1110.0100, or -2

7.10
7.20: 7.2
7.29: 7.2
7.30: 7.4
7.37: 7.4

6.98 to 7.02 is unquestionably 7.0
7.08 to 7.12 alternates between 7.0 and 7.2
7.18 to 7.22 is unquestionably 7.2
7.28 to 7.32 alternates between 7.2 and .74

7.0799999 to 707.9999  truncates to 707, rounds to 708
7.124 to 712.4 truncates to 712, rounds to 712
7.125 to 712.5 truncates to 712, rounds to 713
7.1259999 to 712.5999 truncates to 712, rounds to 713


int intVal = (int)round(floatVal * 10)


Possibilities for FloatFrame<3>:
    * 5 chars defined for .0 .2 .4 .6 .8
    * 4 chars defined for .2 .4 .6 .8
    * prefixes defined (one or both potentially from the pool of custom characters)
    * 1 char defined for 10
    * 1 char defined for 11
    * 1 char defined for 12
    * the Frame has a custom character at its disposal


