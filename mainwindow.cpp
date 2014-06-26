#include <QDebug>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ui_addTransaction.h"
#include "jtransactionlist.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{    
    mBudget = 0;

    ui->setupUi(this);

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
    jTransactionList * lst = mBudget->getTransactionList();
    lst->fillTable(ui->transactionTable);
    ui->transactionTable->scrollToBottom();
}

void MainWindow::addTransaction()
{
    QDialog addDialog(0,0);

    Ui_addTransaction addUi;
    addUi.setupUi(&addDialog);

    //setup default values of add transaction window

    //read transaction values if OK
    if(addDialog.exec() == QDialog::Accepted)
    {
        jTransactionList * lst = mBudget->getTransactionList();
        jTransaction * entry = new jTransaction(QDate::currentDate(), addUi.descEdit->text(), addUi.amountEdit->text().toFloat());
        lst->append(entry);

        int row = ui->transactionTable->rowCount();
        ui->transactionTable->insertRow(row);
        entry->setRow(ui->transactionTable, row);
        ui->transactionTable->scrollToBottom();
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

MainWindow::~MainWindow()
{
    delete ui;
}
