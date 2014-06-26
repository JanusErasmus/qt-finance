
#include "jbudget.h"

jBudget::jBudget(QString fileName)
{
    bool fileExisted = false;
    mBudget = new QFile(fileName);

    if(mBudget->exists())
    {
        qDebug("File exists");
        fileExisted = true;
    }

    if(mBudget->open(QIODevice::ReadWrite))
        mOpened = true;

    if(fileExisted)
        readBudget();
}

void jBudget::readBudget()
{
    if(!mOpened)
        return;

    mBudget->seek(0);

    //first 4 bytes is the version (qint32)
    qint32 version = 0;
    mBudget->read((char*)&version, 4);
    qDebug("Budget File version 0x%08X", version);

    //next 4 bytes lenght of the list
    quint32 length = 0;
    mBudget->read((char*)&length, 4);
    qDebug("List size %d", length);

    jTransaction * entry = new jTransaction();
    for(quint32 k = 0; k < length; k++)
    {
        jTransaction::sEntry * data = entry->getEntry();
        mBudget->read((char*)data, sizeof(jTransaction::sEntry));
        entry->debugShow();
    }
}

jTransactionList * jBudget::getTransactionList()
{
    return mTransList;
}


bool jBudget::setTransactionList(jTransactionList * list)
{
    if(!mOpened)
        return false;

    //first 4 bytes is the version (qint32)
    qint32 version = VERSION;
    mBudget->write((const char*)&version, 4);

    //next 4 bytes lenght of the list
    quint32 length = list->size();
    qDebug("List size %d", length);
    mBudget->write((const char*)&length, 4);

    //now write all the transactions
    jTransaction * entry;
    for(quint32 k = 0; k < length; k++)
    {
        entry = list->at(k);
        entry->debugShow();
        jTransaction::sEntry * data = entry->getEntry();
        mBudget->write((const char*)data, sizeof(jTransaction::sEntry));
    }

    //end with 16 bytes of zeros
    quint8 zeros[16];
    memset(zeros,0,16);
    mBudget->write((const char*)zeros, 16);

    return true;
}

jBudget::~jBudget()
{
    if(mOpened)
        mBudget->close();
}
