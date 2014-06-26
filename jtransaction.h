#ifndef JTRANSACTION_H
#define JTRANSACTION_H
#include <QTableWidgetItem>
#include <QDate>


class jTransaction
{
public:
#ifdef _MSC_VER
#pragma pack(push,1)
        struct sData
        {
            quint16 year;
            quint16 month;
            quint16 day;
            unsigned char description[64];
            unsigned char category[32];
            float amount;

            sData();
        };
#pragma pack(pop)
#elif defined(__GNUC__)
    struct sData
    {
        quint16 year;
        quint16 month;
        quint16 day;
        unsigned char description[64];
        unsigned char category[32];
        float amount;

        sData();
    }__attribute__((packed));
#endif
private:
    QDate mDate;
    QString mCategory;
    QString mDescription;
    float mAmount;

public:
    jTransaction();
    jTransaction(sData data);
    jTransaction(QDate date, QString category, QString description, double amount);
    ~jTransaction();


    sData getData();
    void debugShow(sData data);

    void setRow(QTableWidget* table, int row);
};

#endif // JTRANSACTION_H
