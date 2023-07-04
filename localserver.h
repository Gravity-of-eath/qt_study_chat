
#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "chatsession.h"

class ChatSession;

class LocalServer : public QTcpServer {
    Q_OBJECT

public:
    LocalServer(QObject *parent = 0);
    ChatSession *getOrCreateSession(QString name, QHostAddress *addr);

private:
    QMultiHash<QHostAddress, ChatSession *> sessions;
    int port = 45546;

signals:
    void newConnection(ChatSession *connection);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif
