#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>

#include "jbudget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    jBudget * mBudget;

    QComboBox* mSubCombo;
    QComboBox* mMainCombo;

    void openBudget(QString filename);
    void insertNewEntryRow();
    void fillCombo(QComboBox * main);
    void fillCombo(QComboBox * sub, jCategory * cat);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addTransaction();
    void openBudget();
    void saveBudget();

    void updateSubCombo(QString currSelection);
    void tableTransChange(int row, int col);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
