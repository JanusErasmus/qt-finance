#include <QStandardItemModel>
#include <QDebug>

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
        QStandardItem *item = new QStandardItem( getCategory(cat));

        QList<jCategory::sCategory> subCats = cat->getCategories();
        jCategory::sCategory subCat;
        int i = 0;
        foreach(subCat, subCats)
        {
            QStandardItem *child = new QStandardItem( getCategory(subCat));
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

QString editCategories::getCategory(jCategory * cat)
{
     QLocale sar(QLocale::English, QLocale::SouthAfrica);
    //QList<jCategory::sCategory> subCats = cat->getCategories();

    return QString(cat->getHeading()) + " [" + sar.toCurrencyString(cat->getAmount()) +"]";
}


QString editCategories::getCategory(jCategory::sCategory cat)
{
     QLocale sar(QLocale::English, QLocale::SouthAfrica);

    return QString(cat.name) + " [" + sar.toCurrencyString(cat.amount) +"]";
}

editCategories::~editCategories()
{
    delete ui;
}
