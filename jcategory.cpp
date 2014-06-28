#include "jcategory.h"

jCategory::jCategory()
{
}

jCategory::jCategory(QString heading, float amount) : mHeading(heading), mAmount(amount)
{
}

void jCategory::addSubCategory(QString name, float amount)//sCategory subCat)
{
    sCategory subCat;
    subCat.name = name;
    subCat.amount = amount;
    mCategories.append(subCat);
}

float jCategory::getAmount()
{
    float sum = 0;
    int size = mCategories.size();
    if(size)
    {
        sCategory cat;
        foreach(cat, mCategories)
        {
            sum += cat.amount;
        }

        return sum;
    }

    return mAmount;
}

bool jCategory::getCategory(int idx, jCategory::sData &data)
{    
    if(idx >= mCategories.size())
        return false;

    data.amount = mCategories.at(idx).amount;
    strncpy((char*)data.category, mCategories.at(idx).name.toLocal8Bit().data(), 32);

    return true;
}

jCategory::sData::sData()
{
    memset(category, 0, 32);
    amount = 0;
}

