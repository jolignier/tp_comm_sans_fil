#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_connect_clicked();
    void on_deconnect_clicked();
    void on_update_clicked();
    void on_pay_clicked();
    void on_credit_clicked();

protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;
    QString read_name();
    QString read_surname();
    QString read_counter();

};
#endif // MAINWINDOW_H
