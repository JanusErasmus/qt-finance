#include "jtransaction.h"

jTransaction::jTransaction()
{
}

jTransaction::jTransaction(QDate date, QString description, double amount)
{
    QLocale sar(QLocale::English, QLocale::SouthAfrica);

    mData.year = date.year();
    mData.month = date.month();
    mData.day = date.day();
    mData.amount = amount;
    strncpy((char*)mData.description, description.toLocal8Bit().data(), 64);

    mDateTableItem = new QTableWidgetItem(sar.toString(date, QLocale::ShortFormat));
    mDescTableItem = new QTableWidgetItem(description);
    mAmountTableItem = new QTableWidgetItem(sar.toCurrencyString(amount));
}

jTransaction::jTransaction(sData data) : mData(data)
{
    QLocale sar(QLocale::English, QLocale::SouthAfrica);

    QDate date(mData.year, mData.month, mData.day);

    mDateTableItem = new QTableWidgetItem(sar.toString(date, QLocale::ShortFormat));
    mDescTableItem = new QTableWidgetItem(QString((const char*)mData.description));
    mAmountTableItem = new QTableWidgetItem(sar.toCurrencyString(mData.amount));
}

void jTransaction::debugShow()
{
    qDebug("%s R%f", mData.description, mData.amount);
}

void jTransaction::setRow(QTableWidget* table, int row)
{
    table->setItem(row,0, mDateTableItem);
    table->setItem(row,1, mDescTableItem);
    table->setItem(row,2, mAmountTableItem);
}

jTransaction::~jTransaction()
{
    delete mDateTableItem;
    delete mDescTableItem;
    delete mAmountTableItem;
}

jTransaction::sData::sData()
{
    year = 0;
    month = 0;
    day = 0;
    memset(description, 0, 64);
    amount = 0;
}
