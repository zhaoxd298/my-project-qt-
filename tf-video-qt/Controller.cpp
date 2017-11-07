#include "Controller.h"

const char model[] = {"TF-QB5"};
const u8 card_ver[2] = {0x97, 0x05};
const u8 soft_ver[3] = {1, 2, 2};

Controller::Controller()
{
    m_net = new Net(this);
    m_usb = new usb_t;
    m_media = new Media;
    m_board = new Board;
    m_display = Display::getInstance();
    m_playProg = new PlayProg(m_display);

    m_display->show();

    m_testKey = new TestKey(m_playProg);
    m_testKey->show();
}

Controller::~Controller()
{
    delete m_net;
    delete m_usb;
    delete m_playProg;
    delete m_media;
    delete m_board;

    delete m_testKey;
}
