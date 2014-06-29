#ifndef EDITCATEGORIES_H
#define EDITCATEGORIES_H
#include <QDialog>
#include <QModelIndex>
#include <QStandardItemModel>

#include "jBudget.h"

namespace Ui {
class editCategories;
}

class editCategories : public QDialog
{
    Q_OBJECT

    Ui::editCategories *ui;

    jBudget * mBudget;
   // QList<jCategory*> mCategories;

    void fillTree();    
    QList<QStandardItem*> fillCategory(jCategory::sCategory * cat);

public:
    explicit editCategories(jBudget *budget, QWidget *parent = 0);
    ~editCategories();

public slots:
    void selectCategory(QModelIndex idx);
    void applyEdit();    
    void deleteCatagory();
};

#endif // EDITCATEGORIES_H
