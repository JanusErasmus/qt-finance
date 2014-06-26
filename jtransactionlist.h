#ifndef JTRANSACTIONLIST_H
#define JTRANSACTIONLIST_H
#include <QTableWidgetItem>
#include <QDate>

#include "jtransaction.h"

class jTransactionList
{
     QList<jTransaction*> mList;

public:
    jTransactionList();
    ~jTransactionList();

    void append(jTransaction* entry);
    int size();
    jTransaction * at(int idx);

    void fillTable(QTableWidget* table);
};

#endif // JTRANSACTIONLIST_H
