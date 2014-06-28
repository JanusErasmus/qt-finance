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

public:
    explicit editCategories(QList<jCategory *> cats, QWidget *parent = 0);
    ~editCategories();

private:
    Ui::editCategories *ui;
};

#endif // EDITCATEGORIES_H
