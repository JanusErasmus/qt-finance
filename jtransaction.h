#ifndef JTRANSACTION_H
#define JTRANSACTION_H
#include <QTableWidgetItem>
#include <QDate>

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#elif defined(__GNUC__)
#  define PACKED_STRUCT(name) struct __attribute__((packed)) name
#endif

class jTransaction
{
public:
#pragma pack(push,1)
    struct sEntry
    {
        unsigned char description[64];
        float amount;

        sEntry();
    };
#pragma pack(pop)

private:
    sEntry mEntry;

    QTableWidgetItem * mDate;
    QTableWidgetItem * mDescription;
    QTableWidgetItem * mAmount;

public:
    jTransaction();
    jTransaction(QDate date, QString description, double amount);
    ~jTransaction();

    sEntry * getEntry(){ return &mEntry; }
    void debugShow();

    void setRow(QTableWidget* table, int row);
};

#endif // JTRANSACTION_H
