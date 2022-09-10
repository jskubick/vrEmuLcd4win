1+15: counting up, holds low 15 bits of time in seconds
        renders as "mm:ss" (colon steady)

010 + 13: counting up, holds low 13 bits of time in minutes (need 6000, have 8192)
        renders as "hh:mm" (colon 1hz)

011ccc+10:  counting up
            low 10 bits hold time count in "approximate hours"
                every 1024 hours (42.66 days), it rolls over, and ccc gets incremented
                    Even if it DIDN'T roll over every 1024 hours, the 32-bit unsigned int with millis
                    would roll over every ~49.66 days (~1,191 hours), and I'd have to deal with
                    at least double the rollover complexity just to gain 56 days that will probably
                    never actually be used anyway. A 341-day countdown is an INSANELY long time by
                    embedded-device standards. In a state like Florida, an Arduino powered by a wall
                    wart without backup power would be almost GUARANTEED to have at least one
                    brownout reset LONG before it got anywhere CLOSE to that time.
            thus, this counter can count a maximum of 8 * 1024 hours = 8192 = 341.33 days
            renders as:
                9d23h   59:59
                99day   23:59 (1hz colon)
                341d    23:59 (1hz colon)

001 + 13 (13 < 6000): counting down from at most 99m99s, 1 second at a time
001 + 13 (13 >= 6000): counting down from at most ~36h30m, 1 minute at a time

000 + 13: (13 < 6000): static value that renders to 00:00 through 99:59
000 + 13: (13 >= 6000, but less than 0xFFFE): static value that renders as 35:59 with 1hz colon

011 + 13: counting up, holds low 13 bits of time in hours 
    ~341 days = 8182 hours
    49 days = 

after hitting 100 minutes, I'll switch to hh:mm
100 hours needs 8192
42 days need 1008 hours
if I use 3 bits + 10 bits per 42 days, I can count a maximum of ~336 days
if I stretch it out to one chunk per 49.66 days, I can count a maximum of ~397.28 days






Cycle timer is 5 characters wide
    official range: 100 minutes (99:59)
    If it rolls over, the leading digit becomes A, and it continues to increment until reaching Z.
    
    36 * 10 * 60 = 21600 seconds = 6 hours
    100 minutes = 6000 seconds
    13 bits = 8192

    bits 15 & 14:
        00  not counting up OR counting down
        01  undefined
        10  counting down towards value
        11  counting up towards value

    bit 13: undefined

    bits 12..0:
        when 15/14 are 00: 
            0x1FFF: undefined
            0x1FFE: overflow
            0..5999 = specific value, 00:00 to 99:59

        when 15/14 are 10:
            target I'm counting down towards

        when 15/14 are 11:
            goal I'm counting up towards

   If timer has 15 bits, max time before overflow is 32768s =  546'8" (or, 9h06:08)
    z9:59 is 36*10*60 = 21600

The cycle timer needs to show seconds & minutes... but really, won't exceed 2 hours.
    starts out as 00:00 to 59:59
    with decimalFifth or customTenth, switches to 9:mm$ (1hz colon) for next 1-10 hours

if cycle timer has "1H" character, can use 6 characters & go up to 1:59:59
    H59:59

if cycle timer has 7 characters
    0:mm:ss

timer5: 5 characters, renders as follows:
    <1h:    59:59 (steady colon)
    <24h:   23:59 (1hz colon)
    <1000d:   +999d  23:59 (2hz colon)
    <10000d:  9999d  23:59 (2hz colon)


