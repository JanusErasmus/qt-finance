#include <QStandardItemModel>

#include "editcategories.h"
#include "ui_editcategories.h"


editCategories::editCategories(QList<jCategory*> cats, QWidget *parent) : QDialog(parent),
    ui(new Ui::editCategories)
{
    ui->setupUi(this);
    setWindowTitle("Edit Categories");

    //setup default values of add transaction window
    QStandardItemModel * model = new QStandardItemModel( cats.size(), 1 );
    jCategory * cat;
    int r = 0;
    foreach(cat, cats)
    {
        QStandardItem *item = new QStandardItem( QString(cat->getHeading()));

        QList<jCategory::sCategory> subCats = cat->getCategories();
        jCategory::sCategory subCat;
        int i = 0;
        foreach(subCat, subCats)
        {
            QStandardItem *child = new QStandardItem( QString(subCat.name));
            child->setEditable( false );
            item->appendRow( child );

            i++;
        }

        model->setItem(r, 0, item);

        r++;
    }

    model->setHorizontalHeaderItem( 0, new QStandardItem( "Categroies" ) );

    ui->categoryTree->setModel( model );
}

editCategories::~editCategories()
{
    delete ui;
}
