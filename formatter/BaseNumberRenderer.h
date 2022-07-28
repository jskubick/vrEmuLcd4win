#ifndef FAKELCD_BASENUMBERRENDERER_H
#define FAKELCD_BASENUMBERRENDERER_H


#include "CustomCharManager.h"

class BaseNumberRenderer {

public:

    BaseNumberRenderer(CustomCharManager* pCustomCharManager, float value);
    void setValue(float value);
    void renderValue(char* dest);

protected:
    virtual char* getBuffer() = 0;
    virtual int getBufferSize() = 0;
private:
    float rawValue;
    int get10xValue();
    CustomCharManager* pCustomCharManager;

    void render2digitValue(char* dest);
    void render3digitValue(char* dest);
};


#endif //FAKELCD_BASENUMBERRENDERER_H
