#ifndef CMD_H
#define CMD_H

#include "Types.h"

class Controller;

/*命令定义*/
enum cmd_e {
    CMD_READ_VERSION		= 0x12,		// 读取型号和版本
    CMD_WRITE_FLASH			= 0x13,		// 写Flash
    CMD_START_FLASH			= 0x14,		// 准备写Flash(检测点数，同时黑屏)
    CMD_FINISH_FLASH 		= 0x15,		// 写完Flash
    CMD_VIDEO_UPDATE		= 0x17,    	// 视屏卡升级
    CMD_UPDATE_PARAM 		= 0x20,		// 屏参设置 (含亮度等全部参数)
    CMD_READ_PARAM 			= 0x21,		// 读取屏参
    CMD_RESTART 			= 0x22,		// 重启控制卡
    CMD_RESET 				= 0x23,		// 参数复位
    CMD_CLEAR 				= 0x27,		// 清屏
    CMD_START_UPDATE 		= 0xCA,		// 开始刷固件
    CMD_FINISH_UPDATE 		= 0xCB,		// 完成刷固件
    CMD_NET_DISCOVER		= 0x40,		// 智能网络发现
    CMD_SMART_SET_PARAM 	= 0x41,		// 任意描点(智能设置屏参)
    CMD_START_BOOTLOGO		= 0x42,		// 开始发送开机Logo
    CMD_FINISH_BOOTLOGO		= 0x43,		// 结束发送开机Logo
    CMD_DEFAULT_BOOTLOGO	= 0x44,		// 恢复默认开机Logo
    CMD_ASSIST_TOOLS		= 0x45,		// 视频卡助手指令

    CMD_PARAM_LOCK			= 0x2C,		// 锁定或解锁屏参

    CMD_START_HARDWARE_SET	= 0x50,		// 进入硬件设置
    CMD_FINISH_HARDWARE_SET	= 0x51,		// 退出硬件设置
    CMD_SAVE_HARDWARE_SET	= 0x52,		// 保存硬件设置
    CMD_SET_FPGA_PARAM		= 0x53,		// 设置FPGA参数
    CMD_READ_FPGA_PARAM		= 0x54,		// 读取FPGA参数
    CMD_DETECT_RECV_CARD	= 0x55,		// 探测接收卡

    CMD_OPT_VIDEO_LIST		= 0x56,		// 对视频列表操作
    CMD_PARAM_BACKUP_UNDO	= 0x57,		// 屏参备份还原
};

/*错误代码*/
enum error_code_e {
    ERR_SUCCESS 			= 0,	// 指令执行成功
    ERR_CHECKSUM			= 1,	// 校验和错误
    ERR_INVALID_COMMAND		= 2,	// 命令无法识别
    ERR_UNRESOLVED_COMMAND	= 3,	// 尚未实现或不支持的命令
    ERR_INVALID_PARAMETER	= 4,	// 参数错误
    ERR_BAD_BODY			= 5,	// 指令长度不够或内容不符合基本规则
    ERR_COMMAND_FAILD		= 6,	// 指令执行失败，比如写入spi后再读出校验错误
    ERR_TOO_WIDTH			= 7,	// 设置屏参时，宽度太宽
    ERR_TOO_HEIGHT			= 8,	// 设置屏参时，高度太高
    ERR_SIZETOO_LARGE		= 9,	// 设置屏参时，总点数超限
    ERR_LOW_POWER			= 12,	// 电压过低
    ERR_CONTENT_TOO_LEN		= 15,	// 发信息时，节目内容过多超过存储量
    ERR_PROG_TOO_LEN		= 16,	// 发信息时，节目数量过多
    ERR_LOCKED              = 17,   // 屏参被锁定，设置失败
    ERR_VERSION				= 18,	// 固件升级版不符合最低要求
    ERR_TCP_BUSY			= 19,	// TCP连接忙(TCP正在传输数据,无法再接收数据)
    ERR_LOG_FILE			= 20,	// 读取log文件失败

    ERR_NO_USB				= 101,	// 没有插入U盘
    ERR_NO_PROG				= 102,  // 没有节目
    ERR_READ_PROG			= 103, 	// 读取节目错误
    ERR_WRITE_USB			= 104,  // 写USB错误
};

/*数据、命令包头*/
struct CmdHead 	// 8bytes
{
    u16 len;			// 整个包长
    u8 startFlag;		// 起始标志
    u8 code;			// 指令代码
    s8 param;			// 参数
    u8 reserved;		// 高4位表示数据包的总数，低四位表示当前是第几个包
    u16 panelId;		// 屏ID
};

/*数据、命令包尾*/
struct CmdTail		// 4bytes
{
    u8 reserved;		// 保留
    u8 endFlag;         // 结束标志
    u16 sum;			// 校验和
};


class Cmd {
private:
    Controller* controller;

private:
    void decode(char* data, int len);
    u16 get_sum(const char *buf, int len);

    void pc_read_version(CmdHead, CmdTail);           // 读取型号和版本
    void pc_update_param(CmdHead, CmdTail, char*);    // 屏参设置 (含亮度等全部参数)
    void pc_read_param(CmdHead, CmdTail);             // 获取屏参
    void restart_device(CmdHead, CmdTail);            // 重启控制卡
    void reset_param(CmdHead, CmdTail);               // 参数复位
    void clear_cmd(CmdHead, CmdTail);                 // 清屏
    void cmd_smart_ip(CmdHead, CmdTail, char*);       // 智能IP
    void smart_set_param(CmdHead, CmdTail, char*);    // 任意描点
    void cmd_start_flash(CmdHead, CmdTail, char*);    // 准备写Flash(开始发节目)
    void cmd_finish_flash(CmdHead, CmdTail);          // 写完Flash(节目发送完毕)
    void cmd_start_update(CmdHead, CmdTail, char*);   // 开始刷固件
    void cmd_finish_update(CmdHead, CmdTail);         // 完成刷固件
    void cmd_start_bootlogo(CmdHead, CmdTail, char*); // 0x42 开始发送开机Logo
    void cmd_finish_bootlogo(CmdHead, CmdTail);       // 0x43 结束发送开机Logo
    void cmd_default_bootlogo(CmdHead, CmdTail);      // 0x44 恢复默认开机Logo
    void cmd_assist_tools(CmdHead, CmdTail);          // 0x45 视频卡助手指令
    void cmd_param_lock(CmdHead, CmdTail, char*);     // 0x2c 锁定或解锁屏参

public:
    explicit Cmd(Controller* _controller);
    virtual ~Cmd();

    bool decode_cmd(char *data, int size);
    bool reply_date(CmdHead, CmdTail, char* buf=0, u32 buf_len=0);
    bool udp_send_data(char* buf, u32 size);
    bool tcp_send_data(char* buf, u32 size);
};

#endif // CMD_H
