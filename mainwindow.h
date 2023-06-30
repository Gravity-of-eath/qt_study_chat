#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initName();

private:
    Ui::MainWindow *ui;
    QStringList userList;
    QStringList ipList;
    QByteArray username;
};

#endif // MAINWINDOW_H
