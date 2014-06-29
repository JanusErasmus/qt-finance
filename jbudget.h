#ifndef JBUDGET_H
#define JBUDGET_H
#include <QFile>

#include "jcategory.h"
#include "jtransactionlist.h"

#define VERSION 0x000001

class jBudget
{
    QFile * mBudgetFile;
    float mIncome;

    jTransactionList* mTransList;
    QList<jCategory*> mCategories;

    void readBudget();

    void writeTransactions();
    void readTransactions();

    void writeCategories();
    void readCategories();

public:
    jBudget(QString fileName);
    ~jBudget();

    float getIncome(){ return mIncome; }
    void setIncome(float income){ mIncome = income; }
    jTransactionList * getTransactionList();

    void addCategory(QString heading, float amount);
    jCategory* getCategory(QString heading);

    QList<jCategory*> getCategories(){ return mCategories; }



    bool save();
};

#endif // JBUDGET_H
