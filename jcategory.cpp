#include "jcategory.h"

jCategory::jCategory()
{
}

jCategory::jCategory(QString heading) : mHeading(heading)
{
}

void jCategory::addSubCategory(QString subCat)
{
    mCategories.append(subCat);
}

bool jCategory::fillHeading(char * ptr, int len)
{
    if(len < 32)
        return false;

    strncpy(ptr, mHeading.toLocal8Bit().data(), 32);

    return true;
}

bool jCategory::fillCategory(int idx, char * ptr, int len)
{
    if(len < 32)
        return false;

    if(idx >= mCategories.size())
        return false;

    strncpy(ptr, mCategories.at(idx).toLocal8Bit().data(), 32);

    return true;
}


