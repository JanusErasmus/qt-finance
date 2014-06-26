#ifndef JBUDGET_H
#define JBUDGET_H
#include <QFile>

#include "jtransactionlist.h"

#define VERSION 0x000001

class jBudget
{
    QFile * mBudgetFile;

    jTransactionList* mTransList;

    void readBudget();

public:
    jBudget(QString fileName);
    ~jBudget();

     jTransactionList * getTransactionList();
     bool save();
};

#endif // JBUDGET_H
