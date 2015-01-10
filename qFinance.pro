#-------------------------------------------------
#
# Project created by QtCreator 2014-06-25T13:42:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qFinance
TEMPLATE = app

RC_FILE = qFinance.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    jtransactionlist.cpp \
    jbudget.cpp \
    jtransaction.cpp \
    jcategory.cpp \
    editcategories.cpp \
    parsetransactions.cpp \
    addtransactions.cpp

HEADERS  += mainwindow.h \
    jtransactionlist.h \
    jbudget.h \
    jtransaction.h \
    jcategory.h \
    editcategories.h \
    parsetransactions.h \
    addtransactions.h

FORMS    += mainwindow.ui \
    editcategories.ui \
    addtransactions.ui
