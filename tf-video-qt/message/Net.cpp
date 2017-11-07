#include "Net.h"
#include "Log.h"
#include <QFile>



#define NET_PARAM_PATH "../led/param/net.xml"

Net::Net(Controller *_controller)
    : cmd(_controller)
{
    controller = _controller;
    load_param();

    init_net();
}

Net::~Net()
{
    release_net();
}

bool Net::init_net()
{
    udp_socket = new QUdpSocket(this);

    if (NET_LOCAL_MODE == (enum net_mode_e)m_lanParam.workType) {
        net_mode = NET_LOCAL_MODE;
        start_local_net_mode();
    } else if (NET_REMOTE_MODE == (enum net_mode_e)m_lanParam.workType) {
        net_mode = NET_REMOTE_MODE;
        start_remote_net_mode();
    }

    return true;
}


void Net::release_net()
{
    delete udp_socket;

}

bool Net::start_local_net_mode()
{
    connect(udp_socket, SIGNAL(readyRead()), this, SLOT(recv_udp_data()));
    bool ret = udp_socket->bind(5959);
    if (!ret) {
        print_error("udp_socket->bind error");
        return false;
    }

    return true;
}

bool Net::finish_local_net_mode()
{

    return true;
}
bool Net::start_remote_net_mode()
{

    return true;
}
bool Net::finish_remote_net_mode()
{

    return true;
}

void Net::recv_udp_data()
{
    int ret = 0;
    char buf[BUF_SIZE];
    while(udp_socket->hasPendingDatagrams())
    {
        int size = udp_socket->pendingDatagramSize();

        ret += udp_socket->readDatagram(buf, size, &sender_addr, &sender_port);
        qDebug() << "recived data from:" << sender_addr.toString() << "port:" << sender_port;
        /*for (int i=0; i<ret; i++) {
            qDebug("0x%x ", buf[i] & 0xff);
        }*/
    }

    cmd.decode_cmd(buf, ret);
}

bool Net::parse_lan_param(QDomElement lan_node)
{
    while (!lan_node.isNull()) {
        if ("localport" == lan_node.tagName()) {
            m_lanParam.localPort = lan_node.text().toInt();

        } else if ("localip" == lan_node.tagName()) {
            QByteArray ba = lan_node.text().toLatin1();
            char* s = ba.data();
            int len = IP_SIZE < strlen(s) ? IP_SIZE-1 : strlen(s);
            memset(m_lanParam.localIp, 0, IP_SIZE);
            memcpy(m_lanParam.localIp, s, len);

        } else if ("submask" == lan_node.tagName()) {
            QByteArray ba = lan_node.text().toLatin1();
            char* s = ba.data();
            int len = IP_SIZE < strlen(s) ? IP_SIZE-1 : strlen(s);
            memset(m_lanParam.subMask, 0, IP_SIZE);
            memcpy(m_lanParam.subMask, s, len);

        } else if ("gateway" == lan_node.tagName()) {
            QByteArray ba = lan_node.text().toLatin1();
            char* s = ba.data();
            int len = IP_SIZE < strlen(s) ? IP_SIZE-1 : strlen(s);
            memset(m_lanParam.gateway, 0, IP_SIZE);
            memcpy(m_lanParam.gateway, s, len);

        } else if ("localmac" == lan_node.tagName()) {
            QByteArray ba = lan_node.text().toLatin1();
            char* s = ba.data();
            int len = IP_SIZE < strlen(s) ? IP_SIZE-1 : strlen(s);
            memset(m_lanParam.localMac, 0, IP_SIZE);
            memcpy(m_lanParam.localMac, s, len);

            QString u = lan_node.attributeNode("update").value();
            m_lanParam.mdfMacEnable = u.toInt();

        } else if ("dhcptopc" == lan_node.tagName()) {
            m_lanParam.dhcpToPc = lan_node.text().toInt();

        } else if ("autoip" == lan_node.tagName()) {
            m_lanParam.autoIp = lan_node.text().toInt();

        } else if ("worktype" == lan_node.tagName()) {
            m_lanParam.workType = lan_node.text().toInt();
        }

        lan_node = lan_node.nextSiblingElement();
    }

    return true;
}


