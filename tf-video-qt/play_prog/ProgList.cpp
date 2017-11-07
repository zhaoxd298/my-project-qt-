#include <QFile>

#include "Controller.h"
#include "ProgList.h"
#include "Log.h"
#include "../media/Animat.h"
#include "../display/Display.h"
#include <QFile>


ComAttr::ComAttr(QDomElement pageNode, page_type_e pageType) :
    m_pageType(pageType)
{
    while (!pageNode.isNull()) {
        if ("action" == pageNode.tagName()) {
            m_animat = pageNode.text().toInt();

        } else if ("speed" == pageNode.tagName()) {
            m_speed = pageNode.text().toInt();

        } else if ("delay" == pageNode.tagName()) {
            m_delay = pageNode.text().toInt();

        } else if ("exittype" == pageNode.tagName()) {
            m_exitAction = pageNode.text().toInt();

        } else if ("exitspeed" == pageNode.tagName()) {
            m_exitSpeed = pageNode.text().toInt();

        } else if ("step" == pageNode.tagName()) {
            m_step = pageNode.text().toInt();

        } else if ("backgroundenable" == pageNode.tagName()) {
            m_partBgAttr.enable = pageNode.text().toInt();

        } else if ("backgroundaction" == pageNode.tagName()) {
            m_partBgAttr.action = pageNode.text().toInt();

        } else if ("backgrounddelay" == pageNode.tagName()) {
            m_partBgAttr.delay = pageNode.text().toInt();

        } else if ("backgroundpath" == pageNode.tagName()) {
            QByteArray ba = pageNode.text().toLatin1();
            char* s = ba.data();
            int len = PATH_LEN < strlen(s) ? PATH_LEN-1 : strlen(s);
            memset(m_partBgAttr.path, 0, PATH_LEN);
            memcpy(m_partBgAttr.path, s, len);

            QString e = pageNode.attributeNode("type").value();
            m_partBgAttr.imgCnt = e.toInt();

        } else if ("overlaptype" == pageNode.tagName()) {
            m_overlapType = pageNode.text().toInt();

        } else if ("path" == pageNode.tagName()) {
            QByteArray ba = pageNode.text().toLatin1();
            char* s = ba.data();
            int len = PATH_LEN < strlen(s) ? PATH_LEN-1 : strlen(s);
            memset(m_path, 0, PATH_LEN);
            memcpy(m_path, s, len);

            QString e = pageNode.attributeNode("count").value();
            m_imgCnt = e.toInt();

        } else if ("strokeenable" == pageNode.tagName()) {
            QByteArray ba = pageNode.text().toLatin1();
            char* s = ba.data();
            int a, b, c, d;
            int ret = sscanf(s, "%d,%d,%d,%d", &a, &b, &c, &d);
            if (4 != ret) {
                print_error("parse strokeenable error");
            } else {
                m_strokeAttr.leftEnable = static_cast<bool>(a);
                m_strokeAttr.rightEnable = static_cast<bool>(b);
                m_strokeAttr.upEnable = static_cast<bool>(b);
                m_strokeAttr.downEnable = static_cast<bool>(b);
            }

        } else if ("strokecolor" == pageNode.tagName()) {
            QByteArray ba = pageNode.text().toLatin1();
            char* s = ba.data();
            int ret = sscanf(s, "%x,%x,%x,%x", &(m_strokeAttr.leftColor),
                                               &(m_strokeAttr.rightColor),
                                               &(m_strokeAttr.upColor),
                                               &(m_strokeAttr.downColor));
            if (4 != ret) {
                print_error("parse strokecolor error");
            }

        } else if ("strokewidth" == pageNode.tagName()) {
            m_strokeAttr.strokeWidth = pageNode.text().toInt();

        }

        pageNode = pageNode.nextSiblingElement();
    }
}

ComAttr::~ComAttr()
{

}


