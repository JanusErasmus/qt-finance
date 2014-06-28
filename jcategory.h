#ifndef JCATEGORY_H
#define JCATEGORY_H
#include <QString>
#include <QList>

class jCategory
{
public:
#ifdef _MSC_VER
#pragma pack(push,1)
        struct sData
        {
            unsigned char category[32];
            float amount;
            sData();
        };
#pragma pack(pop)
#elif defined(__GNUC__)
    struct sData
    {
        unsigned char category[32];
        float amount;

        sData();
    }__attribute__((packed));
#endif

    struct sCategory
    {
        QString name;
        float amount;
    };

private:

    QString mHeading;
    float mAmount;
    QList<sCategory> mCategories;

public:
    jCategory();
    jCategory(QString heading, float amount);

    QString getHeading(){ return mHeading; }
    float getAmount();

    void addSubCategory(QString name, float amount);//sCategory subCat);


    int size(){ return mCategories.size(); }

    QList<sCategory> getCategories(){ return mCategories; }

     bool getCategory(int idx, sData &data);
};

#endif // JCATEGORY_H
