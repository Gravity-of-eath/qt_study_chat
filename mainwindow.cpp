#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userlistwindow.h"
#include <QProcessEnvironment>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initName();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initName() {
    QStringList envVariables;
    envVariables << "USERNAME" << "USER" << "USERDOMAIN"
                 << "HOSTNAME" << "DOMAINNAME";
    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    foreach (QString string, envVariables) {
        if (environment.contains(string)) {
            username = environment.value(string).toUtf8();
            break;
        }
    }
    if (username.isEmpty()) {
        username = "unknown";
    }
    ui->lineEdit->setText(username);
    connect(ui->loginButton, &QPushButton::clicked, [&] {
        QString name = ui->lineEdit->text();
        if (name.isEmpty())
            username = "unknown";
        UserListWindow *ulw = new UserListWindow(NULL, username);
        ulw->show();
        this->close();
    });
}
