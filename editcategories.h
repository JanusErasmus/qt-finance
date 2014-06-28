#ifndef EDITCATEGORIES_H
#define EDITCATEGORIES_H
#include <QDialog>

#include "jcategory.h"

namespace Ui {
class editCategories;
}

class editCategories : public QDialog
{
    Q_OBJECT

    Ui::editCategories *ui;

    QString getCategory(jCategory * cat);
    QString getCategory(jCategory::sCategory cat);

public:
    explicit editCategories(QList<jCategory *> cats, QWidget *parent = 0);
    ~editCategories();

};

#endif // EDITCATEGORIES_H
