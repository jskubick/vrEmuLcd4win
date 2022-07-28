//
// Created by root on 7/27/2022.
//

#include <cmath>
#include "CustomCharManager.h"

bool render2digits(char* pTarget, float value) {
    int truncatedValue = round(value);

    if (truncatedValue == 0) {
        pTarget[0]=' ';
        pTarget[1]='0';
        return true;
    }

    if (truncatedValue > 0) {
        if (truncatedValue < 100) {
            char three[3];
            int first = 0;
            if (truncatedValue < 10) {
                three[0] = ' ';
                first = 1;
            }

            itoa(truncatedValue, three, 10);
            for (int x=0; x<2-first; x++)
                *(pTarget+first+x) = three[x];
            return true;
        }
        pTarget[0]='+';
        pTarget[1]=OVERFLOW_CHAR;
        return false;
    }

    if (truncatedValue < 0) {
        pTarget[0] = '-';
        if (truncatedValue > -10) {
            pTarget[1] = 0x30 - truncatedValue;
            return true;
        }
        if (truncatedValue > -36) {
            pTarget[1] = 0x41 - (truncatedValue + 10);
            return true;
        }
        pTarget[1] = OVERFLOW_CHAR;
        return false;
    }

}

bool render3digits(char* pTarget, float value) {

}

bool CustomCharManager::renderNumberTo(char* pTarget, int digits, float value) {
    if (digits == 2)
        render2digits(pTarget, value);
    if (digits == 3)
        render3digits(pTarget, value);
}