bool ComAttr::play(PlayStatus& playStatus, PlayStatus& bgStatus, const PosSize& posSize)
{
    //myDebug("page type:%d\n", m_pageType);
    //QThread::sleep(1);

    (void)bgStatus;
    // 当前窗口所有内容播放完毕，还没有将数据发送出去，直接返回
    if (PlayProg::PLAY_DONE == playStatus.status) {
        return true;
    }

    if (PlayProg::PLAY_DELAY == playStatus.status) {
        if (remain_tm(playStatus.lastTm, m_delay) <= 0) {
            playStatus.imgNum ++;
            if (playStatus.imgNum >= m_imgCnt) {
                playStatus.status = PlayProg::PLAY_DONE;
                return true;
            } else {
                playStatus.status = PlayProg::PLAY_UNREADY;
            }
        } else {
            return true;
        }
    }

    if (PlayProg::PLAY_PAUSE == playStatus.status) {
        //QThread::msleep(10);
        if (remain_tm(playStatus.lastTm, m_speed * PlayProg::OFFSET_SPEED + PlayProg::BASE_SPEED) <= 0) {
            if (playStatus.movCnt >= playStatus.totalMovCnt) {
                playStatus.status = PlayProg::PLAY_DELAY;
                return true;
            } else {
                playStatus.status = PlayProg::PLAY_READY;
            }
        } else {
            return true;
        }
    }

    if (PlayProg::PLAY_UNREADY == playStatus.status) {
        char imgPath[PATH_LEN] = {0};
        sprintf(imgPath, "%s%s%d", PROG_ROOT, m_path, playStatus.imgNum);
        QFile file(imgPath);
        if (!file.exists()) {   /*欲显示的图片不存在，继续显示下一张*/
            playStatus.imgNum ++;
            if (playStatus.imgNum >= m_imgCnt) {
                playStatus.status = PlayProg::PLAY_DONE;
            }
            return true;
        }

        bool ret = Media::decodeBmp(playStatus.imgCache, imgPath, posSize.imgWidth, posSize.imgHeight);
        if (!ret) {
            print_error("decode bmp error");
            return false;
        }

        if (PA_Random == m_animat) {
            u8 lastAnimat = m_actualAnimat;
            while (lastAnimat == m_actualAnimat) {
                srand(get_cur_tm());
                m_actualAnimat = rand() % PA_ACTION_CNT + 1;
            }
        } else {
            m_actualAnimat = m_animat;
        }

        AnimatType animat = static_cast<AnimatType>(m_actualAnimat);
        playStatus.totalMovCnt = Animat::getAnimatActionCnt(animat, posSize.imgWidth, posSize.imgHeight);
        playStatus.movCnt = 0;
        playStatus.flag = 0;
        memset(playStatus.mark, 0, 256);
        playStatus.status = PlayProg::PLAY_READY;
    }

    if (PlayProg::PLAY_READY != playStatus.status) {
        print_error("error play status:%d", playStatus.status);
        return true;
    }

    AnimatType animat = static_cast<AnimatType>(m_actualAnimat);
    bool ret = Animat::playAnimat(posSize, playStatus, m_step, animat);
    if (!ret) {
        print_error("play animat error");
        return false;
    }

    playStatus.dataOk = true;
    playStatus.lastTm = get_cur_tm();
    playStatus.status = PlayProg::PLAY_PAUSE;

    return true;
}


VideoAttr::VideoAttr(QDomElement pageNode, page_type_e pageType) :
    ComAttr(pageNode, pageType)
{
    while (!pageNode.isNull()) {



        pageNode = pageNode.nextSiblingElement();
    }
}

VideoAttr::~VideoAttr()
{

}

WatchPlateAttr::WatchPlateAttr(QDomElement pageNode, page_type_e pageType) :
    ComAttr(pageNode, pageType)
{

}

WatchPlateAttr::~WatchPlateAttr()
{

}

TimeAttr::TimeAttr(QDomElement pageNode, page_type_e pageType) :
    ComAttr(pageNode, pageType)
{

}

TimeAttr::~TimeAttr()
{

}

TemHumAttr::TemHumAttr(QDomElement pageNode, page_type_e pageType) :
    ComAttr(pageNode, pageType)
{

}

TemHumAttr::~TemHumAttr()
{

}


CntDownAttr::CntDownAttr(QDomElement pageNode, page_type_e pageType) :
    ComAttr(pageNode, pageType)
{

}

CntDownAttr::~CntDownAttr()
{

}


SpecialStopwatchAttr::SpecialStopwatchAttr(QDomElement pageNode, page_type_e pageType) :
    ComAttr(pageNode, pageType)
{

}

SpecialStopwatchAttr::~SpecialStopwatchAttr()
{

}


SpecialCountAttr::SpecialCountAttr(QDomElement pageNode, page_type_e pageType) :
    ComAttr(pageNode, pageType)
{

}

