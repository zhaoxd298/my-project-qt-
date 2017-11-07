#ifndef _IDISPLAY_H_
#define _IDISPLAY_H_

#include <QGraphicsItem>

class IDisplay
{
public:
    virtual void displayImage(const char* buf, int w, int h) = 0;
    virtual void clearDisplay() = 0;
    virtual ~IDisplay()
    {}
};

#endif
