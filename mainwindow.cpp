#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "lib/ODALID.h"

#include <QDebug>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

ReaderName Reader;
char pszHost[] = "192.168.1.4";

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_clicked(){
    qDebug() << "clicked !";
    uint16_t status = 0;
    //Reader.Type = ReaderTCP;
    //strcpy(MonLecteur.IPReader, pszHost);
    Reader.Type = ReaderCDC;
    Reader.device = 0;
    status = OpenCOM(&Reader);

    char version[30];
    uint8_t serial[4];
    char stackReader[20];
    status = Version(&Reader, version, serial, stackReader);

    qDebug() << "OpenCOM" << status << version;
}

void MainWindow::on_select_clicked(){
    qDebug() << "select";
}

void MainWindow::on_deconnect_clicked(){
    qDebug() << "deconnect";
}

void MainWindow::on_update_clicked(){
    qDebug() << "update";
}

void MainWindow::on_pay_clicked(){
    qDebug() << "pay";
}

void MainWindow::on_credit_clicked(){
    qDebug() << "credit";
}

