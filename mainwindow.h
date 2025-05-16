#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include "registerscreen.h"
#include "dashboardafterlogin.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_LoginButton_clicked();
    void on_pushButton_clicked();
    void on_pushButtonToLogin_clicked();
    void on_Logout_Button_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    RegisterScreen *RegScr;
    DashboardAfterLogin *DAL;
};
#endif // MAINWINDOW_H
