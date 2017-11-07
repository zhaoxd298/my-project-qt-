#include "Cmd.h"
#include "Log.h"
#include "Net.h"
#include "Controller.h"

#include <string.h>

Cmd::Cmd(Controller *_controller)
{
    controller = _controller;
}

Cmd::~Cmd()
{

}

void Cmd::decode(char* data, int len)
{
    int i;
    u8 flag = data[2];
    if(flag < 0x80)
    {
        for (i=0; i<len; i++) {
            if ((0 == i%2) && (2 != i)) {
                data[i] ^= (flag*(i+5));
            } else if ( 1 == i%2 ) {
                data[i] ^= (flag+i+1);
            }
        }
    }

    return;
}

u16 Cmd::get_sum(const char *buf, int len)
{
    u16 sum = 0;
    for (int i=0; i<len-2; i++) {
        sum += (buf[i] & 0xff);
    }

    return sum;
}

bool Cmd::decode_cmd(char *data, int size)
{
    struct CmdHead cmd_head;
    struct CmdTail cmd_tail;

    decode(data, size);     // 解密

    memcpy(&cmd_head, data, sizeof(struct CmdHead));
    if (cmd_head.len > size) {  // 数据长度有问题，返回
        print_error("invalid data length:%d", cmd_head.len);
        return true;
    }
    memcpy(&cmd_tail, data+cmd_head.len-sizeof(struct CmdTail),
        sizeof(struct CmdTail) );

    if (!cmd_head.code) {  //卡的回复数据不处理
        //debug("111\n");
        return true;
    }

 #if 0
    if (cmd_head.panelId && led_param.panel_param.id != cmd_head.panelId ) {
        // 屏ID不对，对信息不做处理
        return true;
    }
 #else
    if (138 != cmd_head.panelId ) {
        // 屏ID不对，对信息不做处理
        return true;
    }
 #endif

    myDebug("code:0x%x param:%d id:%d\n", cmd_head.code, cmd_head.param, cmd_head.panelId);

    cmd_e code = (cmd_e)cmd_head.code;
    //g_last_cmd = code;
    //g_last_id = cmd_head.panelId;

    if (cmd_tail.sum) {
        u16 sum = get_sum(data, cmd_head.len);
        if ( sum != cmd_tail.sum ) {
            cmd_head.param = ERR_CHECKSUM;
            reply_date(cmd_head, cmd_tail);
            print_error("check sum error %x %x\n", cmd_tail.sum, sum);
            return false;
        }
    } else {
        myDebug("skip check sum\n");
    }

    switch ( code ) {
        case CMD_READ_VERSION:      // 读取型号和版本
            pc_read_version(cmd_head, cmd_tail);
            break;
        case CMD_UPDATE_PARAM:      // 屏参设置 (含亮度等全部参数)
            pc_update_param(cmd_head, cmd_tail,
                data+sizeof(struct CmdHead));
            break;
        case CMD_READ_PARAM:        // 获取屏参
            pc_read_param(cmd_head, cmd_tail);
            break;
        case CMD_RESTART:           // 重启控制卡
            restart_device(cmd_head, cmd_tail);
            break;
        case CMD_RESET:             // 参数复位
            reset_param(cmd_head, cmd_tail);
            break;
        case CMD_CLEAR:             // 清屏
            clear_cmd(cmd_head, cmd_tail);
            break;
        case CMD_NET_DISCOVER:
            cmd_smart_ip(cmd_head, cmd_tail, data+sizeof(struct CmdHead));
            break;
        case CMD_SMART_SET_PARAM:   // 任意描点
            smart_set_param(cmd_head, cmd_tail, data+sizeof(struct CmdHead));
            break;
        case CMD_START_FLASH:       // 准备写Flash(开始发节目)
            cmd_start_flash(cmd_head, cmd_tail, data+sizeof(struct CmdHead));
            break;
        case CMD_FINISH_FLASH:      // 写完Flash(节目发送完毕)
            cmd_finish_flash(cmd_head, cmd_tail);
            break;
        case CMD_START_UPDATE:      // 开始刷固件
            cmd_start_update(cmd_head, cmd_tail, data+sizeof(struct CmdHead));
            break;
        case CMD_FINISH_UPDATE:     // 完成刷固件
            cmd_finish_update(cmd_head, cmd_tail);
            break;
        case CMD_START_BOOTLOGO:    // 0x42 开始发送开机Logo
            cmd_start_bootlogo(cmd_head, cmd_tail, data+sizeof(struct CmdHead));
            break;
        case CMD_FINISH_BOOTLOGO:   // 0x43 结束发送开机Logo
            cmd_finish_bootlogo(cmd_head, cmd_tail);
            break;
        case CMD_DEFAULT_BOOTLOGO:  // 0x44 恢复默认开机Logo
            cmd_default_bootlogo(cmd_head, cmd_tail);
            break;
        case CMD_ASSIST_TOOLS:      // 0x45 视频卡助手指令
            cmd_assist_tools(cmd_head, cmd_tail);
            break;

        case CMD_PARAM_LOCK:              // 0x2c 锁定或解锁屏参
            cmd_param_lock(cmd_head, cmd_tail, data+sizeof(struct CmdHead));
            break;
        default :
                cmd_head.param = ERR_INVALID_COMMAND;
                reply_date(cmd_head, cmd_tail);
                print_error("unknow command:%x", code);
                break;
    }

    return true;
}


