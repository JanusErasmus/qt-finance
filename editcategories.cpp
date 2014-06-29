#include <QDebug>

#include "editcategories.h"
#include "ui_editcategories.h"


editCategories::editCategories(jBudget * budget, QWidget *parent) : QDialog(parent),
                                                                          ui(new Ui::editCategories),
                                                                          mBudget(budget),
                                                                          mCategories(budget->getCategories())

{
    ui->setupUi(this);
    setWindowTitle("Edit Categories");

    ui->incomeEdit->setText(QString::number(mBudget->getIncome()));
    fillTree();
}

void editCategories::fillTree()
{
    float sum = 0;
    ui->categoryTree->reset();

    //setup default values of add transaction window
    QStandardItemModel * model = new QStandardItemModel( mCategories.size(), 2 );
    jCategory * cat;
    int r = 0;
    foreach(cat, mCategories)
    {
        QStandardItem * item = new QStandardItem(cat->getHeading());
        model->setItem(r, 0, item);

        if(!cat->getCategories().size())
        {
            QLocale sar(QLocale::English, QLocale::SouthAfrica);
            QString amountStr = sar.toCurrencyString(cat->getAmount());
            QStandardItem * amount = new QStandardItem(amountStr);
            model->setItem(r, 1, amount);

            sum += cat->getAmount();
        }

        QList<jCategory::sCategory*> subCats = cat->getCategories();
        jCategory::sCategory * subCat;
        int i = 0;
        foreach(subCat, subCats)
        {
            QList<QStandardItem*> lst = fillCategory(subCat);
            item->appendRow( lst );

            sum += subCat->amount;

            i++;
        }



        r++;
    }

    model->setHorizontalHeaderItem( 0, new QStandardItem( "Categories" ) );
    model->setHorizontalHeaderItem( 1, new QStandardItem( "Amount" ) );

    ui->categoryTree->setModel( model );
    ui->categoryTree->expandAll();

    QLocale sar(QLocale::English, QLocale::SouthAfrica);

    float income = ui->incomeEdit->text().toFloat();
    mBudget->setIncome(income);
    float diff = income - sum;

    ui->differenceValue->setText(sar.toCurrencyString(diff));

}

QList<QStandardItem*> editCategories::fillCategory(jCategory::sCategory * cat)
{
    QList<QStandardItem*> lst;
     QLocale sar(QLocale::English, QLocale::SouthAfrica);

     QString nameStr = QString(cat->name);
     QString amountStr = sar.toCurrencyString(cat->amount);

     QStandardItem *name = new QStandardItem( nameStr );
     name->setEditable( false );
     lst.append(name);
     QStandardItem *amount = new QStandardItem( amountStr );
     amount->setEditable( false );
     lst.append(amount);

     return lst;
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
        ui->applyButton->setEnabled(1);
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
            ui->applyButton->setEnabled(1);
        }
        else
        {
            ui->nameEdit->setText("");
            ui->nameEdit->setEnabled(0);
            ui->amountEdit->setText("");
            ui->amountEdit->setEnabled(0);
            ui->applyButton->setEnabled(0);
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
