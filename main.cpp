#define VR_LCD_EMU_STATIC 1
#include "EmulatedLcdWindow.h"
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <chrono>
#include <iostream>
#include <cmath>


#define LCD_PIXEL_DIMENSIONS 3.0

#include "formatter/DisplayManager.h"
#include "formatter/BaseFrame.h"
#include "formatter/StringFrame.h"
#include "formatter/SmashedDecimal.h"
#include "formatter/MultiStringFrame.h"
#include "formatter/MarqueeFrame.h"
#include "formatter/CustomCharManager.h"
#include "formatter/BaseNumberRenderer.h"
#include "formatter/NumberRenderer.h"
#include "formatter/NumberFrame.h"
#include "formatter/TimeCounter5charFrame.h"
#include "formatter/HexFrame.h"
#include "formatter/TimerFrame.h"
#include "formatter/ScaledTimekeeper.cpp"
#include "formatter/MmSsFrame.h"
#include "formatter/Timekeeper.h"

int main1();

int main() {
    return main1();
}

uint32_t millis() {
    return (uint32_t)( std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() & 0xFFFFFFFF);
}

int main2() {
    DeprecatedTimekeeper timekeeper;

    int64_t startTime = 0;
    for (int64_t startTime : {0, 0x7000, 0x8000, 0x9000, 0xf000}) {
        timekeeper.restart(startTime & 0xFFFFFFFF);
        for (int64_t offset = 0; offset < 6000000; offset++) {
            if (startTime == 0x7000)
                printf("break\n");
            uint32_t elapsed = timekeeper.getElapsedTime((startTime + offset) & 0xFFFFFFFF);

            if (elapsed != (offset / 1000)) {
                // @ToDo: craps out when currentTimeOffset = 0x7d0000 (8,192,000)
                printf("mismatch when startTime=%d, offset=%d, elapsed=%d\n", startTime, offset, elapsed);
                return 1;
            }
        }
    }
    printf("Passed!");
}

int mainTimerExperiment() {
    DisplayManager displayManager;
    EmulatedLcdWindow window = EmulatedLcdWindow(LCD_COLS, LCD_ROWS, LCD_PIXEL_DIMENSIONS);

    MmSsFrame expectedInt(0,15);
    //MmSsFrame expectedRounded(1,15);
    MmSsFrame actualInt(1, 15);
    MmSsFrame actualRounded(3,15);
    displayManager.addFrame(&expectedInt);
    //displayManager.addFrame(&expectedRounded);
    displayManager.addFrame(&actualInt);
    displayManager.addFrame(&actualRounded);
    Timekeeper timekeeper;
    uint32_t startTime = 0xfffff000;

    timekeeper.restart(startTime);
    //for (uint32_t milliTime = 0; milliTime < 6000000; milliTime+=13) {
    uint32_t target = millis() + 6000000;
    uint32_t milliTime = startTime;
    do {
        milliTime+=11;
        uint32_t expectedMilli = (milliTime > startTime) ? (milliTime - startTime) : (0x100000000 - startTime) + milliTime;
        expectedInt.setValue(expectedMilli / 1000);
        //expectedRounded.setValue((uint32_t)round(milliTime / 1000.0));
        actualInt.setValue(timekeeper.getAdjustedElapsedTime(milliTime));
        actualRounded.setValue(timekeeper.getRoundedElapsedTime(milliTime));

        displayManager.render(milliTime);

        for (int rowNumber = 0; rowNumber < LCD_ROWS; rowNumber++) {
            for (int x = 0; x < LCD_COLS; x++) {
                char *cp = displayManager.charAt(rowNumber, x);
                window.printChar(rowNumber, x, *cp);
            }
        }
        window.handleWindowEvents();
        //}
    } while (true);

}

