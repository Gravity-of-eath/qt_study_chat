#ifndef USERLISTWINDOW_H
#define USERLISTWINDOW_H

#include <QMainWindow>
#include "broadcaster.h"
#include "device.h"
#include "chatwindow.h"

namespace Ui {
    class UserListWindow;
}

class UserListWindow : public QMainWindow {
    Q_OBJECT

public:
    UserListWindow(QWidget *parent = 0, QString name = "NAME");
    ~UserListWindow();

private:
    Ui::UserListWindow *ui;
    QString name;
    BroadCaster *caster;
    QList <ChatWindow *> opened;
    QMap<QString, Device *> devicesMap;


public slots:
    OnDevice(Device *d, bool onLine);
    onItemClicked(QModelIndex index);
    onWindowClose(ChatWindow *cw);
    void closeEvent(QCloseEvent *e);
};

#endif // USERLISTWINDOW_H
