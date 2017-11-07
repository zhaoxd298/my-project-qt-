#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QDomDocument>
#include <QTextStream>
#include <QGraphicsItem>

#include "Types.h"
#include "Frame.h"
#include "BackGround.h"
#include "PlayStatus.h"

#include <QList>

//#define WIN_NUM 128			/*一个节目最多有128个分区*/

#define PATH_LEN    128
#define PROG_ROOT   "../LED595/"


enum page_type_e{
    PROG_TEXT 			= 1,	// 文本
    PROG_PIC			= 2,	// 图片
    PROG_DAZZLE_TEXT 	= 3,	// 炫彩字
    PROG_TABLE 			= 4,	// 表格
    PROG_VIDEO			= 5,	// 视屏
    PROG_WATCH_PLATE 	= 10,	// 表盘
    PROG_TIME 			= 11,	// 时间
    PROG_TEM_HUM 		= 12,	// 温湿度
    PROG_CNT_DOWN 		= 13, 	// 倒计时
    PROG_STOPWATCH		= 50,	// 特殊区(秒表)
    PROG_COUNT			= 51, 	// 特殊区(计数)
    PROG_CUSTOM			= 52,	// 特殊区(自定义)
    PROG_PHOTO			= 53,	// 特殊区(图片区)
};

/*分区叠加方式*/
enum Win_Overlap_Type {
    Win_Overlap_Direct 		= 0,	// 直接覆盖
    Win_Overlap_Inset		= 1,	// 嵌入叠加(前景色不为0时，保留前景色，为0保留背景色)
    Win_Overlap_Or			= 2,	// 兼容叠加(按位或)
    Win_Overlap_Xor			= 3,	// 异或叠加(按位异或)
    Win_Overlap_NonInset	= 4,	// 取反嵌入叠加(前景色不为0时，该点的值为0，前景色为0保留背景色)
    Win_Overlap_Sel			= 5,	// 取模叠加(前景色不为0时，保留背景色，否则为0)
    Win_Overlap_And 		= 6,	// 相与叠加(按位与)
};

/*分区背景属性*/
struct PartBgAttr {
    u8 enable;          // 分区背景使能
    u8 action;			// 分区背景动画类型
    u16 imgCnt;         // 分区背景图片数量
    u32 delay;			// 分区背景图片两帧之间的停留时间
    char path[PATH_LEN];// 分区背景图片路径
};

/*文字描边属性*/
struct StrokeAttr {
    bool leftEnable;   	// 左边描边使能
    bool rightEnable;	// 右边描边使能
    bool upEnable;     	// 上边描边使能
    bool downEnable;   	// 下边描边使能
    u32 leftColor;     	// 左边描边颜色
    u32 rightColor;     // 右边描边颜色
    u32 upColor;       	// 上边描边颜色
    u32 downColor;     	// 下边描边颜色
    u32 strokeWidth;	// 描边宽度
};

/*--------------------------------------------------------------------------------------*/

/*
* 通用属性，即每个播放对象都有的属性
* 文本[type:1]、图片[type:2]、表格[type:4]、炫彩字[type:3],只有通用属性
* 表盘[type:10]、时间[type:11]、温湿度[type:12]、倒计时[13]，除了有通用属性外，还有自己的属性
*/
class ComAttr
{
protected:
    u8 m_winId;                 // 当前对象所在窗口ID
    u8 m_animat;				// 动画类型
    u8 m_actualAnimat;          // 动画类型(当m_animat为PA_Random时，存放随机产生的动画类型)
    u8 m_speed;                 // 动画速度
    u32 m_delay;				// 两帧之间的停留时间
    u8 m_exitAction;			// 退出动画类型
    u8 m_actualExitAction;      // 实际的退出动画类型
    u8 m_exitSpeed;             // 退出动画速度
    u8 m_overlapType;           // 分区覆盖方式
    u8 m_step;                  // 动画步长
    u16 m_imgCnt;               // 图片数量
    page_type_e m_pageType;     // 当前page编号
    char m_path[PATH_LEN];      // 图片路径(不包含文件名)
    PartBgAttr m_partBgAttr;	// 分区背景属性
    StrokeAttr m_strokeAttr;	// 描边属性

public:
    ComAttr(QDomElement pageNode, page_type_e pageType);
    virtual ~ComAttr();

    virtual bool play(PlayStatus& playStatus, PlayStatus& bgStatus, const PosSize& posSize);

};

/*--------------------------------------------------------------------------------------*/

/*视频属性 [type:5]*/
class VideoAttr : public ComAttr {
protected:
    u8 m_soundEnable;		// 声音使能
    u8 m_volume;			// 音量（适用于中兴平台2016.12.12）

public:
    VideoAttr(QDomElement pageNode, page_type_e pageType);
    virtual ~VideoAttr();

};

