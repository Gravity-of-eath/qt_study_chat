#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "chatsession.h"

ChatWindow::ChatWindow(QWidget *parent, Device *d, LocalServer *server) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow) {
    this->dev = d;
    this->server = server;
    this->setWindowTitle("Chat " + dev->getName());
    ui->setupUi(this);
    ui->name->setText(dev->getName());
    ChatSession se = server->getOrCreateSession(d);
}

ChatWindow::~ChatWindow() {
    delete ui;
}

void ChatWindow::closeEvent(QCloseEvent *e) {
    emit onWindowClose(this);
}
