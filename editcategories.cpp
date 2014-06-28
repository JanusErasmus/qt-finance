#include <QStandardItemModel>
#include <QDebug>

#include "editcategories.h"
#include "ui_editcategories.h"


editCategories::editCategories(QList<jCategory*> cats, QWidget *parent) : QDialog(parent),
                                                                          ui(new Ui::editCategories),
                                                                          mCategories(cats)
{
    ui->setupUi(this);
    setWindowTitle("Edit Categories");

    fillTree();
}

void editCategories::fillTree()
{
    ui->categoryTree->reset();

    //setup default values of add transaction window
    QStandardItemModel * model = new QStandardItemModel( mCategories.size(), 1 );
    jCategory * cat;
    int r = 0;
    foreach(cat, mCategories)
    {
        QString catName = cat->getHeading();

        if(!cat->getCategories().size())
            catName = getCategory(cat);

        QStandardItem * item = new QStandardItem(catName);

        QList<jCategory::sCategory*> subCats = cat->getCategories();
        jCategory::sCategory * subCat;
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
    ui->categoryTree->expandAll();
}

QString editCategories::getCategory(jCategory * cat)
{
    QLocale sar(QLocale::English, QLocale::SouthAfrica);
    //QList<jCategory::sCategory> subCats = cat->getCategories();

    QString name = QString(cat->getHeading()).leftJustified(10, ' ');
    return name + "[" + sar.toCurrencyString(cat->getAmount()) +"]";
}


QString editCategories::getCategory(jCategory::sCategory * cat)
{
     QLocale sar(QLocale::English, QLocale::SouthAfrica);

     QString name = QString(cat->name).leftJustified(10, ' ');

     return name + " [" + sar.toCurrencyString(cat->amount) +"]";
}

void editCategories::selectCategory(QModelIndex idx)
{
    QModelIndex p = idx.parent();
    if(p.isValid())
    {
        //qDebug() << "Selected: cat"<< p.row() << "subCat" << idx.row();
        jCategory * cat = mCategories.at(p.row());
        jCategory::sCategory * subCat = cat->getCategories().at(idx.row());
        //qDebug() << cat->getHeading() << subCat.name;

        ui->nameEdit->setEnabled(1);
        ui->amountEdit->setEnabled(1);
        ui->nameEdit->setText(subCat->name);
        ui->amountEdit->setText(QString::number(subCat->amount));
    }
    else
    {
        //qDebug() << "Selected: cat" << idx.row();
        jCategory * cat = mCategories.at(idx.row());
        if(!cat->size())
        {
            //qDebug() << cat->getHeading();
            ui->nameEdit->setEnabled(1);
            ui->amountEdit->setEnabled(1);
            ui->nameEdit->setText(cat->getHeading());
            ui->amountEdit->setText(QString::number(cat->getAmount()));
        }
        else
        {
            ui->nameEdit->setText("");
            ui->nameEdit->setEnabled(0);
            ui->amountEdit->setText("");
            ui->amountEdit->setEnabled(0);
        }
    }
}

void editCategories::applyEdit()
{
    //qDebug() << "Name change: " << name;
    QStandardItemModel * model  = (QStandardItemModel*)ui->categoryTree->model();
    QStandardItem * child = model->itemFromIndex(ui->categoryTree->currentIndex());
    QStandardItem * parent = child->parent();

    if(parent)
    {
        //qDebug() << parent->text() << child->text();
        QString catName = parent->text().split(" ").at(0);
        QString subName = child->text().split(" ").at(0);
        qDebug() << catName << subName;

        jCategory * cat;
        foreach(cat, mCategories)
        {
            if(cat->getHeading() == catName)
            {
                jCategory::sCategory * sub;
                foreach(sub, cat->getCategories())
                {
                    if(sub->name == subName)
                    {
                        sub->name = ui->nameEdit->text();
                        sub->amount = ui->amountEdit->text().toFloat();
                    }
                }
            }
        }
    }
    else
     {
        QString catName = child->text().split(" ").at(0);


        jCategory * cat;
        foreach(cat, mCategories)
        {
            if(cat->getHeading() == catName)
            {
                cat->setHeading( ui->nameEdit->text());
                cat->setAmount(ui->amountEdit->text().toFloat());
            }
        }
}
    fillTree();

    //    QLocale sar(QLocale::English, QLocale::SouthAfrica);

    //    float amount = ui->amountEdit->text().toFloat();
    //    qDebug() << amount;
    //   QString text = ui->nameEdit->text() + " [" + sar.toCurrencyString(amount) +"]";
    //    child->setText(text);
}

editCategories::~editCategories()
{
    delete ui;
}
