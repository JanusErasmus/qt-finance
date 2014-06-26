#include "jtransaction.h"

jTransaction::jTransaction()
{
    mEntry.amount = 0;
    mEntry.description[0] = 0;
}

jTransaction::jTransaction(QDate date, QString description, double amount)
{
    QLocale sar(QLocale::English, QLocale::SouthAfrica);

    mEntry.year = date.year();
    mEntry.month = date.month();
    mEntry.day = date.day();

    mEntry.amount = amount;
    strncpy((char*)mEntry.description, description.toLocal8Bit().data(), 64);

    mDate = new QTableWidgetItem(sar.toString(date, QLocale::ShortFormat));
    mDescription = new QTableWidgetItem(description);
    mAmount = new QTableWidgetItem(sar.toCurrencyString(amount));
}

jTransaction::jTransaction(sData data) : mEntry(data)
{
    QLocale sar(QLocale::English, QLocale::SouthAfrica);

    QDate date(mEntry.year, mEntry.month, mEntry.day);

    mDate = new QTableWidgetItem(sar.toString(date, QLocale::ShortFormat));
    mDescription = new QTableWidgetItem(QString((const char*)mEntry.description));
    mAmount = new QTableWidgetItem(sar.toCurrencyString(mEntry.amount));
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

jTransaction::sData::sData()
{
    year = 0;
    month = 0;
    day = 0;
    memset(description, 0, 64);
    amount = 0;
}
