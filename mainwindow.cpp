#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "lib/ODALID.h"
#include "MfErrNo.h"

#include <QDebug>
#include <QtGui>
#include <QMessageBox>


// ------------------------------------------
// ----------- Variables globales -----------
// ------------------------------------------

ReaderName Reader;
char pszHost[] = "192.168.1.4";

uint8_t auth_key_A[6] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 };
uint8_t auth_key_B[6] = { 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5 };

uint8_t counter_key_A[6] = { 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5 };
uint8_t counter_key_B[6] = { 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5 };

int auth_sector = 2;
int auth_key_index = 2;
int counter_sector = 3;
int counter_key_index = 3;

uint8_t atq[2];
uint8_t sak[1];
uint16_t uid_len = 12;
uint8_t uid[12];

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
    //status = CloseCOM(&Reader);
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
    //reader_status = OpenCOM(&Reader);

    char version[30];
    uint8_t serial[4];
    char stackReader[20];
    reader_status = Version(&Reader, version, serial, stackReader);

    qDebug() << "OpenCOM" << reader_status << version;

    this->ui->version->setText(version);
    this->ui->version->update();

    // Enable Reader RF Field
    RF_Power_Control(&Reader, TRUE, 0);
    int status = 0;

    // Load Keys into reader memory
    status = Mf_Classic_LoadKey(&Reader, Auth_KeyA, auth_key_A, auth_key_index);
    qDebug() <<" Load key A" << status;
    status = Mf_Classic_LoadKey(&Reader, Auth_KeyB, auth_key_B, auth_key_index);
    qDebug() <<" Load key B" << status;

    status = Mf_Classic_LoadKey(&Reader, Auth_KeyA, counter_key_A, counter_key_index);
    qDebug() <<" Load key C" << status;
    status = Mf_Classic_LoadKey(&Reader, Auth_KeyB, counter_key_B, counter_key_index);
    qDebug() <<" Load key D" << status;

    status = ISO14443_3_A_PollCard(&Reader, atq, sak, uid, &uid_len);

    QString prenom = read_name();
    QString nom = read_surname();
    QString solde = read_counter();

    this->ui->prenom->setText(prenom);
    this->ui->prenom->setDisabled(false);
    this->ui->prenom->update();

    this->ui->nom->setText(nom);
    this->ui->nom->setDisabled(false);
    this->ui->nom->update();

    this->ui->solde->setText(solde);
    this->ui->solde->update();

}

// ------------------------------------------
// ------- Lecture du nom de famille --------
// ------------------------------------------

QString MainWindow::read_surname(){

    uint8_t data[16];
    int16_t status =0;

    status = Mf_Classic_Read_Block(&Reader, TRUE, (auth_sector*4)+2, data, Auth_KeyA, auth_key_index);
    qDebug() << "Read Surname Block status" << status ;

    QString surname((char*)data);
    return surname;
}

// ------------------------------------------
// ----------- Lecture du prénom ------------
// ------------------------------------------

QString MainWindow::read_name(){

    uint8_t data[16];
    int16_t status =0;

    status = Mf_Classic_Read_Block(&Reader, TRUE, (auth_sector*4)+1, data, Auth_KeyA, auth_key_index);
    qDebug() << "Read Name Block status" << status ;

    QString surname((char*)data);
    return surname;
}

// ------------------------------------------
// ------------ Lecture du solde ------------
// ------------------------------------------

QString MainWindow::read_counter(){

    uint32_t data[16];
    int16_t status =0;

    status = Mf_Classic_Read_Value(&Reader, TRUE, (counter_sector*4)+1, data, Auth_KeyA, counter_key_index);
    qDebug() << "Read Counter Value status" << status ;

    QString value = QString::number(data[0]);
    return value;
}

// ------------------------------------------
// -------- Déconnexion de la carte ---------
// ------------------------------------------

