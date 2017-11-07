#include "TestKey.h"
#include "Log.h"
#include <QDebug>

TestKey::TestKey(IPlayProg *playProg, QWidget *parent)
    : QWidget(parent), m_playProg(playProg), m_keyBtn(this)
{
    m_keyBtn.setText("Test");
    connect(&m_keyBtn, SIGNAL(clicked(bool)), this, SLOT(onKeyBtn()));

    resize(150, 100);
}


void TestKey::onKeyBtn()
{
    myDebug("void TestKey::onKeyBtn()\n");
    m_playProg->keyPressed();
}