bool Cmd::reply_date(CmdHead cmd_head_s, CmdTail cmd_tail_s, char* buf, u32 buf_len)
{
    bool ret = false;
    char data[BUF_SIZE];
    struct CmdHead *cmd_head;
    struct CmdTail *cmd_tail = NULL;

    cmd_head = (struct CmdHead *)data;
    *cmd_head = cmd_head_s;
    cmd_head->code = ERR_SUCCESS;   // 回复cmd_header.code==0表示,命令已经收到
    cmd_head->startFlag = 0x0a;

    if (buf_len > (BUF_SIZE-sizeof(CmdHead)-sizeof(CmdTail))) {
        print_error("data is too large");
        buf_len = BUF_SIZE-sizeof(CmdHead)-sizeof(CmdTail);
    }

    if ((buf_len>0) && (buf!=0)) {
        memcpy(data+sizeof(CmdHead), buf, buf_len);
    }

    cmd_head->len = sizeof(CmdHead) + sizeof(CmdTail) + buf_len;
    cmd_tail = (CmdTail *)(data + cmd_head->len - sizeof(CmdTail));
    *cmd_tail = cmd_tail_s;
    cmd_tail->sum = get_sum(data, cmd_head->len);

    if (NET_LOCAL_MODE == controller->m_net->get_cur_net_mode()) {
        ret = udp_send_data(data, cmd_head->len);
    } else if (NET_REMOTE_MODE == controller->m_net->get_cur_net_mode()) {
        ret = tcp_send_data(data, cmd_head->len);
    } else {
        print_error("unknown net mode:%d", controller->m_net->get_cur_net_mode());
    }

    if (ret) {
        myDebug("reply data success! size:%d\n", cmd_head->len);
    } else {
        print_error("send data error");
    }

    return ret;
}

bool Cmd::udp_send_data(char* buf, u32 size)
{
    if (!buf || size<=0) {
        return false;
    }

    return controller->m_net->udp_reply_data(buf, size);
}

bool Cmd::tcp_send_data(char* buf, u32 size)
{
    if (!buf || size<=0) {
        return false;
    }

    return true;
}


void Cmd::pc_read_version(CmdHead cmd_head, CmdTail cmd_tail)
{
    cmd_head.param = ERR_SUCCESS;
    char buf[5];
    memcpy(buf, card_ver, sizeof(card_ver));
    memcpy(buf+sizeof(card_ver), soft_ver, sizeof(soft_ver));

    reply_date(cmd_head, cmd_tail, buf, sizeof(buf));
}

void Cmd::pc_update_param(CmdHead, CmdTail, char*)
{

}

void Cmd::pc_read_param(CmdHead, CmdTail)
{

}

void Cmd::restart_device(CmdHead, CmdTail)
{

}

void Cmd::reset_param(CmdHead, CmdTail)
{

}

void Cmd::clear_cmd(CmdHead, CmdTail)
{

}

void Cmd::cmd_smart_ip(CmdHead, CmdTail, char*)
{

}

void Cmd::smart_set_param(CmdHead, CmdTail, char*)
{

}

void Cmd::cmd_start_flash(CmdHead, CmdTail, char*)
{

}

void Cmd::cmd_finish_flash(CmdHead, CmdTail)
{

}

void Cmd::cmd_start_update(CmdHead, CmdTail, char*)
{

}

void Cmd::cmd_finish_update(CmdHead, CmdTail)
{

}

void Cmd::cmd_start_bootlogo(CmdHead, CmdTail, char*)
{

}

void Cmd::cmd_finish_bootlogo(CmdHead, CmdTail)
{

}

void Cmd::cmd_default_bootlogo(CmdHead, CmdTail)
{

}

void Cmd::cmd_assist_tools(CmdHead, CmdTail)
{

}

void Cmd::cmd_param_lock(CmdHead, CmdTail, char*)
{

}
