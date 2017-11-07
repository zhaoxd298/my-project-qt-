#include "PlayProg.h"
#include "Controller.h"
#include "Log.h"
#include<QDebug>


PlayCache* PlayCache::m_playCache = NULL;

PlayCache::PlayCache()
{
    fbp_prog = new char[CACHE_SIZE];
    if (!fbp_prog) {
        print_error("new fbp_prog error");
    }

    bg_scr = new char[CACHE_SIZE];
    if (!bg_scr) {
        print_error("new bg_scr error");
    }

    fbp_module = new char[CACHE_SIZE];
    if (!fbp_module) {
        print_error("new fbp_module error");
    }

    single_cache = new char[CACHE_SIZE];
    if (!single_cache) {
        print_error("new single_cache error");
    }

    tmp = new char[CACHE_SIZE];
    if (!tmp) {
        print_error("new tmp error");
    }
}

PlayCache* PlayCache::getInstance()
{
    if (NULL == m_playCache) {
        m_playCache = new PlayCache();
    }

    return m_playCache;
}

PlayCache::~PlayCache()
{
    delete []fbp_prog;
    delete []bg_scr;
    delete []fbp_module;
    delete []single_cache;
    delete []tmp;
}


NormProg::NormProg(PlayProg* playProg) : m_playProg(playProg)
{

}

NormProg::~NormProg()
{
    m_progList.releaseProgList();
}

bool NormProg::play()
{
    if (m_progList.progIsEmpty()) {
        selectNextProg();
    }

    m_progList.play();

    return true;
}

void NormProg::stop()
{
    m_progList.stop();
}

void NormProg::selectNextProg()
{
    m_playProg->setCurProgSlopeLineProg();
}

SlopeLineProg::SlopeLineProg(PlayProg* playProg) : m_playProg(playProg)
{

}

bool SlopeLineProg::play()
{
    u32 x, y;
    static int start_pos = 0;
    static int pos_flag = 0;
    static u64 last_tm;
    u32 scr_x_size = get_panel_width();
    u32 scr_y_size = get_panel_height();
    PlayCache* playCache = PlayCache::getInstance();
    fb_t (*fbp_prog)[scr_x_size] = (fb_t (*)[scr_x_size])playCache->fbp_prog;

    if ( remain_tm(last_tm, TEST_SPEED)>0 ) {
        return true;
    }

    memset(fbp_prog, 0, scr_x_size*scr_y_size*BYTE_PER_PIXEL);

    start_pos = pos_flag;
    for ( y=0; y<scr_y_size; y++ ) {
        for ( x=start_pos; x<scr_x_size; x+=INTERVAL ) {
            fbp_prog[y][x] = 0xffffffff;
        }
        if ( ++start_pos >= INTERVAL ) {
            start_pos = 0;
        }
    }

    if ( ++pos_flag >= INTERVAL ) {
        pos_flag = 0;
    }

    m_playProg->displayProg(playCache->fbp_prog, scr_x_size, scr_y_size);

    last_tm = get_cur_tm();

    return true;
}


void SlopeLineProg::selectNextProg()
{
    m_playProg->setCurProgHorizontalLineProg();
}


HorizontalLineProg::HorizontalLineProg(PlayProg* playProg) : m_playProg(playProg)
{}

bool HorizontalLineProg::play()
{
    static int start_pos = INTERVAL-1;
    static u64 last_tm;
    u32 scr_x_size = get_panel_width();
    u32 scr_y_size = get_panel_height();
    PlayCache* playCache = PlayCache::getInstance();
    fb_t (*fbp_prog)[scr_x_size] = (fb_t (*)[scr_x_size])playCache->fbp_prog;

    if ( remain_tm(last_tm, TEST_SPEED)>0 ) {
        return true;
    }

    memset(fbp_prog, 0, scr_x_size*scr_y_size*BYTE_PER_PIXEL);

    for(u32 y=start_pos; y<scr_y_size; y+=INTERVAL) {
        for(u32 x=0; x<scr_x_size; x++) {
            fbp_prog[y][x] = 0x00ffffff;
        }
    }

    if ( --start_pos < 0 ) {
        start_pos = INTERVAL-1;
    }

    //debug("start_pos:%d\n", start_pos);
    m_playProg->displayProg(playCache->fbp_prog, scr_x_size, scr_y_size);

    last_tm = get_cur_tm();

    return true;
}


void HorizontalLineProg::selectNextProg()
{
    m_playProg->setCurProgVertiaclLineProg();
}


VertiaclLineProg::VertiaclLineProg(PlayProg* playProg) : m_playProg(playProg)
{

}

bool VertiaclLineProg::play()
{
    static int start_pos = 0;
    static u64 last_tm;
    u32 scr_x_size = get_panel_width();
    u32 scr_y_size = get_panel_height();
    PlayCache* playCache = PlayCache::getInstance();
    fb_t (*fbp_prog)[scr_x_size] = (fb_t (*)[scr_x_size])playCache->fbp_prog;

    if ( remain_tm(last_tm, TEST_SPEED)>0 ) {
        return true;
    }

    memset(fbp_prog, 0, scr_x_size*scr_y_size*BYTE_PER_PIXEL);

    for(u32 x=start_pos; x<scr_x_size; x+=INTERVAL) {
        for(u32 y=0; y<scr_y_size; y++) {
            fbp_prog[y][x] = 0x00ffffff;
        }
    }

    if ( ++start_pos >= INTERVAL ) {
        start_pos = 0;
    }

    //debug("start_pos:%d\n", start_pos);
    m_playProg->displayProg(playCache->fbp_prog, scr_x_size, scr_y_size);

    last_tm = get_cur_tm();

    return true;
}


