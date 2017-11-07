#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Types.h"
#include "PlayStatus.h"
#include <QDomDocument>
#include <QTextStream>


/*节目背景动画属性*/
class BgScreen {
private:
    u8 m_enable;            // 背景动画使能 0=无, 1=使能
    u8 m_speed;             // 背景动画速度
    u8 m_status;            // 播放状态
    u8 m_dataOk;            // 数据OK
    u32 m_delay;            // 播放完一组背景动画后延时时间
    u32 m_imgCnt;           // 背景动画图片数量
    u32 m_imgNum;           // 当前播放图片编号
    u64 m_lastTm;           // 上次操作时间
    char path[128];         // 背景自定义动画路径
    PosSize m_posSize;      // 背景位置、大小信息

public:
    BgScreen();
    ~BgScreen();

    bool loadAttr(QDomElement attr);
    void setWinSize(u32 w, u32 h);
    void setOffset(u32 x, u32 y);

    bool play();
    bool dataOk();
    void clearDataOk();
};

/*节目背景音乐*/
class BgMusic {
private:
    u8 m_enable;			// 背景音乐使能
    u8 m_status;			// 背景音乐播放状态  0=未播放 1=正在播放
    u8 m_volume;			// 音量（适用于中兴平台2016.12.12）
    char path[128];			// 背景音乐,名字固定, 格式不固定
    //pid_t chld_pid;					// 子进程PID号
    //pthread_t pth_tid;				// 线程ID
    char *mplayer_argv[8];			// mplayer参数列表

public:
    BgMusic();
    ~BgMusic();

    bool loadAttr(QDomElement attr);
    bool play();
};


#endif // BACKGROUND_H
