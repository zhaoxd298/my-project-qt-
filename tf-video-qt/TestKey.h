#ifndef TESTKEY_H
#define TESTKEY_H

#include "play_prog/IPlayProg.h"
#include <QWidget>
#include <QPushButton>

class TestKey : public QWidget
{
    Q_OBJECT
private:
    IPlayProg* m_playProg;
    QPushButton m_keyBtn;
public:
    explicit TestKey(IPlayProg* playProg, QWidget *parent = 0);

public slots:
    void onKeyBtn();
};

#endif // TESTKEY_H
