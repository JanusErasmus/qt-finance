#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "jbudget.h"
#include "jtransactionlist.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    ui->transactionTable->setColumnWidth(1,134);


    jBudget * bud = new jBudget("june.jbud");
    jTransactionList * lst = bud->getTransactionList();

//   double amount = 23.87;
//   for(int k = 0; k < 5; k++)
//   {
//       jTransaction * trans = new jTransaction(QDate(2014,06,24), "Die eerste inskrywing", amount);
//       lst->append(trans);
//       amount += 9.23;
//   }



    lst->fillTable(ui->transactionTable);

//   if(bud->setTransactionList(lst))
//       qDebug("File was written");


//    delete bud;
}

MainWindow::~MainWindow()
{
    delete ui;
}