SpecialCountAttr::~SpecialCountAttr()
{

}


SpecialCustomAttr::SpecialCustomAttr(QDomElement pageNode, page_type_e pageType) :
    ComAttr(pageNode, pageType)
{

}

SpecialCustomAttr::~SpecialCustomAttr()
{

}


WindowAttr::WindowAttr()
{

}

WindowAttr::WindowAttr(QDomElement winNode)
{
    m_winFrame.loadAttr(winNode);

    while (!winNode.isNull()) {
        if ("rect" == winNode.tagName()) {
            QByteArray ba = winNode.text().toLatin1();
            char* s = ba.data();
            u32 x0, y0, x1, y1;
            int ret = sscanf(s, "%u,%u,%u,%u", &x0, &y0, &x1, &y1);
            if (ret != 4) {
                print_error("parse win rect error");
            } else {
                m_posSize.x = x0;
                m_posSize.y = y0;
                m_posSize.w = x1 - x0;
                m_posSize.h = y1 - y0;
            }
        } else if ("overpadtype" == winNode.tagName()) {
            m_overlapType = winNode.text().toInt();

        } else if ("page" == winNode.tagName()) {
            QString e = winNode.attributeNode("type").value();
            int page_type = e.toInt();
            ComAttr* com_attr = getNewPage(winNode.firstChildElement(), (page_type_e)page_type);
            if (com_attr) {
                m_page.append(com_attr);
            } else {
                print_error("get_new_page error");
            }
        }

        winNode = winNode.nextSiblingElement();
    }

    m_winFrame.setWinSize(m_posSize.w, m_posSize.h);

    if (m_winFrame.enable()) {
        m_posSize.xOffset = m_winFrame.imgWidth();
        m_posSize.yOffset = m_winFrame.imgWidth();
    } else {
        m_posSize.xOffset = 0;
        m_posSize.yOffset = 0;
    }

    m_posSize.imgWidth = m_posSize.w - m_posSize.yOffset * 2;
    m_posSize.imgHeight = m_posSize.h - m_posSize.yOffset * 2;
}

WindowAttr::~WindowAttr()
{
}

QRectF WindowAttr::boundingRect() const
{
    return QRectF(0, 0, m_posSize.w, m_posSize.h);
}

void WindowAttr::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QImage img((u8*)m_playStatus.swapCache, boundingRect().width(), boundingRect().height(), QImage::Format_RGB32);
    QPixmap pixmap = QPixmap::fromImage(img);

    painter->drawPixmap(0, 0, pixmap);
}

bool WindowAttr::initResource()
{
    m_curPageNum = 0;

    m_winFrame.initResource();

    memset(&m_playStatus, 0, sizeof(PlayStatus));   // 确保初始状态全部为0

    int cacheSize = m_posSize.w * m_posSize.h * 4 * 2;

    m_playStatus.imgCache = new char[cacheSize];
    m_playStatus.swapCache = new char[cacheSize];
    m_playStatus.tmpCache = new char[cacheSize];

    // 用于当前分区背景动画(目前只用于文本)
    //m_bgStatus.imgCache = new char[cacheSize];
    //m_bgStatus.swapCache = new char[cacheSize];

    return true;
}

void WindowAttr::releaseResource()
{
    //delete[] m_bgStatus.swapCache;
    //delete[] m_bgStatus.imgCache;

    delete[] m_playStatus.tmpCache;
    delete[] m_playStatus.swapCache;
    delete[] m_playStatus.imgCache;
}

/*播放窗口内容*/
bool WindowAttr::play()
{
    m_page[m_curPageNum]->play(m_playStatus, m_bgStatus, m_posSize);

    if (dataOk()) {
        //update();
    }

    if (PlayProg::PLAY_DONE == m_playStatus.status) {
        m_curPageNum ++;
        if (m_curPageNum >= m_page.size()) {
            m_curPageNum = 0;
            m_playStatus.playOver = true;
        }
        m_playStatus.status = PlayProg::PLAY_UNREADY;
        m_playStatus.imgNum = 0;
    }

    return true;
}

bool WindowAttr::dataOk()
{
    return (m_bgStatus.dataOk || m_playStatus.dataOk);
}

void WindowAttr::clearDataOk()
{
    m_bgStatus.dataOk = false;
    m_playStatus.dataOk = false;
}

bool WindowAttr::playOver()
{
    return m_playStatus.playOver;
}

