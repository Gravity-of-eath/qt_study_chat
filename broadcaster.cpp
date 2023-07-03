#include <QtNetwork>
#include "broadcaster.h"
#include "device.h"

bool is_virtual_network_card_or_loopback(QString str_card_name);
QString get_local_ip();
BroadCaster::BroadCaster(QString name  ) {
    this->name = name;
    init();
}


void BroadCaster::init() {
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(port, QUdpSocket::ShareAddress);
    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(OnReceiveData()));
    onLine();
}

void BroadCaster::onLine() {
    QJsonObject myData;
    myData.insert(NAME, name);
    myData.insert(IP_ADDR, get_local_ip());
    myData.insert(STATUS, "onLine");
    QJsonDocument document;
    document.setObject(myData);
    QByteArray array = document.toJson(QJsonDocument::Compact);
    udpSocket->writeDatagram(array, array.size(),
                             QHostAddress::Broadcast, port);
}

void BroadCaster::offLine() {
    QJsonObject myData;
    myData.insert(NAME, name);
    myData.insert(IP_ADDR, get_local_ip());
    myData.insert(STATUS, "offLine");
    QJsonDocument document;
    document.setObject(myData);
    QByteArray array = document.toJson(QJsonDocument::Compact);
    udpSocket->writeDatagram(array, array.size(),
                             QHostAddress::Broadcast, port);
}

void BroadCaster::reply(QHostAddress ip) {
    QJsonObject myData;
    myData.insert(NAME, name);
    myData.insert(IP_ADDR, get_local_ip());
    myData.insert(STATUS, "onLine");
    QJsonDocument document;
    document.setObject(myData);
    QByteArray array = document.toJson(QJsonDocument::Compact);
    udpSocket->writeDatagram(array, array.size(),
                             ip, port);
}



BroadCaster::OnReceiveData() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        QJsonParseError jsonError;
        QString strName;
        QString strStatus;
        QHostAddress senderIp;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size(), &senderIp);
        QJsonDocument document = QJsonDocument::fromJson(datagram.data(), &jsonError); //转化为JSON文档
        if( !document.isNull() && (jsonError.error == QJsonParseError::NoError)) { //解析未发生错误
            if(document.isObject()) {
                QJsonObject object = document.object();
                // 第一项
                if(object.contains(NAME)) {
                    QJsonValue nameChar = object.value(NAME);
                    if(nameChar.isString()) {
                        strName = nameChar.toString();
                        qDebug() << "Devidce name is: 　" << strName ;
                    } else {
                        qDebug("name is not string!");
                    }
                }
                // 第二项
                if(object.contains(IP_ADDR)) {
                    QJsonValue ipAddr = object.value(IP_ADDR);
                    if(ipAddr.isString()) {
                        QString strIp = ipAddr.toString();
                        qDebug() << "RealIp : " << senderIp << " report ip is :" << strIp;
                    }
                }
                if(object.contains(STATUS)) {
                    QJsonValue statusValue = object.value(STATUS);
                    if(statusValue.isString()) {
                        strStatus = statusValue.toString();
                        qDebug() << "status is :" << strStatus;
                    }
                }
            }
        } else {
            qDebug() << "Json error :" << QString(datagram.data());
        }
        if(!strName.isEmpty()  && !strStatus.isEmpty()) {
            qDebug() << strName << "-- " << senderIp << "-- " << strStatus;
            Device *d = new Device(strName, senderIp);
            if(strStatus == "onLine") {
                QString localIP = get_local_ip();
                if(localIP == senderIp.toString()) {
                    qDebug() << "self online ip = " << localIP;
                } else {
                    reply(senderIp);
                    emit onDeviceStatus(d, true);
                }
            } else {
                emit onDeviceStatus(d, false);
            }
        } else {
            qDebug() << "Json error name:  " << strName << "  strIp:" << senderIp << "  strStatus:" << strStatus;
        }
    }
}

/**
 * @brief 检测当前网卡是否是虚拟网卡(VMware/VirtualBox)或回环网卡
 * @param str_card_name  网卡的描述信息
 * @return 如果是虚拟网卡或回环网卡，返回true, 否则返回false
 */
bool is_virtual_network_card_or_loopback(QString str_card_name) {
    if (-1 != str_card_name.indexOf("VMware")
            || -1 != str_card_name.indexOf("Loopback")
            || -1 != str_card_name.indexOf("VirtualBox")
       ) {
        return true;
    }
    return false;
}

/**
 * @brief 获取本机IP地址
 */
QString get_local_ip() {
    QString localIP = "127.0.0.1";
    // 1. 获取所有网络接口
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    QList<QNetworkAddressEntry> entry;
    foreach(QNetworkInterface inter, interfaces) {
        // 过滤掉不需要的网卡信息
        if (is_virtual_network_card_or_loopback(inter.humanReadableName())) {
            continue;
        }
        if (inter.flags() & (QNetworkInterface::IsUp | QNetworkInterface::IsRunning)) {
            entry = inter.addressEntries();
            // entry.at(0) 是IPv6信息
            if (entry.at(1).ip().protocol() == QAbstractSocket::IPv4Protocol) {
                if (-1 != inter.name().indexOf("wireless")) {
                    localIP = entry.at(1).ip().toString();
                    qDebug() << inter.humanReadableName() << inter.name() << " 无线网IP: " << entry.at(1).ip().toString();
                } else if (-1 != inter.name().indexOf("ethernet")) {
                    localIP = entry.at(1).ip().toString();
                    qDebug() << inter.humanReadableName() << inter.name() << " 以太网IP: " << entry.at(1).ip().toString();
                }
            }
            entry.clear();
        }
    }
    return localIP;
}
