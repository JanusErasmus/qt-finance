#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "jbudget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    jBudget * mBudget;

    void openBudget(QString filename);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addTransaction();
    void openBudget();
    void saveBudget();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
