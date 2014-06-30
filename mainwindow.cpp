#include <QDebug>
#include <QFileDialog>
#include <QDateEdit>
#include <QStandardItemModel>
#include <QPalette>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "editCategories.h"
#include "jtransactionlist.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{    
    mBudget = 0;
    mMainCombo = 0;
    mSubCombo = 0;
    mEditRow = -1;

  //  mStatusLabel = new QLabel();

    ui->setupUi(this);
    ui->transactionTable->setColumnWidth(0,100);
    ui->transactionTable->setColumnWidth(2,284);
    ui->transactionTable->horizontalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    ui->transactionTable->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);

//    ui->diffLabel->setText("");

   // ui->statusBar->addPermanentWidget(mStatusLabel);

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


    ui->bankEdit->setText(QString::number(mBudget->getBank()));

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

    item = new QTableWidgetItem(QString(""));
     ui->transactionTable->setItem(row,3, item);

    mMainCombo = new QComboBox();
    connect(mMainCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateSubCombo(QString)));
    fillCombo(mMainCombo);
    ui->transactionTable->setCellWidget(row,1,mMainCombo);

    connect(ui->transactionTable, SIGNAL(cellChanged(int,int)), this, SLOT(tableTransChange(int,int)));
}

void MainWindow::fillCombo(QComboBox * main)
{
    int k = 0;
    jCategory * cat;
    foreach(cat, mBudget->getCategories())
    {
        float total = cat->getAmount();
        float sum = mBudget->getTransactionList()->sumTransactions(cat->getHeading());
        float value = total - sum;

        main->addItem(cat->getHeading());
        main->setItemData(k, QLocale().toCurrencyString(value), Qt::ToolTipRole);
        k++;
    }
}

void MainWindow::fillCombo(QComboBox * sub, jCategory * cat)
{
    if(!sub)
        return;

    int k = 0;
    jCategory::sCategory * subCat;
    foreach(subCat, cat->getCategories())
    {
        float total = subCat->amount;
        float sum = mBudget->getTransactionList()->sumTransactions(cat->getHeading(), subCat->name);
        float value = total - sum;

        sub->addItem(subCat->name);
        sub->setItemData(k, QLocale().toCurrencyString(value), Qt::ToolTipRole);

        k++;
    }
}

void MainWindow::updateSubCombo(QString currSelection)
{
    int row = ui->transactionTable->rowCount() - 1;

    if(mEditRow >= 0)
        row = mEditRow;
    //qDebug() << currSelection;


    jCategory * cat = mBudget->getCategory(currSelection);
    if(cat && cat->getCategories().size())
    {
        if(mSubCombo)
        {
            delete mSubCombo;
        }

         mSubCombo = new QComboBox();
         connect(mSubCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateLeftLabel(QString)));

         fillCombo(mSubCombo, cat);
         ui->transactionTable->setCellWidget(row,2,mSubCombo);


    }
    else
    {
        if(mSubCombo)
            disconnect(mSubCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateLeftLabel(QString)));

        ui->transactionTable->removeCellWidget(row,2);        
        mSubCombo = 0;

        QTableWidgetItem * item = new QTableWidgetItem(QString(""));
        ui->transactionTable->setItem(row,2, item);

        float total = cat->getAmount();
        float sum = mBudget->getTransactionList()->sumTransactions(cat->getHeading());
        float value = total - sum;

        QString leftStr = cat->getHeading() + ": " + QLocale().toCurrencyString(value);
        ui->leftLabel->setText(leftStr);
    }
}

void MainWindow::updateLeftLabel(QString subString)
{
    int row = ui->transactionTable->rowCount() - 1;
    QComboBox * box = (QComboBox *)ui->transactionTable->cellWidget(row, 1);
    QString category = box->currentText();

    //qDebug() << category << subString;

    float total = -1;

    jCategory * cat;
    foreach(cat, mBudget->getCategories())
    {
        if(cat->getHeading() == category)
        {
            jCategory::sCategory * sCat;
            foreach(sCat, cat->getCategories())
            {
                if(sCat->name == subString)
                {total = sCat->amount;
                    break;
                }
            }
        }
    }

    if(total >= 0)
    {
        float sum = mBudget->getTransactionList()->sumTransactions(category, subString);
        float value = total - sum;

        QString leftStr = category + " - " + subString + ": " + QLocale().toCurrencyString(value);
        ui->leftLabel->setText(leftStr);
    }
    else
    {
        ui->leftLabel->setText("");
    }
}


void MainWindow::tableTransChange(int row, int col)
{
    //qDebug() << "Changed:" << row << col;

    if(mEditRow >= 0)
    {
        if(col == 3)
            applyTransChanges();

        updateBank();

        return;
    }

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

    updateBank();
}

