#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "lib/ODALID.h"
#include "MfErrNo.h"

#include <QDebug>
#include <QtGui>


// ------------------------------------------
// ----------- Variables globales -----------
// ------------------------------------------

#define Auth_KeyA   TRUE
#define Auth_KeyB   FALSE

ReaderName Reader;
char pszHost[] = "192.168.1.4";

unsigned char auth_key_A[6] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 };
unsigned char auth_key_B[6] = { 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5 };

unsigned char counter_key_A[6] = { 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5 };
unsigned char counter_key_B[6] = { 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5 };

unsigned char key_ff[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

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
    uint16_t reader_status = 0;
    Reader.Type = ReaderCDC;
    Reader.device = 0;
    reader_status = OpenCOM(&Reader);

    char version[30];
    uint8_t serial[4];
    char stackReader[20];
    reader_status = Version(&Reader, version, serial, stackReader);

    qDebug() << "OpenCOM" << reader_status << version;



    // Enable Reader RF Field
    RF_Power_Control(&Reader, TRUE, 0);
    int status = 0;

    // Load Keys into reader memory
    status = Mf_Classic_LoadKey(&Reader, Auth_KeyA, auth_key_A, 2);
    qDebug() <<" Load key A" << status;
    status = Mf_Classic_LoadKey(&Reader, Auth_KeyB, auth_key_B, 2);
    qDebug() <<" Load key B" << status;

    status = Mf_Classic_LoadKey(&Reader, Auth_KeyA, counter_key_A, 3);
    qDebug() <<" Load key C" << status;
    status = Mf_Classic_LoadKey(&Reader, Auth_KeyB, counter_key_B, 3);
    qDebug() <<" Load key D" << status;

}

void MainWindow::on_select_clicked(){
    qDebug() << "select";

    uint8_t data[16];
    int16_t status =0;

    status = Mf_Classic_Read_Block(&Reader, TRUE, 10, data, Auth_KeyA, 2);
    qDebug() << "Read Block status" << status ;


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

