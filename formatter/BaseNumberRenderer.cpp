//
// Created by root on 7/27/2022.
//

#include <cmath>
#include <cstring>
#include "BaseNumberRenderer.h"
#include "DisplayManager.h"


BaseNumberRenderer::BaseNumberRenderer(CustomCharManager* pCustomCharManager, float initialValue)
    : rawValue(initialValue),
      pCustomCharManager(pCustomCharManager) {}

void BaseNumberRenderer::setValue(float newValue) {
    rawValue = newValue;
}

void BaseNumberRenderer::render2digitValue(char* dest) {
    render3digitValue(dest);
}

void BaseNumberRenderer::render3digitValue(char* dest) {
    char buf[LCD_COLS+1]; // on stack, so it's guaranteed to go away when method returns
    itoa(get10xValue(), buf, 10);
    int length = strlen(buf);
    int width = getBufferSize();
    if (length > width) {
        for (int x=0; x<width; x++)
            dest[x] = 0x3f;
        return;
    }
    int leftPadLength = (width - length);
    for (int x=0; x<leftPadLength; x++) {
        dest[x] = 0x20;
    }
    for (int x=0; x<width; x++) {
        dest[x+leftPadLength] = buf[x];
    }

    // @ToDo: remove this!
    //dest[2+leftPadLength] = 0xeb;
}

void BaseNumberRenderer::renderValue(char* dest) {
    if (getBufferSize() == 3) {
        render3digitValue(dest);
        return;
    }

    if (pCustomCharManager != nullptr) {
        pCustomCharManager->renderNumberTo(dest, getBufferSize(), rawValue);
        return;
    }

    if (getBufferSize() == 2)
        render2digitValue(dest);
}

int BaseNumberRenderer::get10xValue() {
    return (int)round(rawValue * 10);
}