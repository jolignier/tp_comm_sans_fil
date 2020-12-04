#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "lib/ODALID.h"

#include <QDebug>
#include <QtGui>


// ------------------------------------------
// ----------- Variables globales -----------
// ------------------------------------------

ReaderName Reader;
char pszHost[] = "192.168.1.4";

// ------------------------------------------
// --------- Gestion de la fenêtre ----------
// ------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){
    qDebug() << "Program closed";
    int16_t status = 0;
    RF_Power_Control(&Reader, FALSE, 0);
    status = LEDBuzzer(&Reader, LED_OFF);
    status = CloseCOM(&Reader);
    qApp->quit();
}

// ------------------------------------------
// ---------- Connexion à la carte ----------
// ------------------------------------------

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

