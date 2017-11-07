#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "message/Net.h"
#include "message/Usb.h"
#include "display/Display.h"
#include "play_prog/PlayProg.h"
#include "media/Media.h"
#include "board/Board.h"

#include "TestKey.h"

#include <sys/time.h>

class Controller {
private:
    TestKey* m_testKey;
public:
    Net* m_net;
    usb_t* m_usb;
    Display* m_display;
    PlayProg* m_playProg;
    Media* m_media;
    Board* m_board;
public:
    Controller();
    ~Controller();
};

extern const char model[];
extern const u8 card_ver[2];
extern const u8 soft_ver[3];


/*
* 获取当前时间，返回u64类型，返回时间单位为ms
*/
static inline u64 get_cur_tm(void)
{
    s64 cur_tm;
    struct timeval tm_val;
    gettimeofday(&tm_val, NULL);

    cur_tm = (u64)tm_val.tv_sec*1000 + (u64)tm_val.tv_usec/1000;

    //printf("sec:%d usec:%d\n", tm_val.tv_sec, tm_val.tv_usec);
    return cur_tm;
}

/*
* 确定等待时间是否到，考虑当time_cnt溢出情况
* 返回值：大于0：等待时间未到  小于等于0：等待时间到
* @last_tm:开始等待时间(ms)
* @delay_tm：需要等待的时间(ms)
*/
static inline s64 remain_tm(u64 last_tm, u32 delay_tm)
{
    s64 tm = 0;
    u64 cur_tm  = get_cur_tm();

    tm = (s64)(last_tm + delay_tm - cur_tm);
    if (tm > delay_tm) {
        tm = 0;
    }

    return tm;
}

static inline u32 get_panel_width()
{
    return 800;
}

static inline u32 get_panel_height()
{
    return 600;
}

#endif // CONTROLLER_H