/*表盘属性[type:10]*/
class WatchPlateAttr : public ComAttr {
protected:
    u8 m_space;				// 被占用空间大小(像素点)
    u32 m_hourSize;			// 时针大小
    u32 m_minSize;			// 分针大小
    u32 m_secSize;			// 秒针大小
    u32 m_hourColor;			// 时针颜色
    u32 m_minColor;			// 分针颜色
    u32 m_secColor;			// 秒针颜色
    s32 m_addMin;             // 时差，分钟数

public:
    WatchPlateAttr(QDomElement pageNode, page_type_e pageType);
    virtual ~WatchPlateAttr();

};

/*--------------------------------------------------------------------------------------*/

/*时间属性[type:11]*/
class TimeAttr : public ComAttr {
protected:
    u8 m_singleLine;		// 是否单行显示
    u32 m_dayEnable;        // 日期使能
    u32 m_weekEnable;		// 星期使能
    u32 m_timeEnable;		// 时间使能
    //u8 font_type;			// 字库类型, 0:系统本身字库
    u8 m_fontSize;			// 字体大小
    u32 m_dayFormt;			// 日期格式
    u32 m_weekFormt;		// 星期格式
    u32 m_timeFormt;		// 时间格式
    u32 m_dayColor;			// 日期颜色
    u32 m_weekColor;		// 星期颜色
    u32 m_timeColor;		// 时间颜色
    u32 m_dayXPpos;			// 日期坐标
    u32 m_dayYPos;
    u32 m_weekXPos;			// 星期坐标
    u32 m_weekYPos;
    u32 m_timeXPos;			// 时间坐标
    u32 m_timeYPos;
    s32 m_addMin;			// 时差，分钟数
public:
    TimeAttr(QDomElement pageNode, page_type_e pageType);
    virtual ~TimeAttr();

};

/*--------------------------------------------------------------------------------------*/

/*温湿度属性[type:12]*/
class TemHumAttr : public ComAttr {
protected:
    u8 m_numSize;			// 温度、湿度数字大小
    u8 m_temUnit;			// 温度单位，0=摄氏度,1=华氏度
    u8 m_plusSymbolEnable;	// 是否有'+'号
    u8 m_sensor;			// 传感器类型
    u8 m_temEnable;			// 温度使能
    u8 m_humEnable;			// 湿度使能
    u32 m_temColor;			// 温度颜色
    u32 m_temXPos;			// 温度坐标
    u32 m_temYPos;
    double m_temCorrect;	// 温度校正值
    u32 m_humColor;			// 湿度颜色
    u32 m_humXPos;			// 湿度坐标
    u32 m_humYPos;
    int m_humCorrect;		// 湿度校正值
    u32 m_zeroAhead;		// 当只有个位数时是否前面加0显示

public:
    TemHumAttr(QDomElement pageNode, page_type_e pageType);
    virtual ~TemHumAttr();

};

/*--------------------------------------------------------------------------------------*/

/*倒计时属性[type:13]*/
class CntDownAttr : public ComAttr {
protected:
    u8 m_cntDownType;		// 倒计时类型
    u8 m_cntDownFmt;		// 倒计时格式
    u32 m_dayEnable;         // 天使能
    u32 m_hourEnable;		// 时使能
    u32 m_minuteEnable;		// 分使能
    u32 m_secondEnable;		// 秒使能
    //u8 font_type;			// 字库类型, 0:系统本身字库
    u8 m_fontSize;			// 字体大小
    u32 m_fontYPos;			// y坐标，显示居中，故x坐标需要动态计算
    u32 m_targetTime;		// 目标时间,4字节，16进制数
    u32 m_dayTextColor;     // 天的文字颜色
    u32 m_dayNumColor;		// 天的数字颜色
    u32 m_hourTextColor;	// 时的文字颜色
    u32 m_hourNumColor;		// 时的数字颜色
    u32 m_minTextColor;		// 分的文字颜色
    u32 m_minNumColor;		// 分的数字颜色
    u32 m_secTextColor;		// 秒的文字颜色
    u32 m_secNumColor;		// 秒的数字颜色

public:
    CntDownAttr(QDomElement pageNode, page_type_e pageType);
    virtual ~CntDownAttr();

};

/*--------------------------------------------------------------------------------------*/

