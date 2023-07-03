#-------------------------------------------------
#
# Project created by QtCreator 2023-06-29T16:30:08
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LocalChat
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    userlistwindow.cpp \
    broadcaster.cpp \
    device.cpp \
    chatwindow.cpp \
    localserver.cpp \
    chatsession.cpp

HEADERS += \
        mainwindow.h \
    userlistwindow.h \
    broadcaster.h \
    device.h \
    chatwindow.h \
    localserver.h \
    chatsession.h

FORMS += \
        mainwindow.ui \
    userlistwindow.ui \
    chatwindow.ui