timer65 (requires decimalFifths with zero at 0..4
    uses .0, .2, and .4 to represent :00-:19, :20-:39, and :40-:59
    indicated as $ below:
    <1d:    23:59$ (1hz colon)
    <17y:   "+1 day", "9 days", "99days", "+9999d" then 23:59$ (2hz colon)

timer6A (requires dedicated custom character)
    uses custom character to render tenths of a minute (ie, .3=30 to 39s, .5=50s to 59s)
    otherwise, same as timer65

a ~2 hour timer needs 13 bits, so I could actually use 3 of them for other purposes.
* use 1 to indicate rollover


z9:59 is 36 x 10 x 60 seconds = 360 minutes or 21,600 seconds




8 bits: 4:16
9 bits: 8:32
10 bits: 17:04
11 bits: 34:08
12 bits: 68:16
13 bits: 136:32 = 2:16:32
14 bits: 273:04 = 4:33:04
15 bits: 546:08 = 9:06:08
16 bits: 1092:16 = 18:12:16
17 bits: 36 hours
18 bits: 72 hours
19 bits: 6 days
20 bits: 12 days
21 bits: 24 days
22 bits: 48 days
23 bits: 96 days
24 bits: 192 days
25 bits: 384 days
26 bits: 2 years
27 bits: 4 years
28 bits: 8 years
29 bits: 17 years
30 bits: 34 years
31 bits: ~68 years
32 bits: ~136 years
    
    0h to 1h:   00:00 to 59:59 (steady colon)
    1h to 24H   00:00 to 23:59 (1hz colon indicates that it's hh:mm and not mm:ss)
                +01h       59:59   (2hz colon indicates that time > 1h)
    1d to 10d:  9d 24h    59:59     (2hz colon indicates that time > 1h)
    10d=99d:    99+24    
    9d23h  59:59
        1-10d
        2hz colon
    99+24   59:59
        11-100 days
        2hz colon
    9999d  23:59
        assumed to be abnormal overflow condition, but good for 27 years
        2hz colon

timer6: 6 characters, needs .0/.2/.4 or ownChar, renders as follows:
    _59:59 (1-bit option to disable and go straight to 23:59$)
        0s to 1h
        colon steady
    23:59$
        1h to 24h
        $ is .0 (0-19s), .2 (20-39s), or .4 (40-59s)
        colon blinks at 1hz
    9 days  23:59$
    99 day  23:59$
    999day  23:59$
    99999d  23:59$
        same as 23:59$, but colon blinks at 2hz
        could conceivably cover ~273 years

timer8: 8 characters
    23:59:59 or 23h59m59s
        0s to 1day
        colons steady
    99+23:59:59 or 99+23h59:59
    99d23:59:59 or 99d23h59:59
        1-100 days
        1hz colon
    999 days 23:59:59
    9999 day 23:59:59
        up to 27 years
        colon between hours and minutes alternates between : and dot
    config-bits:
    0:  23:59:59
    1:  23h59m59s

    0: displays values between 1 and 100 days as 09+23:59
    1: displays values between 1 and 9999 days as if they were 999+ days

    23:59:59
    23h59:59

    99d24:59 (1hz colon)
    99+24:59 (1hz colon)

    00  23:59:59    99d23:59
    01  23:59:59    99+23:59
    10  23h59m59s   99d23:59
    11  23h59m59s   99+23:59

x0: times less than 1 day render as 23:59:59
x1: times less than 1 day render as 23h59:59

00: 1-10 days renders as 01d23:59 (1hz colon)
01: 1-10 days renders as 01+23:59 (1hz colon)

10: 1-10 days renders as 10 days  then  23:59:59
11: 1-10 days renders as 10 days  then  23h59:59




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



2/2
[-19 ,-9] to [99, 109, 119]

3/2
(steals leading whitespace for minus when necessary)
-[-199,-99] to [99,109, 119]

3/3
-99 to 999
-9.8 to 99.8, 109.8, or 119.8

4/3
-999 to 999
-99.8 to 99.8, 109.8, or 119.8

config bits:

000 no prefix. Character width is same as number of digits. Negative values have 1/10 the range of positive values.
001 undefined
010 prefix is space, negative prefix is (
011 positive prefix is '+', negative prefix is '-'
100 positive prefix is '+', negative prefix is '-'
101 positive prefix is space, negative prefix is furnished by CustomCharManager
110 positive prefix is colon, negative prefix is furnished by CustomCharManager
111 positive and negative prefix furnished by CustomCharManager

no decimal, right-aligned
no decimal, right-aligned and zero-prefixed
no decimal, left-aligned
if room, tenths (if customChar != 0) or fifths... right-aligned
right-aligned .1 if room, left-aligned otherwise
right-aligned .01 if room, left-aligned otherwise
right-aligned .001 if room, left-aligned otherwise
    ex, with 5 characters:
        9.001
        10.01
        100.1
        1001 
        99999
        -.001
        -1.01
        -10.1
        -100
        -9999



a TempHumFrame
    3-characters wide
        temperature:
            -10 to -99
            -9$ to -0$
             0$ to  9$
            10$ to 99$
            100 to 999
        dew point:
            01ﾞ to 99ﾐ
        humidity:
             0% to  9%
            10% to 99%
            1∞%


/*
* A ScaledTimekeeper keeps (approximate) track of the passage of time for more than a year using only 2 bytes of RAM
* to maintain its state between calls.
* OK, obviously, the class itself takes RAM if you're using it on a platform that doesn't execute directly from flash
* (I developed it primarily with 8-bit AVR Arduino in mind), and it uses up to a few dozen bytes of RAM at a time
* during execution... but as far as I know, all of THAT RAM comes off the stack, not the heap, so it won't contribute
* to long-term heap fragmentation. If you happen to stumble over something in my code that DOES result in heap allocation,
* PLEASE file a highest-priority bug report.
*
* The diminishing resolution is my best attempt to strike a balance between "needing to know how much time has elapsed"
* and "being able to keep track of potentially long periods of time". My rationale was, "the more time that's elapsed
* since the event occurred, the less the PRECISION of the timestamp really matters".
*
* To be honest, this whole project started out when I was writing the code to display a 5-character timer, decided I
* wanted some way to gracefully handle overflows, then went wildly overboard attempting to handle even the most EGREGIOUS
* of overflows. Hence, the 1-second resolution for the first 100 minutes, and 60s resolution for the next ~100 hours.
*
*  10 20 30 40 50 (1' 2' 3' 4' 5')
*  15 30 45 00
*  00 20 40 (.0 .2 .4)
*

*
* Timekeeping resolution is as follows:
*      Scheme 1 (15-second resolution for second period)
*          enables 6-char display w/custom character, like 17:23~, where ~ is 00, 15, 30, 45 mashed together
*      exact static value between 00:00 and 99:59, plus other values to flag things like '--:--' and '??:??'
*          000 + 13
*      first 99m99s: 1-second
*          001+13
*      next 262h20m: 1-minute (period ends exactly 11 days (264 hours) after timekeeping begins to simplify remaining calculations
*          01+14
*      next 7 x 42 days: 15-minute
*          1nnn (nnn<111) + 12
*      last 4 x 42 days: 1-hour
*          1111nn + 10
*
*      Scheme 1
*          000+13: constant
*          001+13: first 99m99s (1s resolution), ends exactly 1h40m after timer start
*          01+14: next 262h20m (1m resolution), ends exactly 11 days (264h) after timekeeping begins
*          1nnx + 12: 7 x 42 days (15m resolution)
*          1111nn + 10: 4 x 42 days (1h resolution)
*
*      Scheme 2
*          000+13: constant
*          001+13: first 2 hours (1s resolution), ends exactly 2 hours after timer start
*          010+13: next 22 hours (10s resolution), ends exactly 1 day/24h after timer start
*          011+13: next 5 days (1m resolution), ends exactly 6d/144h after timer start
*          1nnnx + 12: 7 x 42 days (15m resolution)
*          1111nn + 10: 4 x 42 days (1 hour resolution)
*
*      Scheme 3 (10-second or better resolution for first 47 hours, 1-minute or better next 11 days
*          000+13: constant
*          001+13: first 2 hours (1s resolution)
*          01+14: next 45 hours (10s resolution)
*          10+14: 11 days (1-minute resolution)
*          1nnn (<1111) + 12: 7 x 42 days (15-minute resolution)
*          1111nn + 10: 4 x 42 days (1-hour resolution)
*
*      Alternate Scheme 1:
*          00+14: 4h32m (1s resolution)
*          01+14: next 22 hours (10s resolution)
*          1nnx+12: 7 x 42 days (15m resolution)
*          111110 + 10: 2 x 42 days (1 hour resolution)
*          111111 + 10: constant values from 00:00 to 99:55 in 5-second increments
*                       plus entire range from 00:00 to 00:59, plus 8 spares for hardwired values like --:-- and ??:??
*
*      Alternate Scheme 2:
*          000+13: 2h32m (1s resolution)
*          001+13: 
*
*          at 1s resolution, an entire 16-bit value can only encode 18 hours
*
*      24-bit scheme:
*          [00 01 10] + 22 = 48 days @ 1-second resolution
*          11zzz+19 = 7 x 30 days @ 5-second resolution
*          11111zzz+16 = 7 x 45 days @ 1-minute resolution
*          11111111zzz+13: undefined
*          11111111111+13 = specific and special values 00:00 to 99:59 plus others
*
*      32-bit scheme:
*          nnnnnnnnnn + 22 = 1024 x 48 days @ 1-second resolution (134 years)
*          to meet my goal of being able to go 1 year without overflow and 48-day periods, I need 8 periods
*          8 periods of 48 days with 1-second resolution requires 25 bits... 26, if I set aside a range for constant values.
*
*          if I set aside a 22-bit range for constant values,
*              26 bits will get me 720 days
*              27 bits will get me 1440 days (a little under 4 years)
*              28 bits will get me 2880 days (7.88 years)
*
*          if timer itself is 32 bits, but encoding millis, I can conveniently count seconds in 22 bits and use remainder for rollover counting
*
*          2^32 / 1000 = 0x418397
*          2^32 / 1024 = 0x3fffff
*
*          to represent a 32-bit timer with millis in seconds, I need 23 bits
*          if I pretend there are 1024ms per second...
*              error will accumulate at 1.44 seconds per minute, 86.4 seconds per hour, 2,073.6 seconds (34.5 minutes) per day.
*              86,400,000ms vs 88,473,600ms
*
*         if I divide a second into 1024 mibiseconds
*
*         divide millTime by 1000, save low 22 bits
*         goal = 48 x 24 x 60 x 60 = 4,147,200 seconds
*         max that can fit in 22 bits is 4,194,304
*         2^32ms / 1000 = 4,294,967. Damn, just a little too big.
*
*         Divide milliTime by 1000
*
*         if x is a 23-bit unsigned int packed into a 32-bit bitfield,
*         milliTime is a 32-bit unsigned int holding a millisecond offset
*         and I divide milliTime by 1000, is there any conceivable way it could make a difference whether or not
*         I AND it with 0x7fffff?
*
*         uint32_t millisecTime
*         // target is unsigned 23-bit value packed into a bitfield
*         target =
*
*         Suppose I create a 32-bit bitfield, and stick a 23-bit unsigned int into it.
*         Now, suppose I want to divide a 32-bit uint32_t by 1000, and store the result into it.
*         What does C++ *require* me to do to absolutely guarantee (and silence any warnings) that I can assign the
*         result of that division to my 23-bit unsigned
*
*
*
*
*      to test Arduino millis-overflow handling, do this:
*          noInterrupts();
*          timer0_millis = 4294901760; // set timer counter to 45 seconds before overflow
*          interrupts();
*/