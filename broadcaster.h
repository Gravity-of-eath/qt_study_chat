#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <QObject>
#include <QtNetwork>
#include "device.h"

class BroadCaster: public QObject
{
    Q_OBJECT
public:
    int port = 45456;
    explicit BroadCaster(QString name);
    const char *NAME = "name";
    const char *IP_ADDR = "ip_address";
    const char *STATUS = "status";
    const char *REPTY = "repty";
    void init();
    void onLine();
    void offLine();
    void reply(QHostAddress ip);
private:
    QString name;
    QUdpSocket *udpSocket;

signals:
    onDeviceStatus(Device *d, bool online);

public slots:
    OnReceiveData();
};

#endif // BROADCASTER_H
