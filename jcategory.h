#ifndef JCATEGORY_H
#define JCATEGORY_H
#include <QString>
#include <QList>

class jCategory
{
    QString mHeading;
    QList<QString> mCategories;

public:
    jCategory();
    jCategory(QString heading);

    void addSubCategory(QString subCat);

    int size(){ return mCategories.size(); }
    QString getHeading(){ return mHeading; }

    QList<QString> getCategories(){ return mCategories; }

    bool fillHeading(char * ptr, int len);
    bool fillCategory(int idx, char * ptr, int len);
};

#endif // JCATEGORY_H
