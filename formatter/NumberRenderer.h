//
// Created by root on 7/27/2022.
//

#ifndef FAKELCD_NUMBERRENDERER_H
#define FAKELCD_NUMBERRENDERER_H


#include "BaseNumberRenderer.h"
template<int SIZE>
class NumberRenderer : public BaseNumberRenderer {
public:
    NumberRenderer(CustomCharManager* pCustomCharManager, float value);
    char* getBuffer();
    int getBufferSize();
    void renderValue(char* dest);

private:
    char buffer[SIZE];
    int width = SIZE;
};

template<int SIZE>
NumberRenderer<SIZE>::NumberRenderer(CustomCharManager* pCustomCharManager, float initialValue )
    : BaseNumberRenderer(pCustomCharManager, initialValue) {}

template<int SIZE>
char* NumberRenderer<SIZE>::getBuffer() {
    return buffer;
}

template<int SIZE>
int NumberRenderer<SIZE>::getBufferSize() {
    return width;
}

template<int SIZE>
void NumberRenderer<SIZE>::renderValue(char* dest) {
    return BaseNumberRenderer::renderValue(dest);
}

#endif //FAKELCD_NUMBERRENDERER_H
