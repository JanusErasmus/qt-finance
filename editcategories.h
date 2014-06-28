#ifndef EDITCATEGORIES_H
#define EDITCATEGORIES_H
#include <QDialog>
#include <QModelIndex>

#include "jcategory.h"

namespace Ui {
class editCategories;
}

class editCategories : public QDialog
{
    Q_OBJECT

    Ui::editCategories *ui;

    QList<jCategory*> mCategories;

    void fillTree();
    QString getCategory(jCategory * cat);
    QString getCategory(jCategory::sCategory * cat);

public:
    explicit editCategories(QList<jCategory *> cats, QWidget *parent = 0);
    ~editCategories();

public slots:
    void selectCategory(QModelIndex idx);
    void applyEdit();
};

#endif // EDITCATEGORIES_H
