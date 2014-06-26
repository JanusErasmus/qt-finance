#include "jtransaction.h"

jTransaction::jTransaction()
{
    mEntry.amount = 0;
    mEntry.description[0] = 0;
}

jTransaction::jTransaction(QDate date, QString description, double amount)
{
    QLocale sar(QLocale::English, QLocale::SouthAfrica);

    mEntry.amount = amount;
    strcpy_s((char*)mEntry.description, 64, description.toLocal8Bit().data());

    mDate = new QTableWidgetItem(sar.toString(date, QLocale::ShortFormat));
    mDescription = new QTableWidgetItem(description);
    mAmount = new QTableWidgetItem(sar.toCurrencyString(amount));
}

void jTransaction::debugShow()
{
    qDebug("%s R%f", mEntry.description, mEntry.amount);
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

jTransaction::sEntry::sEntry()
{
    memset(description, 0, 64);
}
