#ifndef ADDTRANS_H
#define ADDTRANS_H
#include <QDialog>
#include <QModelIndex>
#include <QStandardItemModel>

#include "jbudget.h"
#include "parsetransactions.h"

namespace Ui {
class addTransactions;
}

class addTransactions : public QDialog
{
    Q_OBJECT

    Ui::addTransactions *ui;

    int pTransIdx;

    jBudget * mBudget;
    QList<parseTransactions::pTransaction*> mTransactions;

    void nextPtrans(jTransaction * tr);
    void handleDescription(jCategory * cat);
    float round(float num);

public:
    explicit addTransactions(jBudget * budget = 0, QString fileName = 0, QWidget *parent = 0);
    ~addTransactions();

public slots:
    void okPressed();
    void cancelPressed();
    void catChange(int idx);

signals:
    void setAmount(float amount);

};

#endif // addTransactions_H
