#include <QDebug>

#include "jtransaction.h"

jTransaction::jTransaction()
{
}

jTransaction::jTransaction(QDate date, QString category, QString description, double amount) :
    mDate(date),
    mCategory(category),
    mDescription(description),
    mAmount(amount)
{
}

jTransaction::jTransaction(sData data)
{    
    mDate = QDate(data.year, data.month, data.day);
    mDescription = QString((const char*)data.description);
    mCategory = QString((const char*)data.category);
    mAmount = data.amount;
}

void jTransaction::debugShow()
{
    qDebug() << mCategory << mDescription << mAmount;
}

void jTransaction::debugShow(sData data)
{
    qDebug("%s R%f", data.description, data.amount);
}

void jTransaction::setRow(QTableWidget* table, int row)
{
    QTableWidgetItem * dateTableItem;
    QTableWidgetItem * descTableItem;
    QTableWidgetItem * catTableItem;
    QTableWidgetItem * amountTableItem;

    dateTableItem = new QTableWidgetItem();
    dateTableItem->setFlags(dateTableItem->flags() ^ Qt::ItemIsEditable);
    dateTableItem->setData(Qt::EditRole, mDate);

    descTableItem = new QTableWidgetItem(mDescription);
    descTableItem->setFlags(descTableItem->flags() ^ Qt::ItemIsEditable);

    catTableItem = new QTableWidgetItem(mCategory);
    catTableItem->setFlags(catTableItem->flags() ^ Qt::ItemIsEditable);

    amountTableItem = new QTableWidgetItem(QLocale().toCurrencyString(mAmount));
    amountTableItem->setFlags(amountTableItem->flags() ^ Qt::ItemIsEditable);

    table->setItem(row,0, dateTableItem);
    table->setItem(row,1, catTableItem);
    table->setItem(row,2, descTableItem);
    table->setItem(row,3, amountTableItem);
}
jTransaction::sData jTransaction::getData()
{
    sData data;

    data.year = mDate.year();
    data.month = mDate.month();
    data.day = mDate.day();
    data.amount = mAmount;
    strncpy_s((char*)data.category, 32, mCategory.toLocal8Bit().data(), 32);
    strncpy_s((char*)data.description, 64, mDescription.toLocal8Bit().data(), 64);

    return data;
}

jTransaction::~jTransaction()
{
}

jTransaction::sData::sData()
{
    year = 0;
    month = 0;
    day = 0;
    memset(description, 0, 64);
    memset(category, 0, 32);
    amount = 0;
}
