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

    jBudget * mBudget;

    QComboBox* mSubCombo;
    QComboBox* mMainCombo;
    int mCurrentComboIndex;
    int mEditRow;

    void insertNewEntryRow();
    void fillCombo(QComboBox * main);
    void fillCombo(QComboBox * sub, jCategory * cat);

    QList<QStandardItem*> fillCategory(jCategory * cat, jCategory::sCategory * subCat);

    void applyTransChanges();

    jBudget * generateNextBudget(QString filename);

public:
    explicit MainWindow(QWidget *parent = 0);

    void openBudget(QString filename);

    ~MainWindow();

public slots:
    void openListWindow();
    void editCategoryWindow();
    void openBudget();
    void saveBudget();
    void saveBudgetAs();
    void newBudget();

    void updateAmount2Category(int index);
    void updateAmount2subCategory(int index);
    void updateSubCombo(QString currSelection);    
    void tableTransChange(int row, int col);
    void tableTransDoubleClick(int row, int col);

     void fillTree();
     void updateBank();
     void updateBank(float amount);

     void generateNextMonth();

     QBrush getBrush(float val);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
