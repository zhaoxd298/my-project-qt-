#ifndef NET_H
#define NET_H

#include <QWidget>

#include <QDomDocument>
#include <QTextStream>

#include <QUdpSocket>
#include <QHostAddress>

#include "Types.h"
#include "Cmd.h"

#define IP_SIZE             32
#define MAC_SIZE            20

#define BUF_SIZE 2048

enum net_mode_e {
    NET_LOCAL_MODE  = 0,    // 局域网模式
    NET_REMOTE_MODE = 1     // 集群网络模式
};

/*局域网参数*/
struct LanParm {
    u32 localPort;             // 本地端口
    char localIp[IP_SIZE];     // 本地IP
    char subMask[IP_SIZE];     // 子网掩码
    char gateway[IP_SIZE];      // 网关
    u8 mdfMacEnable;          // 是否修改mac 0:不修改 1:修改
    char localMac[MAC_SIZE];   // mac地址
    u8 dhcpToPc;              // 是否为电脑分配IP ,1=是
    u8 autoIp;                 // 是否通过DHCP获取IP地址,1=是
    u8 workType;               // 工作模式1=远程网络,0=局域网
};

/*互联网参数*/
struct WanParam {
    char devName[24];          // 设备名
    u32 remotePort;            // 远程端口
    char remoteAddr[IP_SIZE];  // 远程地址
    char dns[IP_SIZE];          // DNS地址
    char apn[8];                // 接入点：CMNET或UNINET
    u8 protocol;                // 0=TCP;1=UDP
    u32 heartInterval;         // 心跳包间隔,单位为秒
    char imei[40];        // 设备序列号, 此字段为只读字段,不可更改
};

class Controller;

class Net : public QWidget {
    Q_OBJECT
public:
    explicit Net(Controller* _controller);
    virtual ~Net();

    bool load_param();
    bool parse_param_file(const char* file_name);
    bool parse_param_buf(char* buf, int size);
    bool sava_param();
    bool save_param_to_file(const char* file_name);
    bool save_param_to_buf(char** buf, int *size);

    bool add_lan_param_to_xml(QDomElement &root);
    bool add_wan_param_to_xml(QDomElement &root);

    net_mode_e get_cur_net_mode();
    bool udp_reply_data(const char* buf, u32 size);
    bool tcp_reply_data(const char* buf, u32 size);

private:
    bool parse_lan_param(QDomElement lan_node);
    bool parse_wan_param(QDomElement wan_node);

    bool init_net();        // 初始化网络
    void release_net();     // 释放网络资源
    bool start_local_net_mode();
    bool finish_local_net_mode();
    bool start_remote_net_mode();
    bool finish_remote_net_mode();

private:
    Controller* controller;

    LanParm m_lanParam;
    WanParam m_wanParam;

    QUdpSocket *udp_socket;
    QHostAddress sender_addr;
    quint16 sender_port;

    net_mode_e net_mode;    // 记录当前网络模式
    Cmd cmd;

private slots:
    void recv_udp_data();
};

#endif // NET_H
