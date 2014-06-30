#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QComboBox>
#include <QStandardItemModel>
#include <QLabel>

#include "jbudget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    //QLabel * mStatusLabel;

    jBudget * mBudget;

    QComboBox* mSubCombo;
    QComboBox* mMainCombo;
    int mEditRow;

    void openBudget(QString filename);
    void insertNewEntryRow();
    void fillCombo(QComboBox * main);
    void fillCombo(QComboBox * sub, jCategory * cat);

    QList<QStandardItem*> fillCategory(jCategory * cat, jCategory::sCategory * subCat);

    void applyTransChanges();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void editCategoryWindow();
    void openBudget();
    void saveBudget();

    void updateSubCombo(QString currSelection);
    void tableTransChange(int row, int col);
    void tableTransDoubleClick(int row, int col);

     void fillTree();
     void updateBank();

     void updateLeftLabel(QString subString);

     QBrush getBrush(float val);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
