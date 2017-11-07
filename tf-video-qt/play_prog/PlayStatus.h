#ifndef PLAYSTATUS_H
#define PLAYSTATUS_H

/*位置 大小信息*/
struct PosSize {
    u32 x;			// 窗口内容起始x坐标
    u32 y;			// 窗口内容起始y坐标
    u32 w;			// 窗口总大小，包括边框
    u32 h;

    u32 xOffset;	// 媒体相对于这个窗口偏移位置
    u32 yOffset;	//
    u32 imgWidth;   // 不包括边框的大小
    u32 imgHeight;
};

/*节目播放状态结构体*/
struct PlayStatus {
    u8 status;					// 播放状态
    u8 effect_dir;				// 当前动画的播放方向（0=进入 1=退出）
    bool playOver;				// 当前窗口播放结束标志位
    bool dataOk;				// 数据OK
    int flag;					// 标志位
    u8 mark[256];
    u32 imgNum;                 // 当前播放图片编号
    u32 movCnt;                 // 当前播放图片移动的次数累计
    u32 totalMovCnt;			// 当前动画需要操作的总次数
    u64 lastTm;                 // 上次操作时间
    u64 curTm;					// 当前时间
    int curNodeIndex;           // 当前播放对象编号
    char* imgCache;             // 当前对象图片缓冲区
    char* swapCache;			// 交换区缓冲区
    char *tmpCache;             // 临时缓冲区
    u32 swapCacheSize;          // 交换区缓冲区大小
};


#endif // PLAYSTATUS_H
