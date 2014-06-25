#include "jtransaction.h"

jTransaction::jTransaction(QDate date, QString description, double amount)
{
    QLocale sar(QLocale::English, QLocale::SouthAfrica);

    mDate = new QTableWidgetItem(sar.toString(date, QLocale::ShortFormat));
    mDescription = new QTableWidgetItem(description);
    mAmount = new QTableWidgetItem(sar.toCurrencyString(amount));
}

void jTransaction::setRow(QTableWidget* table, int row)
{
    table->setItem(row,0, mDate);
    table->setItem(row,1, mDescription);
    table->setItem(row,2, mAmount);
}

jTransaction::~jTransaction()
{
    delete mDate;
    delete mDescription;
    delete mAmount;
}
