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
    jtransactionlist.cpp \
    jbudget.cpp \
    jtransaction.cpp \
    jcategory.cpp

HEADERS  += mainwindow.h \
    jtransactionlist.h \
    jbudget.h \
    jtransaction.h \
    jcategory.h

FORMS    += mainwindow.ui \
    addTransaction.ui
