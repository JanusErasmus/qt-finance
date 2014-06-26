
#include "jbudget.h"

jBudget::jBudget(QString fileName)
{
    mBudgetFile = new QFile(fileName);

    mTransList = new jTransactionList();

    if(mBudgetFile->exists())
    {
        qDebug("File exists");
        readBudget();
    }
}

void jBudget::readBudget()
{
    if(!mBudgetFile->open(QIODevice::ReadWrite))
        return;

    mBudgetFile->seek(0);

    //first 4 bytes is the version (qint32)
    qint32 version = 0;
    mBudgetFile->read((char*)&version, 4);
    qDebug("Budget File version 0x%08X", version);

    //next 4 bytes lenght of the list
    quint32 length = 0;
    mBudgetFile->read((char*)&length, 4);
    qDebug("List size %d", length);

    for(quint32 k = 0; k < length; k++)
    {
        jTransaction::sData data;
        mBudgetFile->read((char*)&data, sizeof(jTransaction::sData));

        jTransaction * trans = new jTransaction(data);
        //trans->debugShow();
        mTransList->append(trans);
    }

    mBudgetFile->close();
}

jTransactionList * jBudget::getTransactionList()
{
    return mTransList;
}


bool jBudget::save()
{
    if(!mBudgetFile->open(QIODevice::ReadWrite))
        return false;

    mBudgetFile->seek(0);

    //first 4 bytes is the version (qint32)
    qint32 version = VERSION;
    mBudgetFile->write((const char*)&version, 4);

    //next 4 bytes lenght of the list
    quint32 length = mTransList->size();
    qDebug("List size %d", length);
    mBudgetFile->write((const char*)&length, 4);

    //now write all the transactions
    jTransaction * entry;
    for(quint32 k = 0; k < length; k++)
    {
        entry = mTransList->at(k);
        //entry->debugShow();
        jTransaction::sData * data = entry->getData();
        mBudgetFile->write((const char*)data, sizeof(jTransaction::sData));
    }

    //end with 16 bytes of zeros
    quint8 zeros[16];
    memset(zeros,0,16);
    mBudgetFile->write((const char*)zeros, 16);

    mBudgetFile->close();

    return true;
}

jBudget::~jBudget()
{
    delete mTransList;
}
