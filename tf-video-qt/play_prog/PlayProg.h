#ifndef PLAY_PROG_H
#define PLAY_PROG_H

#include <QThread>

#include "Types.h"
#include "ProgList.h"
#include "../display/IDisplay.h"
#include "IPlayProg.h"


#define TEST_SPEED	200 	/*测试节目速度，每200ms移动一次*/
#define INTERVAL 	4		/*测试节目间距(如横线的间距等)*/

/*开关机参数*/
struct PowerParam {
    u8 mode;                // 开关机模式 1=手动，2=定时
    u8 state;               // 开机状态 1=开机，0=关机,此项只在手动模式时出现
    u32 start_min[4];       // 开机时间, 关机时间; 时间是从1970/01/01 00:00到开始或结束时间的分钟数,此项只在定时模式时出现,可有4项
    u32 end_min[4];
    u8 paramCnt;           // 自动模式下,开关机时间段个数
};

/*--------------------------------------------------------------------------------------*/

/*
* 播放缓冲区 (采用单件模式)
*/
class PlayCache {
private:
    static PlayCache* m_playCache;
    PlayCache();
    PlayCache(const PlayCache& playCache);
    PlayCache& operator = (const PlayCache& playCache);
public:
    enum {
        CACHE_SIZE = 1280*768*4,
    };
    char *fbp_prog;				// 节目缓冲区(节目数据最终缓冲区)
    char *bg_scr;				// 背景动画缓冲区
    char *fbp_module;			// 按模组重排后数据缓冲区
    char *single_cache;			// 单机版数据缓冲区
    char *tmp;					// 临时缓冲

public:
    static PlayCache* getInstance();
    ~PlayCache();
};

/*--------------------------------------------------------------------------------------*/

class PlayProg;


/*节目类型接口*/
class IProg {
public:
    virtual ~IProg() {}
    virtual bool play() = 0;
    virtual void stop() {}
    virtual void selectNextProg() = 0;
};

/*常规节目*/
class NormProg : public IProg {
private:
    PlayProg* m_playProg;
    ProgList m_progList;
public:
    NormProg(PlayProg* playProg);
    ~NormProg();
    bool play();
    void stop();
    void selectNextProg();
};

/*斜线节目*/
class SlopeLineProg : public IProg {
private:
    PlayProg* m_playProg;
public:
    SlopeLineProg(PlayProg* playProg);
    bool play();
    void selectNextProg();
};

/*横线节目*/
class HorizontalLineProg : public IProg {
private:
    PlayProg* m_playProg;
public:
    HorizontalLineProg(PlayProg* playProg);
    bool play();
    void selectNextProg();
};

/*竖线节目*/
class VertiaclLineProg : public IProg {
private:
    PlayProg* m_playProg;
public:
    VertiaclLineProg(PlayProg* playProg);
    bool play();
    void selectNextProg();
};

/*默认节目*/
class DefaultProg : public IProg {
private:
    PlayProg* m_playProg;
public:
    DefaultProg(PlayProg* playProg);
    bool play();
    void selectNextProg();
};


/*屏幕全亮*/
class ScrBrightAll : public IProg {
private:
    PlayProg* m_playProg;
public:
    ScrBrightAll(PlayProg* playProg);
    bool play();
    void selectNextProg();
};

/*屏幕全灭*/
class ScrDarkAll : public IProg {
private:
    PlayProg* m_playProg;
public:
    ScrDarkAll(PlayProg* playProg);
    bool play();
    void selectNextProg();
};



/*
* 播放节目类  (状态模式)
*/
class PlayProg : public QThread, public IPlayProg {
    Q_OBJECT
private:
    bool m_runFlag;
    IDisplay* m_display;
    PowerParam m_powerParam;

    IProg* m_normProg;
    IProg* m_slopeLineProg;
    IProg* m_horizontalLineProg;
    IProg* m_vertiaclLineProg;
    IProg* m_defaultProg;
    IProg* m_scrBrightAll;
    IProg* m_scrDarkAll;

    IProg* m_curProg;

private:
    void run(); // 线程入口函数
public:
    /*节目播放状态*/
    enum {
        PLAY_UNREADY	= 0,	// 未就绪状态，数据还没有准备好
        PLAY_READY		= 1,	// 就绪状态，数据准备好了，播放视频时表示正在播放视频
        PLAY_PAUSE		= 2,	// 停顿状态，即还未到下一次移动时间
        PLAY_DELAY		= 3,	// 停留状态，即一副图片移动完成后停留时间
        PLAY_DONE		= 4,	// 完成状态，即本组图片全部播放完成
    };

    /*播放节目时相关参数*/
    enum {
        MUSIC_UNPLAY    = 0,
        MUSIC_PLAYING   = 1,
        OFFSET_SPEED    = 2,
        BASE_SPEED      = 15,
    };


public:
    explicit PlayProg(IDisplay *display, QObject *parent = 0);
    virtual ~PlayProg();

    void keyPressed();
    void startPlayProg();
    void finishPlayProg();

    bool inline displayProg(const char* buf, u32 w, u32 h);

    void setCurProgNormProg();
    void setCurProgSlopeLineProg();
    void setCurProgHorizontalLineProg();
    void setCurProgVertiaclLineProg();
    void setCurProgDefaultProg();
    void setCurProgScrBrightAll();
    void setCurProgScrDarkAll();

};

#endif // PLAY_PROG_H
