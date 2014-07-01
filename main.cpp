#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool validFlag = false;
    QString fileName;

    if(argc > 1)
    {
        //qDebug("File name %s", argv[1]);
        fileName = QString(argv[1]);
        if(fileName.split(".").at(1) == "jbud")
        {
            //qDebug("Valid file");
            validFlag = true;
        }
    }

    MainWindow w;

    if(validFlag)
        w.openBudget(fileName);

    w.show();

    return a.exec();
}
