#-------------------------------------------------
#
# Project created by QtCreator 2014-06-25T13:42:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qFinance
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    jtransaction.cpp \
    jtransactionlist.cpp

HEADERS  += mainwindow.h \
    jtransaction.h \
    jtransactionlist.h

FORMS    += mainwindow.ui
