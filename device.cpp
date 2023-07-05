#include "device.h"


Device::Device(QString name, QHostAddress addr) {
    this->name = name;
    this->addr = addr;
}

QString Device::getName() {
    return name;
}

QString Device::getIPString() {
    return QHostAddress(addr.toIPv4Address()).toString() ;
}

QHostAddress Device::getAddress() {
    return addr;
}