/*特殊区(秒表)属性[type:50]*/
class SpecialStopwatchAttr : public ComAttr {
protected:
    u8 m_index;				// 序号
    u8 m_autoEnable;		// 自动开始使能
    u8 m_tmFormat;			// 时间格式(0=HH:MM:SS 1=MM:SS 2=SS)
    u8 m_cntType;			// 计时类型, 0=倒计时, 1=正计时
    u32 m_defaultValue;		// 初始值时间值
    u32 m_fontColor;		// 字体颜色
    u8 m_fontSize;			// 字体大小
    u8 m_msEnable;			// 1/100秒显示使能
    u8 m_msFontSize;		// 1/100秒字体大小
    u32 m_msFontColor;		// 1/100秒字体颜色
    u32 m_fontYPos;			// y坐标，显示居中，故x坐标需要动态计算

public:
    SpecialStopwatchAttr(QDomElement pageNode, page_type_e pageType);
    virtual ~SpecialStopwatchAttr();

};

/*--------------------------------------------------------------------------------------*/

/*特殊区(计数)属性[type:51]*/
class SpecialCountAttr : public ComAttr {
protected:
    u8 m_index;				// 序号
    u8 m_negativeEnable;	// 是否允许负数
    u8 m_numWidth;			// 数值有效位数宽度
    u8 m_fontSize;			// 字体大小
    u32 m_fontColor;		// 字体颜色
    int m_defaultValue;		// 初始值值
    u32 m_fontYPos;			// y坐标，显示居中，故x坐标需要动态计算

public:
    SpecialCountAttr(QDomElement pageNode, page_type_e pageType);
    virtual ~SpecialCountAttr();

};

/*--------------------------------------------------------------------------------------*/

/*
*特殊区(自定义)属性[type:52]
* 自定义区[type:52]、图片区[type:53]
*/
class SpecialCustomAttr : public ComAttr {
protected:
    u8 m_index;				// 序号

public:
    SpecialCustomAttr(QDomElement pageNode, page_type_e pageType);
    virtual ~SpecialCustomAttr();

};


/*--------------------------------------------------------------------------------------*/

/*窗口属性*/
class WindowAttr : public QGraphicsItem
{
private:
    u8 m_Id;                     // 标识当前窗口编号
    u8 m_overlapType;            // 分区覆盖方式
    u8 m_lastPageType;           // 上一个page类型
    int m_curPageNum;            // 当前播放page编号
    PosSize m_posSize;           // 窗口位置信息
    Frame m_winFrame;            // 窗口边框
    PlayStatus m_playStatus;     // 当前对象的播放状态
    PlayStatus m_bgStatus;       // 当前对象的背景播放状态(目前只用于文字背景)
    QList<ComAttr*> m_page;      // 存放page列表容器
private:
    ComAttr* getNewPage(QDomElement pageNode, page_type_e pageType);

public:
    WindowAttr();
    WindowAttr(QDomElement winNode);
    ~WindowAttr();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*);

    bool initResource();
    void releaseResource();

    bool play();
    bool dataOk();
    void clearDataOk();
    bool playOver();
    char* winCache();
    PosSize posSize();
    u8 overlapType();

    void releaseProg();
};

/*--------------------------------------------------------------------------------------*/

/*节目属性*/
class ProgAttr : public QObject
{
    Q_OBJECT
private:
    u16 m_id;					// 节目ID
    u8 m_playType;				// 播放方式
    u8 m_playAsWeek;			// 星期播放<使能(0:启用该功能) 6 5 4 3 2 1 日>
    u32 m_playTime;				// 当播放方式为(定时播放、TF-RMT/BUS点播)时有效
    u32 m_startDate;			// 当此项最高位为0时启用该功能
    u32 m_endDate;
    u32 m_startTime;			// 当此项最高位为0时启用该功能
    u32 m_endTime;
    u64 m_startTm;				// 开始播放节目时间，用于定时播放
    Frame m_progFrame;          // 节目边框
    QList<WindowAttr*> m_win;    // 窗口属性列表(一个节目可以有多个窗口)
    BgScreen m_bgScr;           // 节目背景动画
    BgMusic m_bgMusic;          // 节目背景音乐

private:
    bool m_playFlag;
    bool m_stopFlag;
    void displayProg();
    void overlaysWindow(char* des, char* winCache, PosSize posSize, u8 overlapType);
    void loadAttr(QDomElement progNode);

public:
    ProgAttr();
    ProgAttr(QDomElement progNode);
    ~ProgAttr();

    bool initResource();
    void releaseResource();
    bool play();
    void stop();
    void releaseProg();

signals:
    void addQGraphicsItem(QGraphicsItem* item, int x, int y);
    void clearQGraphicsItem();
};

/*--------------------------------------------------------------------------------------*/

/*播放列表*/
class ProgList {
private:
    bool m_playFlag;
    int m_curProgIndex;
    QList<ProgAttr*> m_progList;

public:
    ProgList();
    bool loadProgList();
    void releaseProgList();

    bool progIsEmpty();
    ProgAttr *getNextProg();

    bool play();
    void stop();
};

#endif // PLAYLIST_H
