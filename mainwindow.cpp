#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "jtransactionlist.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

  //  QTableWidgetItem* naam = new QTableWidgetItem("Hello");
   ui->transactionTable->setColumnWidth(1,134);

   double amount = 23.87;
    jTransactionList lst;

   for(int k = 0; k < 500; k++)
   {

       lst.append(new jTransaction(QDate(2014,06,24), "Die eerste inskrywing", amount));
       amount += 9.23;
   }



   lst.fillTable(ui->transactionTable);




   // delete entry;
}

MainWindow::~MainWindow()
{
    delete ui;
}