void VertiaclLineProg::selectNextProg()
{
    m_playProg->setCurProgDefaultProg();
}


DefaultProg::DefaultProg(PlayProg* playProg) : m_playProg(playProg)
{

}

bool DefaultProg::play()
{
    QThread::sleep(1);
    qDebug() << "play Default Prog";
    return true;
}


void DefaultProg::selectNextProg()
{
    m_playProg->setCurProgScrBrightAll();
}


ScrBrightAll::ScrBrightAll(PlayProg* playProg) : m_playProg(playProg)
{}

bool ScrBrightAll::play()
{
    u32 scr_x_size = get_panel_width();
    u32 scr_y_size = get_panel_height();
    static u64 last_tm;

    if ( remain_tm(last_tm, TEST_SPEED)>0 ) {
        return true;
    }

    PlayCache* playCache = PlayCache::getInstance();
    fb_t (*fbp_prog)[scr_x_size] =
        (fb_t (*)[scr_x_size])playCache->fbp_prog;
    for (u32 i=0; i<scr_y_size; i++) {
        //memset(fbp[i], 0xff, scr_x_size*BYTE_PER_PIXEL);
        memset(fbp_prog[i], 0xff, scr_x_size*BYTE_PER_PIXEL);
    }

    m_playProg->displayProg(playCache->fbp_prog, scr_x_size, scr_y_size);

    last_tm = get_cur_tm();

    return true;
}


void ScrBrightAll::selectNextProg()
{
    m_playProg->setCurProgScrDarkAll();
}


ScrDarkAll::ScrDarkAll(PlayProg* playProg) : m_playProg(playProg)
{}
bool ScrDarkAll::play()
{
    u32 scr_x_size = get_panel_width();
    u32 scr_y_size = get_panel_height();
    static u64 last_tm;

    if ( remain_tm(last_tm, TEST_SPEED)>0 ) {
        return true;
    }

    PlayCache* playCache = PlayCache::getInstance();
    fb_t (*fbp_prog)[scr_x_size] = (fb_t (*)[scr_x_size])playCache->fbp_prog;
    for (u32 i=0; i<scr_y_size; i++) {
        //memset(fbp[i], 0xff, scr_x_size*BYTE_PER_PIXEL);
        memset(fbp_prog[i], 0, scr_x_size*BYTE_PER_PIXEL);
    }

    m_playProg->displayProg(playCache->fbp_prog, scr_x_size, scr_y_size);

    last_tm = get_cur_tm();

    return true;
}


void ScrDarkAll::selectNextProg()
{
    m_playProg->setCurProgNormProg();
}

PlayProg::PlayProg(IDisplay *display, QObject *parent) :
    QThread(parent)
{
    m_display = display;

    m_normProg = new NormProg(this);
    m_slopeLineProg = new SlopeLineProg(this);
    m_horizontalLineProg = new HorizontalLineProg(this);
    m_vertiaclLineProg = new VertiaclLineProg(this);
    m_defaultProg = new DefaultProg(this);
    m_scrBrightAll = new ScrBrightAll(this);
    m_scrDarkAll = new ScrDarkAll(this);

    m_curProg = m_normProg;
    //m_curProg = m_slopeLineProg;

    startPlayProg();
}


PlayProg::~PlayProg()
{
    finishPlayProg();

    delete m_normProg;
    delete m_slopeLineProg;
    delete m_horizontalLineProg;
    delete m_vertiaclLineProg;
    delete m_defaultProg;
    delete m_scrBrightAll;
    delete m_scrDarkAll;
}

void PlayProg::keyPressed()
{
    // 按键防抖
    // 切换节目
    finishPlayProg();
    //m_curProg->stop();
    m_curProg->selectNextProg();
    startPlayProg();
}

void PlayProg::startPlayProg()
{
    m_runFlag = true;

    start();    // 启动播放线程 run();
}

void PlayProg::finishPlayProg()
{
    m_runFlag = false;
    m_curProg->stop();
    wait();     // 等待播放线程结束
}


void PlayProg::run()
{
    while (m_runFlag) {
        m_curProg->play();
    }
}


bool PlayProg::displayProg(const char* buf, u32 w, u32 h)
{
    if (0 == m_display) {
        return false;
    }

    m_display->displayImage(buf, w, h);

    return true;
}

void PlayProg::setCurProgNormProg()
{
    m_curProg = m_normProg;
}

void PlayProg::setCurProgSlopeLineProg()
{
    m_curProg = m_slopeLineProg;
}

void PlayProg::setCurProgHorizontalLineProg()
{
    m_curProg = m_horizontalLineProg;
}

void PlayProg::setCurProgVertiaclLineProg()
{
    m_curProg = m_vertiaclLineProg;
}

void PlayProg::setCurProgDefaultProg()
{
    m_curProg = m_defaultProg;
}

void PlayProg::setCurProgScrBrightAll()
{
    m_curProg = m_scrBrightAll;
}

void PlayProg::setCurProgScrDarkAll()
{
    m_curProg = m_scrDarkAll;
}

