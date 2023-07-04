#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "chatsession.h"

ChatWindow::ChatWindow(QWidget *parent, Device *dev, ChatSession *se) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow) {
    this->se = se;
    this->dev = dev;
    this->setWindowTitle("Chat " + dev->getName());
    ui->setupUi(this);
    ui->name->setText(se->name());
    init();
}

ChatWindow::~ChatWindow() {
    delete ui;
}

ChatWindow::init() {
    connect(se, SIGNAL(readyForUse()), this, SLOT(readyForUse()));
    connect(ui->send, &QPushButton::clicked, [&] {
        QString ss =  this->ui->input->toPlainText();
        this->ui->input->setText("");
        this->ui->msgBox->append(se->localName);
        this->ui->msgBox->append(":");
        this->ui->msgBox->append(ss);
        this->ui->msgBox->append("\n");
        se->sendMessage(ss);
    });
}

void ChatWindow::onNewMessage(QString name, QString msg) {
    ui->msgBox->append(name);
    ui->msgBox->append(":");
    ui->msgBox->append(msg);
    ui->msgBox->append("\n");
}

void ChatWindow::readyForUse() {
    connect(se, SIGNAL(newMessage(QString, QString)), this, SLOT(onNewMessage(QString, QString)));
}

void ChatWindow::closeEvent(QCloseEvent *e) {
    emit onWindowClose(this);
}
