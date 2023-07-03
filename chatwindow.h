#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include "device.h"
#include "localserver.h"

namespace Ui {
    class ChatWindow;
}

class ChatWindow : public QMainWindow {
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = 0, Device *d = NULL, LocalServer *server = NULL);
    Device *dev;
    ~ChatWindow();

private:
    Ui::ChatWindow *ui;
    LocalServer *server;
signals:
    onWindowClose(ChatWindow *cw);
public slots:

    void closeEvent(QCloseEvent *e);
};

#endif // CHATWINDOW_H
