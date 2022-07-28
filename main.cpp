#define VR_LCD_EMU_STATIC 1
#include "EmulatedLcdWindow.h"
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <chrono>


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

int main() {
    DisplayManager displayManager;
    EmulatedLcdWindow window = EmulatedLcdWindow(LCD_COLS, LCD_ROWS, LCD_PIXEL_DIMENSIONS);

    displayManager.addFrame(new StringFrame(0,0,8, "Status:"));
    displayManager.addFrame(new StringFrame(2,0,4, "Left"));
    displayManager.addFrame(new StringFrame(2,7, 6, "Middle"));
    displayManager.addFrame(new StringFrame(2, 15, 5, "Right"));


    MultiStringFrame* foo = new MultiStringFrame(1,5,4, 3);
    foo->setValue(0, 500, "Uno!");
    foo->setValue(1, 500, "Dos!");
    foo->setValue(2, 1000, "Tres");
    displayManager.addFrame(foo);

    MultiStringFrame* bar = new MultiStringFrame(1,10, 1, 3);
    bar->setValue(0, 500, "1");
    bar->setValue(1, 500, "2");
    bar->setValue(2, 1000, "3");
    displayManager.addFrame(bar);

    char* nextChar;

    MultiStringFrame* pop = new MultiStringFrame(1, 19, 1, 5);
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
    displayManager.addFrame(new NumberFrame(3,17, 3, &number));

    NumberRenderer<2> number2(&charManager, 9.4);
    NumberFrame* numberFrame2 = (NumberFrame*)displayManager.addFrame(new NumberFrame(3, 14, 2, &number2));


    int testval = 5;
    unsigned long long lastChange = 0;
    while (window.isOpen()) {
        // @ToDo: tidy this up
        auto ahora = std::chrono::system_clock::now().time_since_epoch();
        auto currentTimeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(ahora).count();

        if (lastChange == 0)
            lastChange = currentTimeMillis;

        if (currentTimeMillis > (lastChange + 300)) {
            numberFrame2->set(testval);
            lastChange = currentTimeMillis;
            testval++;
            if (testval > 110)
                testval = -50;

        }

        displayManager.render((int)(currentTimeMillis & 0x7fffffff));

        for (int rowNumber = 0; rowNumber < LCD_ROWS; rowNumber++) {
            for (int x=0; x<LCD_COLS; x++) {
                char* cp = displayManager.charAt(rowNumber, x);
                window.printChar(rowNumber, x, *cp);
            }
        }

        window.handleWindowEvents();




        //Sleep(100);
    }
}
