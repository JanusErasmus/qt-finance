#ifndef PARSETRANSACTIONS_H
#define PARSETRANSACTIONS_H
#include <QList>

#include "jtransaction.h"

class parseTransactions
{
public:
    struct pTransaction{
        jTransaction* mTrans;
        float mBank;
        pTransaction(){}
        pTransaction(jTransaction * t, float bank);
    };

private:

    QList<pTransaction*> mTransactions;



public:
    parseTransactions(QString fileName);

    QList<pTransaction*> getList(){ return mTransactions; }
};

#endif // PARSETRANSACTIONS_H
