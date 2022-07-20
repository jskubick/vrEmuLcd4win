//
// Created by root on 7/14/2022.
//

#ifndef FAKELCD_BASEFRAME_H
#define FAKELCD_BASEFRAME_H


class BaseFrame {
public:
    BaseFrame(char* pointer, int length);
    void update(char* newValue);

    virtual bool render(int millis);
protected:
    char* buf;
    const int maxLength;
    bool copy(char* src, char* dest, int len);
};


#endif //FAKELCD_BASEFRAME_H