bool Net::parse_wan_param(QDomElement wan_node)
{
    while (!wan_node.isNull()) {
        if ("devicename" == wan_node.tagName()) {
            QByteArray ba = wan_node.text().toLatin1();
            char* s = ba.data();
            int len = 20 < strlen(s) ? 20-1 : strlen(s);
            memset(m_wanParam.devName, 0, 20);
            memcpy(m_wanParam.devName, s, len);

        } else if ("remoteport" == wan_node.tagName()) {
            m_wanParam.remotePort = wan_node.text().toInt();

        } else if ("remoteaddr" == wan_node.tagName()) {
            QByteArray ba = wan_node.text().toLatin1();
            char* s = ba.data();
            int len = IP_SIZE < strlen(s) ? IP_SIZE-1 : strlen(s);
            memset(m_wanParam.remoteAddr, 0, IP_SIZE);
            memcpy(m_wanParam.remoteAddr, s, len);

        } else if ("dns" == wan_node.tagName()) {
            QByteArray ba = wan_node.text().toLatin1();
            char* s = ba.data();
            int len = IP_SIZE < strlen(s) ? IP_SIZE-1 : strlen(s);
            memset(m_wanParam.dns, 0, IP_SIZE);
            memcpy(m_wanParam.dns, s, len);

        } else if ("apn" == wan_node.tagName()) {
            QByteArray ba = wan_node.text().toLatin1();
            char* s = ba.data();
            int len = 8 < strlen(s) ? 8-1 : strlen(s);
            memset(m_wanParam.apn, 0, 8);
            memcpy(m_wanParam.apn, s, len);

        } else if ("protocol" == wan_node.tagName()) {
            m_wanParam.protocol = wan_node.text().toInt();

        } else if ("heartinterval" == wan_node.tagName()) {
            m_wanParam.heartInterval = wan_node.text().toInt();

        } else if ("imei" == wan_node.tagName()) {
            // 此处调用获取ime码函数
        }

        wan_node = wan_node.nextSiblingElement();
    }

    return true;
}


bool Net::load_param()
{
    return parse_param_file(NET_PARAM_PATH);
}

bool Net::parse_param_file(const char* file_name)
{
    if (!file_name) {
        print_error("invalid parameter");
        return false;
    }

    QFile file(file_name);

    if (!file.open(QIODevice::ReadOnly)) {
        print_error("open file:\"%s\" error", file_name);
        return false;
    }

    int size = file.size() + 1;
    char* buf = new char[size];
    memset(buf, 0, size);

    size = file.read(buf, size);

    parse_param_buf(buf, size);

    file.close();
    delete []buf;

    return true;
}

bool Net::parse_param_buf(char* buf, int size)
{
    if (NULL==buf || size<=0) {
        print_error("invalid paratemeter");
        return false;
    }

    bool ret = false;
    QDomDocument doc("param");

    // debug("%s\n", buf);
    QByteArray bt(buf, size);
    if (!doc.setContent(bt)) {
        print_error("doc.setContent() error");
        return false;
    }

    QDomElement root = doc.documentElement();

    //debug("root node:%s\n", root.tagName().toLatin1().data());
    //debug("root node:%s\n", child.tagName().toLatin1().data());
    if ("led595" != root.tagName()) {
        print_error("invalid node");
        return false;
    }

    QDomElement child = root.firstChildElement();
    while (!child.isNull()) {
        if ("lancfg" == child.tagName()) {
            ret = parse_lan_param(child.firstChildElement());
        } else if ("wancfg" == child.tagName()) {
            ret = parse_wan_param(child.firstChildElement());
        }

        child = child.nextSiblingElement();
    }

    return ret;
}

bool Net::sava_param()
{
    return save_param_to_file(NET_PARAM_PATH);
    // return save_param_to_file("../led/tmp.xml");
}

bool Net::save_param_to_file(const char* file_name)
{
    if (!file_name) {
        print_error("invalid parameter");
        return false;
    }

    char *buf = NULL;
    int size = 0;

    if (!save_param_to_buf(&buf, &size)) {
        return false;
    }

    QFile file(file_name);

    if (!file.open(QIODevice::WriteOnly)) {
        print_error("open file:\"%s\" error", file_name);
        return false;
    }

    file.write(buf, size);
    file.close();
    delete []buf;

    return true;
}

bool Net::save_param_to_buf(char** buf, int *size)
{
    QDomDocument doc;
    QDomText text;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction( "xml", "version = \"1.0\" encoding=\"utf-8\"" );
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("led595");
    doc.appendChild(root);

    add_lan_param_to_xml(root);

    add_wan_param_to_xml(root);

    QString s;
    QTextStream out(&s);
    doc.save(out, 4);

    //qDebug() << s;
    //return false;

    QByteArray ba = s.toLatin1();
    char *mm = ba.data();

    *size = strlen(mm);
    *buf = new char[*size + 1];
    memset(*buf, 0, *size+1);
    if (NULL == *buf) {
        print_error("new buf error");
        return false;
    }
    memcpy(*buf, mm, *size);

    //qDebug() << *buf;
    //printf("%s\n", *buf);
    //return false;

    return true;
}

