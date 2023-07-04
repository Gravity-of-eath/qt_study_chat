
#include <QtNetwork>

#include "chatsession.h"
#include "localserver.h"

LocalServer::LocalServer(QObject *parent)
    : QTcpServer(parent) {
    listen(QHostAddress::Any);
}

ChatSession *LocalServer::getOrCreateSession(QString name, QHostAddress *addr) {
    ChatSession *session;
    if( sessions.keys().contains(*addr) ) {
        session = sessions.find(*addr).value();
    } else {
        session = new ChatSession(this);
        session->connectToHost(*addr, port);
        session->localName = name;
        sessions.insert(*addr, session);
    }
    return session;
}

void LocalServer::incomingConnection(qintptr socketDescriptor) {
    ChatSession *session = new ChatSession(this);
    session->setSocketDescriptor(socketDescriptor);
    emit newConnection(session);
}
