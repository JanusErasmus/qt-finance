#ifndef JTRANSACTION_H
#define JTRANSACTION_H
#include <QTableWidgetItem>
#include <QDate>

#ifdef _MSC_VER
#define PACKED_STRUCT(__struct__) #pragma pack(push,1) __struct__; #pragma pack(pop)
#elif defined(__GNUC__)
#  define PACKED_STRUCT(__struct__) __struct__ __attribute__((packed));
#endif

class jTransaction
{
public:
PACKED_STRUCT(
        struct sData
        {
            quint16 year;
            quint16 month;
            quint16 day;
            unsigned char description[64];
            float amount;

            sData();
        }
)

private:
    sData mData;

    QTableWidgetItem * mDateTableItem;
    QTableWidgetItem * mDescTableItem;
    QTableWidgetItem * mAmountTableItem;

public:
    jTransaction();
    jTransaction(sData data);
    jTransaction(QDate date, QString description, double amount);
    ~jTransaction();


    sData * getData(){ return &mData; }
    void debugShow();

    void setRow(QTableWidget* table, int row);
};

#endif // JTRANSACTION_H
