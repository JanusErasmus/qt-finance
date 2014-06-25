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
    void append(jTransaction* entry);

    void fillTable(QTableWidget* table);
};

#endif // JTRANSACTIONLIST_H