bool Net::add_lan_param_to_xml(QDomElement &root)
{
    QDomDocument doc;
    QDomElement lan_node = doc.createElement("lancfg");
    root.appendChild(lan_node);

    QDomElement sub_node = doc.createElement("localport");
    QDomText text = doc.createTextNode(QString().setNum(m_lanParam.localPort));
    sub_node.appendChild(text);
    lan_node.appendChild(sub_node);

    sub_node = doc.createElement("localip");
    text = doc.createTextNode(QString(m_lanParam.localIp));
    sub_node.appendChild(text);
    lan_node.appendChild(sub_node);

    sub_node = doc.createElement("submask");
    text = doc.createTextNode(QString(m_lanParam.subMask));
    sub_node.appendChild(text);
    lan_node.appendChild(sub_node);

    sub_node = doc.createElement("gateway");
    text = doc.createTextNode(QString(m_lanParam.gateway));
    sub_node.appendChild(text);
    lan_node.appendChild(sub_node);

    sub_node = doc.createElement("localmac");
    text = doc.createTextNode(QString(m_lanParam.localMac));
    sub_node.appendChild(text);
    sub_node.setAttribute("update", m_lanParam.mdfMacEnable);
    lan_node.appendChild(sub_node);

    sub_node = doc.createElement("dhcptopc");
    text = doc.createTextNode(QString().setNum(m_lanParam.dhcpToPc));
    sub_node.appendChild(text);
    lan_node.appendChild(sub_node);

    sub_node = doc.createElement("autoip");
    text = doc.createTextNode(QString().setNum(m_lanParam.autoIp));
    sub_node.appendChild(text);
    lan_node.appendChild(sub_node);

    sub_node = doc.createElement("worktype");
    text = doc.createTextNode(QString().setNum(m_lanParam.workType));
    sub_node.appendChild(text);
    lan_node.appendChild(sub_node);

    return true;
}

bool Net::add_wan_param_to_xml(QDomElement &root)
{
    QDomDocument doc;
    QDomElement wan_node = doc.createElement("wancfg");
    root.appendChild(wan_node);

    QDomElement sub_node = doc.createElement("devicename");
    QDomText text = doc.createTextNode(QString(m_wanParam.devName));
    sub_node.appendChild(text);
    wan_node.appendChild(sub_node);

    sub_node = doc.createElement("remoteport");
    text = doc.createTextNode(QString().setNum(m_wanParam.remotePort));
    sub_node.appendChild(text);
    wan_node.appendChild(sub_node);

    sub_node = doc.createElement("remoteaddr");
    text = doc.createTextNode(QString(m_wanParam.remoteAddr));
    sub_node.appendChild(text);
    wan_node.appendChild(sub_node);

    sub_node = doc.createElement("dns");
    text = doc.createTextNode(QString(m_wanParam.dns));
    sub_node.appendChild(text);
    wan_node.appendChild(sub_node);

    sub_node = doc.createElement("apn");
    text = doc.createTextNode(QString(m_wanParam.apn));
    sub_node.appendChild(text);
    wan_node.appendChild(sub_node);

    sub_node = doc.createElement("protocol");
    text = doc.createTextNode(QString().setNum(m_wanParam.protocol));
    sub_node.appendChild(text);
    wan_node.appendChild(sub_node);

    sub_node = doc.createElement("heartinterval");
    text = doc.createTextNode(QString().setNum(m_wanParam.heartInterval));
    sub_node.appendChild(text);
    wan_node.appendChild(sub_node);

    sub_node = doc.createElement("imei");
    //text = doc.createTextNode(QString(wan_param.serial_num));
    //sub_node.appendChild(text);
    wan_node.appendChild(sub_node);

    return true;
}


net_mode_e Net::get_cur_net_mode()
{
    return net_mode;
}


bool Net::udp_reply_data(const char* buf, u32 size)
{
    if (0==buf || 0==size) {
        print_error("invalid parameter");
        return false;
    }

    qDebug() << sender_addr.toString() << " " << sender_port;
    u32 ret = udp_socket->writeDatagram(buf, size, sender_addr, sender_port);
    if (ret != size) {
        print_error("udp send data error");
        return false;
    }

    return true;
}

bool Net::tcp_reply_data(const char* buf, u32 size)
{
    if (!buf || size==0) {
        return false;
    }

    return true;
}
