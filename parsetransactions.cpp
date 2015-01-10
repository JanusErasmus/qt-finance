#include <QFile>
#include <QDebug>
#include <QRegExp>
#include <QStringList>

#include "parsetransactions.h"

parseTransactions::parseTransactions(QString fileName)
{
    QFile f(fileName);
    if(!f.exists())
    {
        qDebug() << "File does not exist";
        return;
    }

    if(!f.open(QFile::ReadOnly))
    {
        qDebug() << "Could not open file";
        return;
    }

    while(!f.atEnd())
    {
        QString line = f.readLine();
        //qDebug() << line;

        QRegExp rx("(\\d{4})(\\d{2})(\\d{2}),(.+),(-?\\d+.\\d+),(-?\\d+.\\d+)");
        rx.indexIn(line);

        int year, month, day;
        QString desc;
        float amount;
        float bank;

        QStringList lst = rx.capturedTexts();
        if(lst.length())
        {
            int idx = 0;
            QString str;
            foreach (str, lst)
            {
                switch(idx++)
                {
                case 1:
                {
                    year = str.toInt();
                }
                 break;

                case 2:
                {
                    month = str.toInt();
                }
                    break;

                case 3:
                {
                    day = str.toInt();
                }
                    break;

                case 4:
                {
                    desc = str;
                }
                    break;

                case 5:
                {
                    amount = str.toDouble();
                }
                    break;

                case 6:
                {
                    bank = str.toDouble();
                }
                    break;


                default:
                    break;
                }


            }

            pTransaction * trans = new pTransaction(new jTransaction(QDate(year, month, day), "", desc, amount), bank);
            mTransactions.append(trans);
        }
    }

    f.close();
}

parseTransactions::pTransaction::pTransaction(jTransaction * t, float bank)
{
    mTrans = t;
    mBank = bank;
}