char* WindowAttr::winCache()
{
    return m_playStatus.swapCache;
}

PosSize WindowAttr::posSize()
{
    return m_posSize;
}
u8 WindowAttr::overlapType()
{
    return m_overlapType;
}

/*
* 创建page （简单工厂方法）
*/
ComAttr* WindowAttr::getNewPage(QDomElement pageNode, page_type_e pageType)
{
    ComAttr* com_attr = NULL;
    switch (pageType) {
        case PROG_TEXT: 		// 文本
        case PROG_PIC:			// 图片
        case PROG_DAZZLE_TEXT: 	// 炫彩字
        case PROG_TABLE: 		// 表格
            com_attr = new ComAttr(pageNode, pageType);
            break;
        case PROG_VIDEO:		// 视屏
            com_attr = new VideoAttr(pageNode, pageType);
            break;
        case PROG_WATCH_PLATE: 	// 表盘
            com_attr = new WatchPlateAttr(pageNode, pageType);
            break;
        case PROG_TIME: 		// 时间
            com_attr = new TimeAttr(pageNode, pageType);
            break;
        case PROG_TEM_HUM: 		// 温湿度
            com_attr = new TemHumAttr(pageNode, pageType);
            break;
        case PROG_CNT_DOWN: 	// 倒计时
            com_attr = new CntDownAttr(pageNode, pageType);
            break;
        case PROG_STOPWATCH:	// 特殊区(秒表)
            com_attr = new SpecialStopwatchAttr(pageNode, pageType);
            break;
        case PROG_COUNT:		// 特殊区(计数)
            com_attr = new SpecialCountAttr(pageNode, pageType);
            break;
        case PROG_CUSTOM:		// 特殊区(自定义)
        case PROG_PHOTO:		// 特殊区(图片区)
            com_attr = new SpecialCustomAttr(pageNode, pageType);
            break;
        default:
            print_error("unknown page type:%d", pageType);
            break;
    }

    return com_attr;
}

void WindowAttr::releaseProg()
{
    for (int i=0; i<m_page.size(); i++) {
        delete m_page[i];
    }
    m_page.clear();
}


ProgAttr::ProgAttr()
{

}

ProgAttr::ProgAttr(QDomElement prog_node)
{
    loadAttr(prog_node);

    Display* display = Display::getInstance();

    connect(this, SIGNAL(addQGraphicsItem(QGraphicsItem*,int,int)),
            display, SLOT(onAddQGraphicsItem(QGraphicsItem*,int,int)));
    connect(this, SIGNAL(clearQGraphicsItem()), display, SLOT(onClearQGraphicsItem()));
}

ProgAttr::~ProgAttr()
{

}


bool ProgAttr::initResource()
{
    m_progFrame.initResource();

    for (int i=0; i<m_win.size(); i++) {
        m_win[i]->initResource();
        //display->addQGraphicsItem(m_win[i], m_win[i]->posSize().x, m_win[i]->posSize().y);
        emit addQGraphicsItem(m_win[i], m_win[i]->posSize().x, m_win[i]->posSize().y);
    }

    return true;
}

void ProgAttr::releaseResource()
{
    m_progFrame.releaseResource();

    emit clearQGraphicsItem();

    for (int i=0; i<m_win.size(); i++) {
        m_win[i]->releaseResource();
    }

    IDisplay* display = Display::getInstance();
    display->clearDisplay();

    //  ????????
}

/*播放节目*/
bool ProgAttr::play()
{
    bool dataOk = false;
    bool allWinPlayOver = true;

    m_playFlag = true;

    initResource();

    while (m_playFlag) {
        dataOk = false;
        allWinPlayOver = true;

        if (false == m_bgMusic.play()) {

        }

        if (false == m_bgScr.play()) {

        }
        dataOk |= m_bgScr.dataOk();
        m_bgScr.clearDataOk();

        if (false == m_progFrame.play()) {

        }
        dataOk |= m_progFrame.dataOk();
        m_progFrame.clearDataOk();

        for (int i=0; i<m_win.size(); i++) {
            if (false == m_win[i]->play())
            {

            }
            dataOk |= m_win[i]->dataOk();
            m_win[i]->clearDataOk();

            allWinPlayOver &= m_win[i]->playOver();

            if (!m_playFlag) {
                break;
            }
        }

        if (dataOk) {   // display program
            displayProg();
        }

        if (allWinPlayOver) {   // 所有窗口都播放完了，退出
            break;
        }
    }

    releaseResource();
    m_stopFlag = true;

    return true;
}

