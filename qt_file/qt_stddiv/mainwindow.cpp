#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "std_div.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //--------------------------------------------------------
    TStdDiv * div = new TStdDiv();
    div->setParent(this);


    div->move(100, 100);
    div->resize(200, 150);

//    div->setAutoFillBackground(true);

//    QPalette p = div->palette();

//    //QColor color = QColor(44,50,61);

//    p.setColor(QPalette::Window, QColor(190,190,190));

//    //透明色
//    p.setColor(QPalette::Window, QColor(190,190,190, 0));

//    //p.setBrush(QPalette::Window,brush);

//    div->setPalette(p);


}

MainWindow::~MainWindow()
{
    delete ui;
}

