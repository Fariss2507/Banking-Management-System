#ifndef DASHBOARDAFTERLOGIN_H
#define DASHBOARDAFTERLOGIN_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class DashboardAfterLogin;
}

class DashboardAfterLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DashboardAfterLogin(QWidget *parent = nullptr);
    ~DashboardAfterLogin();

private slots:
    void on_Dashboard_Button_clicked();

    void on_SendMoney_Button_clicked();

    void on_DepositMoney_Button_clicked();

    void on_WithdrawMoney_Button_clicked();

    void on_Settings_Button_clicked();

    void on_Send_Button_clicked();

    void on_Deposit_Button_clicked();

    void on_Withdraw_button_clicked();

    void set_Balance();

    void set_settingsData();

    void on_SettingsEmailChangeButton_2_clicked();

    void on_SettingsPasswordChangeButton_clicked();

    void on_SettingsUsernameChangeButton_clicked();

    void on_BackButton_clicked();

    void on_BackButton_2_clicked();

    void on_BackButton_3_clicked();

    void on_BackButton_4_clicked();

    void on_ConfirmButton_clicked();

    void on_ConfirmButton_2_clicked();

    void on_ConfirmButton_3_clicked();

private:
    Ui::DashboardAfterLogin *ui;
    QSqlDatabase db;
};

#endif // DASHBOARDAFTERLOGIN_H
