//
// Created by root on 7/27/2022.
//

#ifndef FAKELCD_CUSTOMCHARMANAGER_H
#define FAKELCD_CUSTOMCHARMANAGER_H

#define OVERFLOW_CHAR 0xe3

class CustomCharManager {
public:

    bool hasDecimalFifths();

    // @ToDo: deprecated
    bool renderNumberTo(char* pTarget, int digits, float value);
};


#endif //FAKELCD_CUSTOMCHARMANAGER_H
