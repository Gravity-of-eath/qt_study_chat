#include "userlistwindow.h"
#include "ui_userlistwindow.h"
#include "chatwindow.h"

UserListWindow::UserListWindow(QWidget *parent, QString name) :
    QMainWindow(parent),
    ui(new Ui::UserListWindow) {
    ui->setupUi(this);
    ui->listView->setSpacing(2);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));
//    setFixedSize (300,600);
    connect(ui->refresh, &QPushButton::clicked,
    this, [&] {
        caster->onLine();
    });
    this->name = name;
    ui->label->setText(name);
    caster = new BroadCaster(name);
    server = new LocalServer(this);
    connect(caster, SIGNAL(onDeviceStatus(Device *, bool)),
            this, SLOT(OnDevice(Device *, bool)));
    connect(this, SIGNAL(QWidget::closeEvent()), this, SLOT(onClosed()));
    connect(server, SIGNAL(LocalServer::newConnection(ChatSession *)), this, SLOT(newConnection(ChatSession *)));
}

UserListWindow::~UserListWindow() {
    delete ui;
}

UserListWindow::OnDevice(Device *d, bool onLine) {
    qDebug() << "OnDevice=--------";
    if(onLine) {
        devicesMap.insert(d->getIPString(), d);
    } else {
        devicesMap.remove(d->getIPString());
    }
    QStringList list;
    if(!devicesMap.keys().isEmpty()) {
        QMap<QString, Device *>::Iterator itrt = devicesMap.begin();
        while (itrt != devicesMap.end()) {
            Device *dev = itrt.value();
            list.append(QString(dev->getName()).append(" (" ).append( dev->getIPString()).append( ")"));
            itrt++;
        }
        QStringListModel *m = new QStringListModel(list);
        ui->listView->setModel(m);
    }
}

UserListWindow::onItemClicked(QModelIndex index) {
    int indexInt = index.row();
    qDebug() << "OnDevice=--------" << index.row()  ;
    if(indexInt >= 0 && indexInt < devicesMap.keys().size()) {
        QString key = devicesMap.keys().at(indexInt);
        Device *devs =  devicesMap.find(key).value();
        if(!opened.isEmpty()) {
            for(int i = 0; i < opened.size(); ++i) {
                ChatWindow *open = opened.at(i);
                if(open->dev == devs) {
                    open->showNormal();
                    open->setWindowState(Qt::WindowActive);
                    return 0;
                }
            }
        }
        QHostAddress addr = devs->getAddress();
        ChatSession *se = server->getOrCreateSession(devs->getName(),  &addr);
        ChatWindow *cw = new ChatWindow(NULL, devs, se);
        opened.append(cw);
        connect(cw, SIGNAL(onWindowClose(ChatWindow *)), this, SLOT(onWindowClose(ChatWindow *)));
        cw->show();
    }
}

UserListWindow::onWindowClose(ChatWindow *cw) {
    for(int i = 0; i < opened.size(); ++i) {
        ChatWindow *open = opened.at(i);
        if(cw->dev == open->dev) {
            opened.removeOne(open);
        }
        return 0;
    }
}

void UserListWindow::closeEvent(QCloseEvent *e) {
    caster->offLine();
    for(int i = 0; i < opened.size(); ++i) {
        ChatWindow *open = opened.at(i);
        open->close();
    }
    exit(0);
}

UserListWindow::newConnection(ChatSession *cs) {
    qDebug() << "UserListWindow.newConnection";
    QHostAddress paddr = cs->peerAddress();
    Device *devs;
    QString key = paddr.toString();
    if( devicesMap.keys().contains(key)) {
        devs =  devicesMap.find(key).value();
    } else {
        devs = new Device(cs->name(), paddr);
        devicesMap.insert(key, devs);
    }
    cs->d = devs;
    cs->localName = this->name;
    ChatWindow *cw = new ChatWindow(NULL, devs, cs);
    opened.append(cw);
    connect(cw, SIGNAL(onWindowClose(ChatWindow *)), this, SLOT(onWindowClose(ChatWindow *)));
    cw->show();
}
