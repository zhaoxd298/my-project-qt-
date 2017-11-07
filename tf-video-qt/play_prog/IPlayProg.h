#ifndef IPLAYPROG_H
#define IPLAYPROG_H

class IPlayProg {
public:
    virtual ~IPlayProg() {}
    virtual void keyPressed() = 0;
    virtual void startPlayProg() = 0;
    virtual void finishPlayProg() = 0;
};

#endif // IPLAYPROG_H
