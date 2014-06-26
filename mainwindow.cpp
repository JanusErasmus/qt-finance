#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "jbudget.h"
#include "jtransactionlist.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

  //  QTableWidgetItem* naam = new QTableWidgetItem("Hello");
   ui->transactionTable->setColumnWidth(1,134);

   double amount = 23.87;
   jTransactionList * lst = new jTransactionList();

   for(int k = 0; k < 5; k++)
   {
       jTransaction * trans = new jTransaction(QDate(2014,06,24), "Die eerste inskrywing", amount);
      // trans->debugShow();
       lst->append(trans);
       amount += 9.23;
   }

   lst->fillTable(ui->transactionTable);


   jBudget bud("june.jbud");
//   if(bud.setTransactionList(lst))
//       qDebug("File was written");


   // delete entry;
}

MainWindow::~MainWindow()
{
    delete ui;
}
