#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include "lib/ODALID.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

//ReaderName Reader;
//char pszHost[] = "192.168.1.4";

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_clicked()
{
    qDebug() << "CLICKED !";
    //uint16_t status = 0;
    //MonLecteur.Type = ReaderTCP;
    //strcpy(MonLecteur.IPReader, pszHost);
    //Reader.Type = ReaderCDC;
    //Reader.device = 0;
    //status = OpenCOM1(&Reader);
    //qDebug() << "OpenCOM1" << status;
}
