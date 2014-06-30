#include <QDebug>
#include <QFileDialog>
#include <QDateEdit>
#include <QStandardItemModel>


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "editCategories.h"
#include "jtransactionlist.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{    
    mBudget = 0;
    mMainCombo = 0;
    mSubCombo = 0;

    ui->setupUi(this);
    ui->transactionTable->setColumnWidth(0,100);
    ui->transactionTable->setColumnWidth(2,284);
    ui->transactionTable->horizontalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    ui->transactionTable->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);

    setWindowTitle("qFinance");

    openBudget("june.jbud");

}

void MainWindow::openBudget(QString filename)
{
    qDebug() << "Opening" << filename;

    if(mBudget)
    {
        delete mBudget;
        ui->transactionTable->clear();
    }

    mBudget = new jBudget(filename);

    //fill transaction list
    jTransactionList * lst = mBudget->getTransactionList();
    lst->fillTable(ui->transactionTable);
    ui->transactionTable->scrollToBottom();
    insertNewEntryRow();

    //fill summary tree
    fillTree();
}

void MainWindow::insertNewEntryRow()
{
    int row = ui->transactionTable->rowCount();
    ui->transactionTable->insertRow(row);

    QTableWidgetItem * item = new QTableWidgetItem();
    item->setData(Qt::EditRole, QDate::currentDate());
    ui->transactionTable->setItem(row,0, item);

    if(mMainCombo)
        delete mMainCombo;

    mMainCombo = new QComboBox();
    connect(mMainCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateSubCombo(QString)));
    fillCombo(mMainCombo);
    ui->transactionTable->setCellWidget(row,1,mMainCombo);

    connect(ui->transactionTable, SIGNAL(cellChanged(int,int)), this, SLOT(tableTransChange(int,int)));
}

void MainWindow::fillCombo(QComboBox * main)
{
    jCategory * cat;
    foreach(cat, mBudget->getCategories())
    {
        main->addItem(cat->getHeading());
    }
}

void MainWindow::fillCombo(QComboBox * sub, jCategory * cat)
{
    if(!sub)
        return;

    jCategory::sCategory * subCat;
    foreach(subCat, cat->getCategories())
    {
        sub->addItem(subCat->name);
    }
}

void MainWindow::updateSubCombo(QString currSelection)
{
    int row = ui->transactionTable->rowCount() - 1;
    //qDebug() << currSelection;


    jCategory * cat = mBudget->getCategory(currSelection);
    if(cat && cat->getCategories().size())
    {
         mSubCombo = new QComboBox();
         fillCombo(mSubCombo, cat);
         ui->transactionTable->setCellWidget(row,2,mSubCombo);
    }
    else
    {
        ui->transactionTable->removeCellWidget(row,2);
        QTableWidgetItem * item = new QTableWidgetItem(QString(""));
        ui->transactionTable->setItem(row,2, item);
    }
}


void MainWindow::tableTransChange(int row, int col)
{
    //qDebug() << "Changed:" << row << col;

    int lastRow = ui->transactionTable->rowCount() - 1;
    if(row == lastRow && col == 3)
    {
        //qDebug("Add transaction");

        QTableWidgetItem* theItem = ui->transactionTable->item(row, 0);

        QDate date = QDate::fromString(theItem->text(), "yyyy-MM-dd");
        //qDebug() << theItem->text() << date;

        QComboBox * box = (QComboBox *)ui->transactionTable->cellWidget(row, 1);
        QString category = box->currentText();
        //qDebug() << category;

        QString description;
        box = (QComboBox *)ui->transactionTable->cellWidget(row, 2);
        if(box)
        {
            description = box->currentText();
        }
        else
        {
            theItem = ui->transactionTable->item(row, 2);
            description = theItem->text();
        }


        //qDebug() << description;

        theItem = ui->transactionTable->item(row, 3);
        double amount = theItem->text().toFloat();
        //qDebug() << amount;



        jTransactionList * lst = mBudget->getTransactionList();
        jTransaction * entry = new jTransaction(date, category, description, amount);
        lst->append(entry);

        disconnect(ui->transactionTable, SIGNAL(cellChanged(int,int)), this, SLOT(tableTransChange(int,int)));

        ui->transactionTable->removeCellWidget(row,0);
        ui->transactionTable->removeCellWidget(row,1);
        ui->transactionTable->removeCellWidget(row,2);
        ui->transactionTable->removeCellWidget(row,3);

        entry->setRow(ui->transactionTable, row);

        insertNewEntryRow();
    }
}

void MainWindow::editCategoryWindow()
{
    editCategories catUi(mBudget, this);

    //read transaction values if OK
    if(catUi.exec() == QDialog::Accepted)
    {

    }
}

void MainWindow::openBudget()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),".",tr("jBudget Files (*.jbud)"));
    if(!fileName.isEmpty())
    {
        openBudget(fileName);
    }
}

 void MainWindow::saveBudget()
 {
     if(mBudget->save())
         qDebug("File was written");
 }

 void MainWindow::fillTree()
 {
     ui->summaryTree->reset();

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

             float total = cat->getAmount();
             float sum = mBudget->getTransactionList()->sumTransactions(cat->getHeading());

             QString amountStr = sar.toCurrencyString(total - sum);
             QStandardItem * amount = new QStandardItem(amountStr);
             amount->setSelectable(false);
             model->setItem(r, 1, amount);
         }

         QList<jCategory::sCategory*> subCats = cat->getCategories();
         jCategory::sCategory * subCat;
         int i = 0;
         foreach(subCat, subCats)
         {
             QList<QStandardItem*> lst = fillCategory(cat, subCat);
             item->appendRow( lst );

             i++;
         }

         r++;
     }

     model->setHorizontalHeaderItem( 0, new QStandardItem( "Categories" ) );
     model->setHorizontalHeaderItem( 1, new QStandardItem( "Amount" ) );

     ui->summaryTree->setModel( model );
     ui->summaryTree->expandAll();
 }

 QList<QStandardItem*> MainWindow::fillCategory(jCategory *cat, jCategory::sCategory * subCat)
 {
     QList<QStandardItem*> lst;
      QLocale sar(QLocale::English, QLocale::SouthAfrica);

      QString nameStr = QString(subCat->name);

      float total = subCat->amount;
      float sum = mBudget->getTransactionList()->sumTransactions(cat->getHeading(), subCat->name);
      QString amountStr = sar.toCurrencyString(total - sum);

      QStandardItem *name = new QStandardItem( nameStr );
      name->setEditable( false );
      lst.append(name);
      QStandardItem *amount = new QStandardItem( amountStr );
      amount->setEditable( false );
      amount->setSelectable(false);
      lst.append(amount);

      return lst;
 }

MainWindow::~MainWindow()
{
    if(mBudget)
        delete mBudget;

    delete ui;
}
