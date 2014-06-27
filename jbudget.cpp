#include <QDebug>

#include "jbudget.h"

jBudget::jBudget(QString fileName)
{
    mBudgetFile = new QFile(fileName);
    mTransList = new jTransactionList();

//    jCategory * c = new jCategory("Sakgeld", 1000);
//    c->addSubCategory("Koeldrank", 1000);
//    c->addSubCategory("Snoepie", 1000);
//    c->addSubCategory("Bier", 1000);
//    mCategories.append(c);

//    c = new jCategory("Spaar", 2);
//    c->addSubCategory("TV", 1000);
//    c->addSubCategory("Kar", 1000);
//    mCategories.append(c);

//    c = new jCategory("Bonus", 1000);
//    mCategories.append(c);

    if(mBudgetFile->exists())
    {
        qDebug("File exists");
        readBudget();
    }

    qDebug() << "Total " << mTransList->sumTransactions();
    jCategory * c;
    foreach(c, mCategories)
    {
        float total = c->getAmount();
        float sum = mTransList->sumTransactions(c->getHeading());
        qDebug() << c->getHeading() << "[" << total <<"]"  << sum << " = " << total - sum;

       QList<jCategory::sCategory> clst = c->getCategories();
       jCategory::sCategory sc;
       foreach(sc, clst)
       {
           total = sc.amount;
           sum = mTransList->sumTransactions(c->getHeading(), sc.name);
           qDebug() << " - " << sc.name << "[" << total <<"]"  << sum << " = " << total - sum;
       }
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

    readTransactions();
    readCategories();


    mBudgetFile->close();
}

jTransactionList * jBudget::getTransactionList()
{
    return mTransList;
}

jCategory* jBudget::getCategory(QString heading)
{
    jCategory * cat;
    foreach(cat, mCategories)
    {
        if(cat->getHeading() == heading)
            return cat;
    }

    return 0;
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
    //qDebug("List size %d", length);
    mBudgetFile->write((const char*)&length, 4);

    //now write all the transactions
    jTransaction * entry;
    for(quint32 k = 0; k < length; k++)
    {
        entry = mTransList->at(k);

        //entry->debugShow();
        jTransaction::sData data = entry->getData();
        mBudgetFile->write((const char*)&data, sizeof(jTransaction::sData));
    }

    //write the categories
    writeCategories();

    mBudgetFile->close();

    return true;
}

void jBudget::readTransactions()
{
    //next 4 bytes lenght of the list
    quint32 length = 0;
    mBudgetFile->read((char*)&length, 4);
    //qDebug("List size %d", length);

    for(quint32 k = 0; k < length; k++)
    {
        jTransaction::sData data;
        mBudgetFile->read((char*)&data, sizeof(jTransaction::sData));

        jTransaction * trans = new jTransaction(data);
        //trans->debugShow();
        mTransList->append(trans);
    }
}

void jBudget::writeCategories()
{
    jCategory * t;
    foreach(t, mCategories)
    {
        //first 4 bytes is the amount of categories
        quint32 length = t->size();
        //qDebug("%d Categories", length);
        mBudgetFile->write((const char*)&length, 4);

        //The next 32 bytes is the category heading        
        mBudgetFile->write((const char*)(t->getHeading().toLocal8Bit().data()), 32);

        //The next 4 bytes is the amount of this category
        float amount = t->getAmount();
        mBudgetFile->write((const char*)&amount, 4);

        //The following 32 byte packets is the categories
        for(quint32 k = 0; k < length; k++)
        {
            jCategory::sData data;
             if(t->getCategory(k, data))
                 mBudgetFile->write((const char*)&data, sizeof(jCategory::sData));
        }
    }
}

void jBudget::readCategories()
{
     char string[32];

     while(!mBudgetFile->atEnd())
     {
         //next 4 bytes lenght of the list
         quint32 length = 0;
         mBudgetFile->read((char*)&length, 4);
         //qDebug("%d Categories", length);

         mBudgetFile->read(string, 32);
         //qDebug("Heading %s", string);

         float amount = 0;
         mBudgetFile->read((char*)&amount, 4);
         //qDebug() << amount;

         jCategory * t = new jCategory(string, amount);

         for(quint32 k = 0; k < length; k++)
         {
             jCategory::sData data;
             mBudgetFile->read((char*)&data, sizeof(jCategory::sData));
             //qDebug(" - %s %f", data.category, data.amount);

             t->addSubCategory(QString((char*)data.category), data.amount);
         }

         mCategories.append(t);
     }
}

jBudget::~jBudget()
{
    delete mTransList;
}

