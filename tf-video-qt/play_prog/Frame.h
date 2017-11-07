#ifndef FRAME_H
#define FRAME_H


#include "Types.h"
#include <QDomDocument>
#include <QTextStream>

#define RANDOM_TIME		5000

#define FRAME_UNREADY	0	// 未就绪状态，边框没有播放过,需要的数据没有准备好
#define FRAME_READY 	1	// 就绪状态，可以正常播放边框
#define FRAME_PAUSE		2	// 停顿状态，下次播放时间未到

#define FA_ACTION_NUM	6

enum EmFrameAction
{
    FA_RANDOM		=	0,	//随机
    FA_CLOCKWISE	=	1,	//顺时针
    FA_ANTI			=	2,	//逆时针
    FA_FLASH		=	3,	//亮灭闪
    FA_FLASH_REV	=	4,	//反白闪
    FA_CLOCK_RACE	= 	5,	//顺时针追逐
    FA_ANTI_RACE	=	6	//逆时针追逐
};

/*边框播放状态*/
struct FrameStatus {
    u8 status;				// 播放状态
    u8 flag;				// 标志位
    u8 stepNum;				// 边框移动次数累计
    u8 actualAction;		// 具体的边框特效
    u8 dataOk;				// 数据OK
    u64 lastTm;				// 上次操作时间
    u64 lastRandomTm;		// 上次计算随机动画时间
    u64 curTm;				// 当前时间
    u32 framePos;			// 边框位置(顺(逆)时针追逐时用)
    char *imgCache;         // 边框图片元素缓冲区
};

/*
* 节目、窗口边框属性
*/
class Frame {
private:
    bool m_enable;          // 边框使能
    u8 m_type;				// 边框类型
    u8 m_action;			// 边框特效(移动方式)
    u8 m_step;				// 步长(节目边框属性)
    u8 m_speed;				// 移动速度
    bool m_flatEnable;        // 旋转使能(节目边框属性)
    //s32 m_x;              // 边框起始x坐标
    //s32 m_y;              // 边框起始y坐标
    u32 m_imgWidth;			// 边框元素宽
    u32 m_imgHeight;        // 边框元素高
    u32 m_winWidth;			// 窗口大小
    u32 m_winHeight;
    u32 m_color;            // 边框颜色(窗口边框属性)
    FrameStatus m_status;   // 边框播放状态信息
    //pthread_t tid;		// 线程ID
    char path[128];         // 边框元素路径(包括文件名)

public:
    Frame();
    ~Frame();

    bool enable();
    u32 imgWidth();
    u32 imgHeight();

    bool loadAttr(QDomElement attr);
    void setWinSize(u32 w, u32 h);
    bool initResource();
    void releaseResource();

    bool play();
    bool dataOk();
    void clearDataOk();
};

#endif // FRAME_H
