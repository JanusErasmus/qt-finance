#include <QDebug>

#include "editcategories.h"
#include "ui_editcategories.h"


editCategories::editCategories(jBudget * budget, QWidget *parent) : QDialog(parent),
                                                                          ui(new Ui::editCategories),
                                                                          mBudget(budget)


{
    ui->setupUi(this);
    setWindowTitle("Edit Categories");
   // mCategories = budget->getCategories();

    ui->incomeEdit->setText(QString::number(mBudget->getIncome()));
    fillTree();
}

void editCategories::fillTree()
{
    float sum = 0;
    ui->categoryTree->reset();

    //setup default values of add transaction window
    QStandardItemModel * model = new QStandardItemModel( mBudget->getCategories().size(), 2 );
    jCategory * cat;
    int r = 0;
    foreach(cat, mBudget->getCategories())
    {
        QStandardItem * item = new QStandardItem(cat->getHeading());
        model->setItem(r, 0, item);

        if(!cat->getCategories().size())
        {
            QLocale sar(QLocale::English, QLocale::SouthAfrica);
            QString amountStr = sar.toCurrencyString(cat->getAmount());
            QStandardItem * amount = new QStandardItem(amountStr);
            amount->setSelectable(false);
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
        QStandardItem *newItem = new QStandardItem( "*" );
        item->appendRow( newItem );

        r++;
    }
    QStandardItem * newItem = new QStandardItem("*");
    model->setItem(r, 0, newItem);

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
     amount->setSelectable(false);
     lst.append(amount);

     return lst;
}

void editCategories::selectCategory(QModelIndex idx)
{
    if(idx.column() == 1)
      {
        ui->nameEdit->setText("");
        ui->nameEdit->setEnabled(0);
        ui->amountEdit->setText("");
        ui->amountEdit->setEnabled(0);
        ui->applyButton->setEnabled(0);
        ui->deleteButton->setEnabled(0);
        return;
    }

    QModelIndex p = idx.parent();
    if(p.isValid())
    {
        jCategory * cat = mBudget->getCategories().at(p.row());

        if(idx.row() >= cat->getCategories().size())
        {
            ui->nameEdit->setEnabled(1);
            ui->amountEdit->setEnabled(1);
            ui->nameEdit->setText("");
            ui->amountEdit->setText("");
            ui->applyButton->setEnabled(1);
            ui->applyButton->setText("Add");
            ui->deleteButton->setEnabled(0);
            return;
        }
        //qDebug() << "Selected: cat"<< p.row() << "subCat" << idx.row();

        jCategory::sCategory * subCat = cat->getCategories().at(idx.row());
        //qDebug() << cat->getHeading() << subCat.name;

        ui->nameEdit->setEnabled(1);
        ui->amountEdit->setEnabled(1);
        ui->nameEdit->setText(subCat->name);
        ui->amountEdit->setText(QString::number(subCat->amount));
        ui->applyButton->setEnabled(1);
        ui->applyButton->setText("Apply");
        ui->deleteButton->setEnabled(1);
    }
    else
    {
        if(idx.row() >= mBudget->getCategories().size())
        {
            ui->nameEdit->setEnabled(1);
            ui->amountEdit->setEnabled(1);
            ui->nameEdit->setText("");
            ui->amountEdit->setText("");
            ui->applyButton->setEnabled(1);
            ui->applyButton->setText("Add");
            ui->deleteButton->setEnabled(0);
            return;
        }

        //qDebug() << "Selected: cat" << idx.row();
        jCategory * cat = mBudget->getCategories().at(idx.row());
        if(!cat->size())
        {
            //qDebug() << cat->getHeading();
            ui->nameEdit->setEnabled(1);
            ui->amountEdit->setEnabled(1);
            ui->nameEdit->setText(cat->getHeading());
            ui->amountEdit->setText(QString::number(cat->getAmount()));
            ui->applyButton->setEnabled(1);
            ui->applyButton->setText("Apply");
            ui->deleteButton->setEnabled(1);
        }
        else
        {
            ui->nameEdit->setText("");
            ui->nameEdit->setEnabled(0);
            ui->amountEdit->setText("");
            ui->amountEdit->setEnabled(0);
            ui->applyButton->setEnabled(0);
            ui->deleteButton->setEnabled(0);
        }
    }
}

void editCategories::applyEdit()
{    
    //qDebug("Apply");
    bool updateFlag = false;
    QStandardItemModel * model  = (QStandardItemModel*)ui->categoryTree->model();    
    QStandardItem * child = model->itemFromIndex(ui->categoryTree->currentIndex());
    QStandardItem * parent = child->parent();

    QString catName;
    QString subName;

    if(parent)
    {
        catName = parent->text();
        subName = child->text();
        qDebug() << "cat:" << catName << "subcat: " << subName;

        jCategory * cat;
        foreach(cat, mBudget->getCategories())
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
                        updateFlag = true;
                    }
                }
            }
        }
    }
    else
    {
        catName = child->text();
        qDebug() << "cat:" << catName;

        jCategory * cat;
        foreach(cat, mBudget->getCategories())
        {
            if(cat->getHeading() == catName)
            {
                cat->setHeading( ui->nameEdit->text());
                cat->setAmount(ui->amountEdit->text().toFloat());
                updateFlag = true;
            }
        }
    }

    if(!updateFlag)
    {
        if(subName.isEmpty())
         {
            qDebug()  << "Add " << ui->nameEdit->text() << ui->amountEdit->text();
            mBudget->addCategory(ui->nameEdit->text(),  ui->amountEdit->text().toFloat());
        }
        else
        {
            qDebug()  << "Append to " << catName  << ui->nameEdit->text() << ui->amountEdit->text();
            jCategory * cat;
            foreach(cat, mBudget->getCategories())
            {
                if(cat->getHeading() == catName)
                {
                    cat->addSubCategory(ui->nameEdit->text(), ui->amountEdit->text().toFloat());
                }
            }
        }

    }

    ui->nameEdit->setText("");
    ui->nameEdit->setEnabled(0);
    ui->amountEdit->setText("");
    ui->amountEdit->setEnabled(0);
     ui->applyButton->setEnabled(0);

    fillTree();
}


void editCategories::deleteCatagory()
{
    qDebug() << "Delete";
    QStandardItemModel * model  = (QStandardItemModel*)ui->categoryTree->model();
    QStandardItem * child = model->itemFromIndex(ui->categoryTree->currentIndex());
    QStandardItem * parent = child->parent();

    QString catName;
    QString subName;

    if(parent)
    {
        catName = parent->text();
        subName = child->text();
        qDebug() << "cat:" << catName << "subcat: " << subName;
        jCategory * cat;
        foreach(cat, mBudget->getCategories())
        {
            if(cat->getHeading() == catName)
            {
                cat->removeSubCategory(subName);
            }
        }
    }
    else
    {
        catName = child->text();
        qDebug() << "cat:" << catName;
        mBudget->removeCategory(catName);
    }

    fillTree();
}

editCategories::~editCategories()
{
    delete ui;
}
