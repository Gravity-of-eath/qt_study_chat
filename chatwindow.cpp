#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent, Device *d) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow) {
    this->dev = d;
    this->setWindowTitle("Chat " + dev->getName());
    ui->setupUi(this);
    ui->name->setText(dev->getName());
}

ChatWindow::~ChatWindow() {
    delete ui;
}

void ChatWindow::closeEvent(QCloseEvent *e) {
    emit onWindowClose(this);
}
