#include <QDebug>
#include "jcategory.h"

#include "addtransactions.h"
#include "ui_addtransactions.h"

addTransactions::addTransactions(jBudget *budget, QString fileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addTransactions),
    mBudget(budget)
{


    ui->setupUi(this);
    setWindowTitle("Add Transactions");

    ui->descBox->hide();

    parseTransactions trans(fileName);
    mTransactions =  trans.getList();
    pTransIdx = mTransactions.length() - 1;

    jTransaction * tr = mTransactions.at(0)->mTrans;
    ui->p_desc->setText(tr->getDescription());
    ui->AmountEdit->setText(QString::number(round(tr->getAmount())));
    
    if(mBudget)
    {
        //fill category drop down
        jCategory * cat;
        foreach(cat, mBudget->getCategories())
            ui->categoryBox->addItem(cat->getHeading());
    }

    nextPtrans(mTransactions.at(pTransIdx)->mTrans);
}

void addTransactions::handleDescription(jCategory * cat)
{
    if(cat->getCategories().length())
    {
        ui->descBox->clear();

        jCategory::sCategory * subCat;
        foreach(subCat,cat->getCategories())
            ui->descBox->addItem(subCat->name);

        ui->descBox->show();
        ui->DescEdit->hide();
    }
    else
    {
        ui->DescEdit->show();
        ui->descBox->hide();
    }
}

void addTransactions::okPressed()
{
    QString category = ui->categoryBox->currentText();
    QString description = ui->descBox->currentText();
    if(ui->descBox->isHidden())
            description = ui->DescEdit->text();
    float amount = round(ui->AmountEdit->text().toFloat());

    jTransactionList * lst = mBudget->getTransactionList();
    jTransaction * entry = new jTransaction( QDate::currentDate(), category, description, amount);
    lst->append(entry);

    nextPtrans(mTransactions.at(pTransIdx)->mTrans);
}

void addTransactions::cancelPressed()
{
     nextPtrans(mTransactions.at(pTransIdx)->mTrans);
}

void addTransactions::nextPtrans(jTransaction * tr)
{
    ui->p_desc->setText(tr->getDescription());
    ui->AmountEdit->setText(QString::number(round(-(tr->getAmount()))));


    if(--pTransIdx < 0)
        close();
    else
         emit setAmount( round(mTransactions.at(pTransIdx + 1)->mBank));
}

 float addTransactions::round(float num)
 {
     if(num > 0.0)
         return (int)floorf(num + 0.5);
     else
         return (int)ceilf(num + 0.5);
 }

void addTransactions::catChange(int idx)
{
    if(mBudget)
    {
        handleDescription(mBudget->getCategories().at(idx));
    }
}

addTransactions::~addTransactions()
{
    delete ui;
}
