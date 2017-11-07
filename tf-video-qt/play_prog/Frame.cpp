#include "Frame.h"
#include "Log.h"

Frame::Frame()
{
    m_enable = false;
    m_type = 0;
    m_action = 0;
    m_step = 0;
    m_speed = 0;
    m_flatEnable = false;
    m_imgWidth = 0;
    m_imgHeight = 0;
    m_winWidth = 0;
    m_winHeight = 0;
    m_color = 0;

    m_status.imgCache = NULL;
}

Frame::~Frame()
{

}

bool Frame::enable()
{
    return m_enable;
}

u32 Frame::imgWidth()
{
    return m_imgWidth;
}

u32 Frame::imgHeight()
{
    return m_winHeight;
}

bool Frame::loadAttr(QDomElement attr)
{
    while (!attr.isNull()) {
        if ("framesize" == attr.tagName()) {
            QByteArray ba = attr.text().toLatin1();
            char* s = ba.data();
            int ret = sscanf(s, "%d,%d", &(m_imgWidth), &(m_imgHeight));
            if ( 2 != ret ) {
                print_error("parse program framesize error");
            }

        }else if ("frameenable" == attr.tagName()) {
            m_enable = attr.text().toInt();

        } else if ("framepath" == attr.tagName()) {
            QByteArray ba = attr.text().toLatin1();
            char* s = ba.data();
            int len = sizeof(path) < strlen(s) ? sizeof(path)-1 : strlen(s);
            memset(path, 0, sizeof(path));
            memcpy(path, s, len);

        } else if ("frameaction" == attr.tagName()) {
            m_action = attr.text().toInt();

        } else if ("framestep" == attr.tagName()) {
            m_step = attr.text().toInt();

        } else if ("framespeed" == attr.tagName()) {
            m_speed = attr.text().toInt();

        } else if ("frameflat" == attr.tagName()) {
            m_flatEnable = attr.text().toInt();

        }

        attr = attr.nextSiblingElement();
    }

    return true;
}

void Frame::setWinSize(u32 w, u32 h)
{
    m_winWidth = w;
    m_winHeight = h;
}

bool Frame::initResource()
{
    if (false == m_enable) {
        return true;
    }

    memset(&m_status, 0, sizeof(FrameStatus));
    m_status.status = FRAME_UNREADY;
    m_status.framePos = 0;
    m_status.imgCache = new char[m_imgWidth * m_imgHeight * 4];

    return true;
}

void Frame::releaseResource()
{
    if (false == m_enable) {
        return;
    }

    delete[] m_status.imgCache;
}

/*播放边框*/
bool Frame::play()
{

    return true;
}

bool Frame::dataOk()
{
    return m_status.dataOk;
}
void Frame::clearDataOk()
{
    m_status.dataOk = false;
}
