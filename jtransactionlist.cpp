#include "jtransactionlist.h"

jTransactionList::jTransactionList()
{
}

float jTransactionList::sumTransactions()
{
    float sum = 0;
    jTransaction * t;
    foreach(t, mList)
    {
        sum += t->getAmount();
    }

    return sum;
}

float jTransactionList::sumTransactions(QString category)
{
    float sum = 0;
    jTransaction * t;
    foreach(t, mList)
    {
        if(t->getCategory() == category)
            sum += t->getAmount();
    }

    return sum;
}

float jTransactionList::sumTransactions(QString category, QString subCategory)
{
    float sum = 0;
    jTransaction * t;
    foreach(t, mList)
    {
        if((t->getCategory() == category) && (t->getDescription() == subCategory))
            sum += t->getAmount();
    }

    return sum;
}

void jTransactionList::append(jTransaction* entry)
{
    mList.append(entry);
}

 int jTransactionList::size()
 {
     return mList.size();
 }

 jTransaction* jTransactionList::at(int idx)
 {
     return mList.at(idx);
 }

void jTransactionList::fillTable(QTableWidget* table)
{
    //reset table
    table->setRowCount(0);

    int row = 0;
    jTransaction * entry;
    foreach (entry, mList)
    {
        table->insertRow(row);
        entry->setRow(table, row);

        row++;
    }
}

jTransactionList::~jTransactionList()
{
    jTransaction * entry;
    foreach (entry, mList)
    {
        delete entry;
    }
}