void MainWindow::tableTransDoubleClick(int row, int col)
{
    qDebug() << "EditRow:" << mEditRow << "[" << row << ":" << col << "]";

    if((mEditRow >= 0) && (mEditRow != row))
    {
        applyTransChanges();
        return;
    }

    int lastRow = ui->transactionTable->rowCount() - 1;

    if(row == lastRow)
        return;

    //stop change links
    disconnect(ui->transactionTable, SIGNAL(cellChanged(int,int)), this, SLOT(tableTransChange(int,int)));
    //stop combo links
    disconnect(mMainCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateSubCombo(QString)));

    ui->transactionTable->removeCellWidget(lastRow,0);
    ui->transactionTable->removeCellWidget(lastRow,1);
    ui->transactionTable->removeCellWidget(lastRow,2);
    ui->transactionTable->removeCellWidget(lastRow,3);
    ui->transactionTable->removeRow(lastRow);

    mEditRow = row;
    QTableWidgetItem * item = (QTableWidgetItem *)ui->transactionTable->item(row, 1);
    QString catStr = item->text();

    QString description;
    QComboBox * box = (QComboBox *)ui->transactionTable->cellWidget(row, 2);
    if(box)
    {
        description = box->currentText();
    }
    else
    {
        item = ui->transactionTable->item(row, 2);
        description = item->text();
    }

    item = ui->transactionTable->item(row, 3);
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    ui->transactionTable->removeCellWidget(row,1);
    ui->transactionTable->removeCellWidget(row,2);
    ui->transactionTable->removeCellWidget(row,3);

    if(mMainCombo)
        delete mMainCombo;

    mMainCombo = new QComboBox();
    connect(mMainCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateSubCombo(QString)));
    fillCombo(mMainCombo);
    mMainCombo->setCurrentText(catStr);
    ui->transactionTable->setCellWidget(row,1,mMainCombo);

    if(mSubCombo)
        mSubCombo->setCurrentText(description);
    else
    {
        item = ui->transactionTable->item(row, 2);
        item->setText(description);
    }

    //link changes again
    connect(ui->transactionTable, SIGNAL(cellChanged(int,int)), this, SLOT(tableTransChange(int,int)));
}

void MainWindow::applyTransChanges()
{
    qDebug() << "Save changes";

    QTableWidgetItem* theItem = ui->transactionTable->item(mEditRow, 0);

    QDate date = QDate::fromString(theItem->text(), "yyyy-MM-dd");
    qDebug() << date;

    QComboBox * box = (QComboBox *)ui->transactionTable->cellWidget(mEditRow, 1);
    QString category = box->currentText();
    qDebug() << category;

    QString description;
    box = (QComboBox *)ui->transactionTable->cellWidget(mEditRow, 2);
    if(box)
    {
        description = box->currentText();
    }
    else
    {
        theItem = ui->transactionTable->item(mEditRow, 2);
        description = theItem->text();
    }
    qDebug() << description;

    theItem = ui->transactionTable->item(mEditRow, 3);
    QString amountStr = theItem->text().replace(QLocale().currencySymbol(), "");
    amountStr = amountStr.replace(",", ".");
    amountStr = amountStr.simplified();
    amountStr = amountStr.replace(" ", "");
    float amount = amountStr.toFloat();
    //qDebug() << amount;

    jTransactionList * lst = mBudget->getTransactionList();
    jTransaction * entry = new jTransaction(date, category, description, amount);
    lst->updateEntry(mEditRow, entry);

    disconnect(ui->transactionTable, SIGNAL(cellChanged(int,int)), this, SLOT(tableTransChange(int,int)));

    ui->transactionTable->removeCellWidget(mEditRow,0);
    ui->transactionTable->removeCellWidget(mEditRow,1);
    ui->transactionTable->removeCellWidget(mEditRow,2);
    ui->transactionTable->removeCellWidget(mEditRow,3);
    entry->setRow(ui->transactionTable, mEditRow);

    mEditRow = -1;
    insertNewEntryRow();
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
         item->setToolTip(QLocale().toCurrencyString(cat->getAmount()));
         model->setItem(r, 0, item);

         if(!cat->getCategories().size())
         {
             float total = cat->getAmount();
             float sum = mBudget->getTransactionList()->sumTransactions(cat->getHeading());
             float value = total - sum;

             QString amountStr = QLocale().toCurrencyString(value);
             QStandardItem * amount = new QStandardItem(amountStr);
             amount->setSelectable(false);             
             amount->setForeground(getBrush(value));
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

     updateBank();

 }

void MainWindow::updateBank()
{
    float bank = ui->bankEdit->text().toFloat();
    float amount =  mBudget->getTransactionList()->sumTransactions();
    float diff = (mBudget->sumCategories() - amount) - bank;

    //qDebug() << "Bank change diff:" << diff;
    mBudget->setBank(bank);

    if(diff != 0)
    {
        ui->diffLabel->setText(QLocale().toCurrencyString(diff));

        if(diff < 0)
        {
            QPalette sample_palette = ui->diffLabel->palette();
            sample_palette.setColor(ui->diffLabel->foregroundRole(), Qt::red);
            ui->diffLabel->setPalette(sample_palette);
        }
        else
        {
            QPalette sample_palette = ui->diffLabel->palette();
            sample_palette.setColor(ui->diffLabel->foregroundRole(), Qt::black);
            ui->diffLabel->setPalette(sample_palette);
        }
    }
    else
        ui->diffLabel->setText("");
}

 QList<QStandardItem*> MainWindow::fillCategory(jCategory *cat, jCategory::sCategory * subCat)
 {
     QList<QStandardItem*> lst;

      QString nameStr = QString(subCat->name);

      float total = subCat->amount;
      float sum = mBudget->getTransactionList()->sumTransactions(cat->getHeading(), subCat->name);
      float value = total - sum;
      QString amountStr = QLocale().toCurrencyString(value);

      QStandardItem *name = new QStandardItem( nameStr );
      name->setToolTip(QLocale().toCurrencyString(subCat->amount));
      name->setEditable( false );

      lst.append(name);
      QStandardItem *amount = new QStandardItem( amountStr );
      amount->setEditable( false );
      amount->setSelectable(false);
      amount->setForeground(getBrush(value));
      lst.append(amount);

      return lst;
 }

QBrush MainWindow::getBrush(float val)
{
    QBrush b;
    if(val == 0)
        b.setColor(Qt::gray);
    else if(val < 0)
        b.setColor(Qt::red);

    return b;
}

MainWindow::~MainWindow()
{
    if(mBudget)
        delete mBudget;

    delete ui;
}