void ProgAttr::displayProg()
{
    PlayCache* playCache = PlayCache::getInstance();

    for (int i=m_win.size()-1; i>=0; i--) {
        overlaysWindow(playCache->fbp_prog, m_win[i]->winCache(), m_win[i]->posSize(), m_win[i]->overlapType());
        m_win[i]->clearDataOk();
    }
    m_progFrame.clearDataOk();

    Display* display = Display::getInstance();
    display->displayImage(playCache->fbp_prog, get_panel_width(), get_panel_height());
}

void ProgAttr::overlaysWindow(char* des, char* winCache, PosSize posSize, u8 overlapType)
{
    u32 x, y, winWidth, winHeight, xPos, yPos;
    u32 scrWidth = get_panel_width();

    xPos = posSize.x;
    yPos = posSize.y;
    winWidth = posSize.w;
    winHeight = posSize.h;

    fb_t (*fbpProg)[scrWidth] = (fb_t (*)[scrWidth])des;
    fb_t (*swapCache)[winWidth] = (fb_t (*)[winWidth])winCache;

    //debug("winWidth:%d winHeight:%d xPos:%d yPos:%d\n", winWidth, winHeight, xPos, yPos);
    switch (overlapType) {
        case Win_Overlap_Direct: 	// 0 直接覆盖
            for (y=0; y<winHeight; y++) {
                memcpy(fbpProg[y+yPos]+xPos, swapCache[y], winWidth*BYTE_PER_PIXEL);
            }
            break;
        default:
            print_error("unknow overlap_type");
        case Win_Overlap_Inset:	// 1 透明覆盖 嵌入叠加(前景色不为0时，保留前景色，为0保留背景色)
            for (y=0; y<winHeight; y++) {
                for (x=0; x<winWidth; x++) {
                    if (swapCache[y][x] && swapCache[y][x]!=0xfefefefe) {
                        fbpProg[yPos+y][x+xPos] = swapCache[y][x];
                    }
                    /*if ( swapCache[y][x] ) {
                        fbpProg[yPos+y][x+xPos] =
                            (fbpProg[yPos+y][x+xPos] & 0x7f7f7f) | (swapCache[y][x] & 0x808080);
                    }*/
                }
            }
            break;
        case Win_Overlap_Or:		//  2 兼容叠加(按位或)
            for (y=0; y<winHeight; y++) {
                for (x=0; x<winWidth; x++) {
                    fbpProg[yPos+y][x+xPos] |= swapCache[y][x];
                }
            }
            break;
        case Win_Overlap_Xor:		// 3 异或叠加(按位异或)
            for (y=0; y<winHeight; y++) {
                for (x=0; x<winWidth; x++) {
                    fbpProg[yPos+y][x+xPos] ^= swapCache[y][x];
                }
            }
            break;

        case Win_Overlap_NonInset:	// 4 取反嵌入叠加(前景色不为0时，该点的值取反，前景色为0保留背景色)
            for (y=0; y<winHeight; y++) {
                for (x=0; x<winWidth; x++) {
                    if ( swapCache[y][x] ) {
                        //fbpProg[yPos+y][x+xPos] = ~swapCache[y][x];  //cancled
                        fbpProg[yPos+y][x+xPos] = ~fbpProg[yPos+y][x+xPos];
                    }
                }
            }
            break;
        case Win_Overlap_Sel:		// 5 取模叠加(前景色不为0时，保留背景色，否则为0)
            for (y=0; y<winHeight; y++) {
                for (x=0; x<winWidth; x++) {
                    if ( !swapCache[y][x] ) {
                        fbpProg[yPos+y][x+xPos] = 0;
                    }
                }
            }
            break;
        case Win_Overlap_And:		// 6 相与叠加(按位与)
            for (y=0; y<winHeight; y++) {
                for (x=0; x<winWidth; x++) {
                    fbpProg[yPos+y][x+xPos] &= swapCache[y][x];
                }
            }
            break;
    }

    return;
}