int main1() {
    DisplayManager displayManager;
    EmulatedLcdWindow window = EmulatedLcdWindow(LCD_COLS, LCD_ROWS, LCD_PIXEL_DIMENSIONS);

    displayManager.addFrame(new StringFrame(0,0,8, "Status:"));



    MultiStringFrame* foo = new MultiStringFrame(1,0,4, 3);
    foo->setValue(0, 500, "Uno!");
    foo->setValue(1, 500, "Dos!");
    foo->setValue(2, 1000, "Tres");
    displayManager.addFrame(foo);

    MultiStringFrame* bar = new MultiStringFrame(1,5, 1, 3);
    bar->setValue(0, 500, "1");
    bar->setValue(1, 500, "2");
    bar->setValue(2, 1000, "3");
    displayManager.addFrame(bar);

    char nextChar[1];

    MultiStringFrame* pop = new MultiStringFrame(1, 12, 1, 5);
    *nextChar = 0x2e;
    pop->setValue(0, 100, nextChar);
    *nextChar = 0xa5;
    pop->setValue(1, 100, nextChar);
    *nextChar = 0xeb;
    pop->setValue(2, 100, nextChar);
    *nextChar = 0xde;
    pop->setValue(3, 100, nextChar);
    *nextChar = 0x20;
    pop->setValue(4, 800, nextChar);
    displayManager.addFrame(pop);

    CustomCharManager charManager;
    char twoDigitValue[2];
    charManager.renderNumberTo(twoDigitValue, 2, -36);

    displayManager.addFrame(new MarqueeFrame(0, 8, 12, 20, "System is running", 10, 50, 200));

    TimeCounter5charFrame te5(2, 0);
    te5.setValue(0,0);
    displayManager.addFrame(&te5);


    HexFrame* secondsFrame = (HexFrame*)displayManager.addFrame(new HexFrame(1, 14, 6));


    TimeCounter5charFrame* elapsedTime = (TimeCounter5charFrame*)displayManager.addFrame(new TimeCounter5charFrame(2,7));
    elapsedTime->startCountingFromZero((uint32_t)(millis()));

    TimeCounter5charFrame* slowCountdown = (TimeCounter5charFrame*)displayManager.addFrame(new TimeCounter5charFrame(3,15));
    slowCountdown->startLongCountdown(6,30, millis());



    TimeCounter5charFrame* countdown = (TimeCounter5charFrame*)displayManager.addFrame(new TimeCounter5charFrame(2, 14));
    countdown->startCountdown(22,10, millis());


    TimerFrame staticTimeFrame(3,0);
    displayManager.addFrame(&staticTimeFrame);
    staticTimeFrame.startTimer(millis() - (2 * 60 * 60 * 1000) - (15 * 60 * 1000));



    /*
    SmashedDecimal* smashed[6];
    for (int x=0; x<6; x++)
        smashed[x] = new SmashedDecimal(&window,x, false, x+5);
    //window.printChar(3,0, "Text on row 4 of LCD");
    window.print(3,0," %1. 72. 73. 74. 75.");
    smashed[5]->setValue(999);
    window.printChar(3,1,5);
    window.printChar(3,3,0);
    window.printChar(3,7, 1);
    window.printChar(3,11, 2);
    window.printChar(3,15, 3);
    window.printChar(3, 19, 4);
*/

    // @ToDo: refactor so NumberRenderer<> combines NumberRenderer<> and NumberFrame

    NumberRenderer<3> number(&charManager, 69.0);
    displayManager.addFrame(new NumberFrame(1,9, 3, &number));

    NumberRenderer<2> number2(&charManager, 9.4);
    NumberFrame* numberFrame2 = (NumberFrame*)displayManager.addFrame(new NumberFrame(1, 6, 2, &number2));



    int testval = 5;
    int test2 = 0;
    unsigned long long lastChange = 0;


    for (uint32_t fakeTime = 0; fakeTime <0xffff; fakeTime++) {
        uint32_t testValue = countdown->test(fakeTime, 8190);
        if (countdown->isSame(fakeTime, (uint16_t)testValue, 8190) == false) {
            std::cout << "different at " << fakeTime;
            countdown->test(fakeTime, 8190);
            countdown->isSame(fakeTime, testValue, 8190);
        }
    }





    while (window.isOpen()) {

        if (lastChange == 0)
            lastChange = millis();

        if (millis() > (lastChange + 300)) {
            numberFrame2->set(testval);
            lastChange = millis();
            testval++;
            if (testval > 110)
                testval = -50;


        }
        te5.setValue(0, test2++);

        secondsFrame->set(millis() / 1000);


        displayManager.render(millis());

        for (int rowNumber = 0; rowNumber < LCD_ROWS; rowNumber++) {
            for (int x=0; x<LCD_COLS; x++) {
                char* cp = displayManager.charAt(rowNumber, x);
                window.printChar(rowNumber, x, *cp);
            }
        }
        window.handleWindowEvents();

    }
}