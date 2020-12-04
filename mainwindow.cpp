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

    unsigned char data[240];
    int status = 0;

    BYTE sect_count = 16;
    BYTE bloc_count, bloc, sect, offset;
    BYTE atq[2];
    BYTE sak[1];
    BYTE uid[12];
    BYTE uid_len = 12;

    for (BYTE sect = 0; sect < sect_count; sect++){
        status = Mf_Classic_Authenticate(&Reader, Auth_KeyB, FALSE, sect, key_ff, 0);
        status = Mf_Classic_Read_Sector(&Reader, FALSE, sect, data, Auth_KeyA, 0);

        if (status != MI_OK){
            printf("[Failed]\n");
            printf("%s (%d)\n", GetErrorMessage(status), status);
        }
        else {
            if (sect < 32)
                bloc_count = 3;
            else
                bloc_count = 15;
            for (bloc = 0; bloc < bloc_count; bloc++){
                for (offset = 0; offset<16; offset++)
                    printf("%02X ", data[16 * bloc + offset]);
                    if (data[16 * bloc + offset] >= ' '){
                        printf("%c", data[16 * bloc + offset]);
                    } else {
                        printf(".");
                    }
            }

        }
    }

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

