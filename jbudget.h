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
    float mBank;

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
    float getBank(){ return mBank; }
    void setBank(float bank){ mBank = bank; }
    jTransactionList * getTransactionList();

    void addCategory(QString heading, float amount);
    void removeCategory(QString heading);
    jCategory* getCategory(QString heading);
    float sumCategories();

    QList<jCategory*> getCategories(){ return mCategories; }

    bool save();
    bool save(QString fileName);
};

#endif // JBUDGET_H