void MainWindow::on_deconnect_clicked(){
    qDebug() << "deconnect";

    // Update the UI to stop displaying data
    // And disable inputs
    this->ui->version->setText("");
    this->ui->version->update();

    this->ui->prenom->setText("");
    this->ui->prenom->setDisabled(true);
    this->ui->prenom->update();

    this->ui->nom->setText("");
    this->ui->nom->setDisabled(true);
    this->ui->nom->update();

    this->ui->solde->setText("");
    this->ui->solde->update();

    // Close the RF Field and
    // Close the communication with the reader
    int16_t status = 0;
    status = ISO14443_3_A_Halt(&Reader);
    RF_Power_Control(&Reader, FALSE, 0);
    //status = CloseCOM(&Reader);
}

// ------------------------------------------
// ------- Mise à jour de l'identité --------
// ------------------------------------------

void MainWindow::on_update_clicked(){
    //Varibles declaration
    uint8_t data[18];
    int16_t status = 0;
    QString firstName, lastName;

    lastName  = ui->nom->text();
    firstName = ui->prenom->text();

    status = ISO14443_3_A_PollCard(&Reader, atq, sak, uid, &uid_len);

    strncpy((char*)data, lastName.toUtf8().constData(), 16);
    qDebug() << "Data -> " << (char*)data;
    status = Mf_Classic_Write_Block(&Reader, TRUE, (auth_sector*4)+2, data, Auth_KeyB, auth_key_index);
    qDebug() << "Write block Status -> " << status;

    strncpy((char*)data, firstName.toUtf8().constData(), 16);
    qDebug() << "Data -> " << (char*)data;
    status = Mf_Classic_Write_Block(&Reader, TRUE, (auth_sector*4)+1, data, Auth_KeyB, auth_key_index);
    qDebug() << "Write block Status -> " << status;

}

// ------------------------------------------
// -------- Décrémentation du solde ---------
// ------------------------------------------

void MainWindow::on_pay_clicked(){
    int16_t status = 0;
    int amount = ui->nb_a_payer->value();
    this->ui->nb_a_payer->setValue(0);

    // Si le solde est suffisant pour payer
    if (read_counter().toInt() >= amount) {
        //Décrémenter la valeur de credit dans le Backup Compteur (bloc 13) de 1
        status = Mf_Classic_Decrement_Value(&Reader, TRUE, (counter_sector*4)+2, amount, (counter_sector*4)+1, Auth_KeyA, counter_key_index);
        qDebug() << "Mf_Classic_Decrement" << status;
        //Copier la valeur du Backup Compteur (bloc 13) dans le Compteur (bloc 14)
        status = Mf_Classic_Restore_Value(&Reader, TRUE, (counter_sector*4)+1, (counter_sector*4)+2, Auth_KeyB, counter_key_index);
        qDebug() << "Mf_Classic_Restore" << status;
    } else {
        QMessageBox msgBox;
        msgBox.setText("Le solde est insuffisant !");
        msgBox.exec();
    }

    QString solde = read_counter();
    this->ui->solde->setText(solde);
    this->ui->solde->update();
}

// ------------------------------------------
// -------- Incrémentation du solde ---------
// ------------------------------------------

// Condition d'accès : 001 => Permet le décrement et la lecture
// Condition d'accès : 110 => Permet l'increment avec la clé B (=clé D)
void MainWindow::on_credit_clicked(){
    int16_t status = 0;
    int amount = ui->nb_a_crediter->value();
    this->ui->nb_a_crediter->setValue(0);

    //Décrémenter la valeur de credit dans le Backup Compteur (bloc 13) de 1
    status = Mf_Classic_Increment_Value(&Reader, TRUE, (counter_sector*4)+2, amount, (counter_sector*4)+1, Auth_KeyB, counter_key_index);
    qDebug() << "Mf_Classic_Decrement" << status;
    //Copier la valeur du Backup Compteur (bloc 13) dans le Compteur (bloc 14)
    status = Mf_Classic_Restore_Value(&Reader, TRUE, (counter_sector*4)+1, (counter_sector*4)+2, Auth_KeyB, counter_key_index);
    qDebug() << "Mf_Classic_Restore" << status;

    QString solde = read_counter();
    this->ui->solde->setText(solde);
    this->ui->solde->update();
}

