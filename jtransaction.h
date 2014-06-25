#ifndef JTRANSACTION_H
#define JTRANSACTION_H
#include <QTableWidgetItem>
#include <QDate>

class jTransaction
{
    QTableWidgetItem * mDate;
    QTableWidgetItem * mDescription;
    QTableWidgetItem * mAmount;

    public:
    jTransaction(QDate date, QString description, double amount);
    ~jTransaction();

    void setRow(QTableWidget* table, int row);
};

#endif // JTRANSACTION_H
