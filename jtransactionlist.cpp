#include "jtransactionlist.h"

jTransactionList::jTransactionList()
{
}

void jTransactionList::append(jTransaction* entry)
{
    mList.append(entry);
}

void jTransactionList::fillTable(QTableWidget* table)
{
    jTransaction * entry;

   int row = 0;
   foreach (entry, mList)
   {
       table->insertRow(row);
       entry->setRow(table, row);

       row++;
   }
}
