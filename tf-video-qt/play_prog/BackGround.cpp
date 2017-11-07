#include "BackGround.h"

BgScreen::BgScreen()
{

}

BgScreen::~BgScreen()
{

}

bool BgScreen::loadAttr(QDomElement attr)
{
    while (!attr.isNull()) {
        if ("bgactionenable" == attr.tagName()) {
           m_enable = attr.text().toInt();

        } else if ("bgactionspeed" == attr.tagName()) {
           m_speed = attr.text().toInt();

        } else if ("bgactiondelay" == attr.tagName()) {
           m_delay = attr.text().toInt();

        } else if ("bgactionimg" == attr.tagName()) {
           QByteArray ba = attr.text().toLatin1();
           char* s = ba.data();
           int len = sizeof(path) < strlen(s) ? sizeof(path)-1 : strlen(s);
           memset(path, 0, sizeof(path));
           memcpy(path, s, len);

           QString e = attr.attributeNode("enable").value();
           m_imgCnt = e.toInt();
        }

        attr = attr.nextSiblingElement();
    }

    return true;
}

void BgScreen::setWinSize(u32 w, u32 h)
{
    m_posSize.x = 0;
    m_posSize.y = 0;
    m_posSize.w = w;
    m_posSize.h = h;
    m_posSize.imgWidth = w;
    m_posSize.imgHeight = h;
}

void BgScreen::setOffset(u32 x, u32 y)
{
    m_posSize.xOffset = x;
    m_posSize.yOffset = y;
}


/*播放背景动画*/
bool BgScreen::play()
{

    return true;
}

bool BgScreen::dataOk()
{
    return m_dataOk;
}

void BgScreen::clearDataOk()
{
    m_dataOk = false;
}


BgMusic::BgMusic()
{

}

BgMusic::~BgMusic()
{

}

bool BgMusic::loadAttr(QDomElement attr)
{
    while (!attr.isNull()) {
        if ("bgmusic" == attr.tagName()) {
           QByteArray ba = attr.text().toLatin1();
           char* s = ba.data();
           int len = sizeof(path) < strlen(s) ? sizeof(path)-1 : strlen(s);
           memset(path, 0, sizeof(path));
           memcpy(path, s, len);

           QString e = attr.attributeNode("enable").value();
           m_enable = e.toInt();

        } else if ("soundvolume" == attr.tagName()) {
            m_volume = attr.text().toInt();

        }

         attr = attr.nextSiblingElement();
    }

    return true;
}

/*播放背景音乐*/
bool BgMusic::play()
{

    return true;
}
