#ifndef DEVICE_H
#define DEVICE_H

#include <QtNetwork>

class Device {

public:
    Device(QString name, QHostAddress addr);
    QString getName();
    QString getIPString();
    QHostAddress getAddress();
    bool  operator ==( Device *out) {
//        if(getAddress() == out->getAddress()) {
//            return true;
//        }
        return false;
    }

private:
    QString name;
    QHostAddress addr;
};

#endif // DEVICE_H
