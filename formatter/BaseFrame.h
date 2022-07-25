#ifndef FAKELCD_BASEFRAME_H
#define FAKELCD_BASEFRAME_H


#include <cstdint>

class BaseFrame {
public:
    BaseFrame(int row, int startCol, int width);
    virtual bool renderInto(char* buf, int currentTimeMillis);
protected:
    const uint8_t row;
    const uint8_t startCol;
    const uint8_t width;
    bool copy(char* src, char* dest, int len);
};

#endif //FAKELCD_BASEFRAME_H
