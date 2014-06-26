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

    QLocale sar(QLocale::English, QLocale::SouthAfrica);


    dateTableItem = new QTableWidgetItem();
    dateTableItem->setData(Qt::EditRole, mDate);
    descTableItem = new QTableWidgetItem(mDescription);
    catTableItem = new QTableWidgetItem(mCategory);
    amountTableItem = new QTableWidgetItem(sar.toCurrencyString(mAmount));

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
    strncpy((char*)data.category, mCategory.toLocal8Bit().data(), 32);
    strncpy((char*)data.description, mDescription.toLocal8Bit().data(), 64);

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
    amount = 0;
}