void ProgAttr::loadAttr(QDomElement prog_node)
{
    m_progFrame.loadAttr(prog_node);
    m_bgMusic.loadAttr(prog_node);
    m_bgScr.loadAttr(prog_node);

    while (!prog_node.isNull()) {
        if ("playtype" == prog_node.tagName()) {
            m_playType = prog_node.text().toInt();
        } else if ("playtime" == prog_node.tagName()) {
            m_playTime = prog_node.text().toInt();

        } else if ("startdate" == prog_node.tagName()) {
            m_startDate = prog_node.text().toUInt(0, 16);
            //myDebug("start_date:%x\n", start_date);

        } else if ("enddate" == prog_node.tagName()) {
            m_endDate = prog_node.text().toUInt(0, 16);

        } else if ("starttime" == prog_node.tagName()) {
            m_startTime = prog_node.text().toUInt(0, 16);

        } else if ("endtime" == prog_node.tagName()) {
            m_endTime = prog_node.text().toUInt(0, 16);

        } else if ("week" == prog_node.tagName()) {
            m_playAsWeek = prog_node.text().toInt();

        } else if ("window" == prog_node.tagName()) {
            //WindowAttr win_attr(prog_node.firstChildElement());
            WindowAttr* win = new WindowAttr(prog_node.firstChildElement());
            m_win.append(win);
        }

        prog_node = prog_node.nextSiblingElement();
    }

    /*节目边框显示目标区域为整个屏幕大小*/
    m_progFrame.setWinSize(get_panel_width(), get_panel_height());

    /*节目背景默认属性设置*/
    m_bgScr.setWinSize(get_panel_width(), get_panel_height());

    if (m_progFrame.enable()) {
        m_bgScr.setOffset(m_progFrame.imgWidth(), m_progFrame.imgHeight());
    } else {
        m_bgScr.setOffset(0, 0);
    }
}

void ProgAttr::stop()
{
    m_stopFlag = false;
    m_playFlag = false;

    while (!m_stopFlag) {
        QThread::msleep(1);
    }
}

void ProgAttr::releaseProg()
{
    for (int i=0; i<m_win.size(); i++) {
        m_win[i]->releaseProg();
    }
    m_win.clear();
}

ProgList::ProgList()
{
    m_curProgIndex = 0;
    m_playFlag = true;
    loadProgList();
}

bool ProgList::loadProgList()
{
    char path[128] = {0};

    snprintf(path, sizeof(path), "%s/playlist.xml", PROG_ROOT);

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        print_error("open file:\"%s\" error", path);
        return false;
    }

    int size = file.size() + 1;
    char* buf = new char[size];
    memset(buf, 0, size);

    size = file.read(buf, size);

    bool ret = true;
    QDomDocument doc("playList");
    QDomElement root;
    QDomElement child;

    QByteArray bt(buf, size);
   if (!doc.setContent(bt)) {
       print_error("doc.setContent() error");
       ret = false;
       goto end_load_prog_list;
   }

   root = doc.documentElement();

   //myDebug("root node:%s\n", root.tagName().toLatin1().data());
    if ("playlist" != root.tagName()) {
       print_error("invalid node");
       ret = false;
       goto end_load_prog_list;
    }

    child = root.firstChildElement();
    while (!child.isNull()) {
        if ("program" == child.tagName()) {
            ProgAttr* prog_attr = new ProgAttr(child.firstChildElement());
            m_progList.append(prog_attr);
        }

        child = child.nextSiblingElement();
    }

 end_load_prog_list:
    file.close();
    delete []buf;

    return ret;
}

void ProgList::releaseProgList()
{
    for (int i=0; i<m_progList.size(); i++) {
        m_progList[i]->releaseProg();
    }
    m_progList.clear();
}


bool ProgList::progIsEmpty()
{
    return m_progList.isEmpty();
}

ProgAttr *ProgList::getNextProg()
{
    if (progIsEmpty()) {
        return 0;
    }

    if (m_curProgIndex == m_progList.size()) {
        m_curProgIndex = 0;
    } else {
        m_curProgIndex ++;
    }

    return m_progList[m_curProgIndex];
}

bool ProgList::play()
{
    /*for (int i=0; i<m_progList.size(); i++) {
        m_progList[i].play();
    }*/
    m_playFlag = true;
    m_curProgIndex = 0;


    while (m_playFlag) {
        m_progList[m_curProgIndex]->play();

        m_curProgIndex += 1;
        if (m_curProgIndex >= m_progList.size()) {
            m_curProgIndex = 0;
        }
    }

    return true;
}

void ProgList::stop()
{
    m_playFlag = false;
    m_progList[m_curProgIndex]->stop();
}

