#include "dashboardafterlogin.h"
#include "ui_dashboardafterlogin.h"
#include "Global_data.h"
#include<QPixmap>
#include<QMessageBox>


DashboardAfterLogin::DashboardAfterLogin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DashboardAfterLogin)
{
    ui->setupUi(this);
    ui->WelcomeLabel->setText(QString("Hello, %1").arg(globalUserName));
    ui->stackedWidget->setCurrentWidget(ui->Dashboard_Page);
    ui->Dashboard_Button->setFocus();
    ui->bankCard->setPixmap(QPixmap(":/rec/Resources/BankCard.png").scaled(ui->bankCard->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->tableWidget->setColumnWidth(0,120);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,500);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    set_Balance();
    set_settingsData();



}

DashboardAfterLogin::~DashboardAfterLogin()
{
    delete ui;
}


void DashboardAfterLogin::on_Dashboard_Button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Dashboard_Page);
    ui->Heading->setText("Dashboard");
}


void DashboardAfterLogin::on_SendMoney_Button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->SendMoney_Page);
    ui->Heading->setText("Send Money");
    ui->SelectAccount_ComboBox->clear();
    db.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query("SELECT Username FROM Account_info", db);
    while (query.next()) {
        QString AccountUsername = query.value(0).toString(); // Get the Account Username from the result
        ui->SelectAccount_ComboBox->addItem(AccountUsername); // Add it to the combo box
    }
    QSqlDatabase::database().commit();
    db.close();
    for(int i = 0 ; i < ui->SelectAccount_ComboBox->count() ; i++){
        if(ui->SelectAccount_ComboBox->itemText(i) == globalUserName){
            ui->SelectAccount_ComboBox->removeItem(i);
            break;
        }
    }

}


void DashboardAfterLogin::on_DepositMoney_Button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->DepositMoney_Page);
    ui->Heading->setText("Deposit Money");
}


void DashboardAfterLogin::on_WithdrawMoney_Button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->WithdrawMoney_Page);
    ui->Heading->setText("Withdraw Money");

}



void DashboardAfterLogin::on_Settings_Button_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Settings_Page);
    ui->Heading->setText("Settings");

}


void DashboardAfterLogin::on_Send_Button_clicked()
{
    db.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(db);
    query.prepare("SELECT Balance FROM Account_info WHERE Username = :username");
    query.bindValue(":username", globalUserName);

    if (query.exec() && query.next()) {
        int Balance = query.value(0).toInt();
        if(ui->SendMoney_Amount->text().toInt() > Balance){
            QMessageBox::information(this, "Amount too large", "The Amount you entered exceeds your total balance");
        }
        else{
            QSqlQuery query(db);
            query.prepare("UPDATE Account_info SET Balance = Balance + :amount WHERE Username = :username");
            query.bindValue(":amount", ui->SendMoney_Amount->text().toInt());
            query.bindValue(":username", ui->SelectAccount_ComboBox->currentText());
            if(query.exec()){
                qDebug() << "Send money works";
            }
            else{
                qDebug() << "Send money error" << db.lastError().text();
            }

            QSqlQuery query2(db);
            query2.prepare("UPDATE Account_info SET Balance = Balance - :amount WHERE Username = :username");
            query2.bindValue(":amount", ui->SendMoney_Amount->text().toInt());
            query2.bindValue(":username", globalUserName);
            if(query2.exec()){
                qDebug() << "Send money works";
                QMessageBox::information(this, "Transaction Successful" , "Your Transaction was Successful");
                set_Balance();
            }
            else{
                qDebug() << "Send money error" << db.lastError().text();
            }
        }
        QSqlDatabase::database().commit();
        db.close();
    } else {
        qDebug() << "Error retrieving Balance: " << query.lastError().text();
        db.close();
    }



}


void DashboardAfterLogin::on_Deposit_Button_clicked()
{
    db.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(db);
    query.prepare("UPDATE Account_info SET Balance = Balance + :amount WHERE Username = :username");
    query.bindValue(":amount", ui->Deposit_Amount->text().toInt());
    query.bindValue(":username", globalUserName);
    if(query.exec()){
        qDebug() << "Deposit money Success";
        QMessageBox::information(this, "Deposit Successful" , "Your Deposit was Successful");
        set_Balance();

    }
    else{
        qDebug() << "Deposit money error" << db.lastError().text();
    }
    QSqlDatabase::database().commit();
    db.close();

}


void DashboardAfterLogin::on_Withdraw_button_clicked()
{
    db.open();
    QSqlQuery query(db);
    query.prepare("SELECT Balance FROM Account_info WHERE Username = :username");
    query.bindValue(":username", globalUserName);

    if (query.exec() && query.next()) {
        int Balance = query.value(0).toInt();
        if(ui->Withdraw_Amount->text().toInt() > Balance){
            QMessageBox::information(this, "Amount too large", "The Amount you entered exceeds your total balance");
        }
        else{

            QSqlQuery query(db);
            query.prepare("UPDATE Account_info SET Balance = Balance - :amount WHERE Username = :username");
            query.bindValue(":amount", ui->Withdraw_Amount->text().toInt());
            query.bindValue(":username", globalUserName);
            if(query.exec()){
                qDebug() << "Withdraw money Success";
                QMessageBox::information(this, "WIthdraw Successful" , "Your Withdraw was Successful");
                set_Balance();
            }
            else{
                qDebug() << "Withdraw money error" << db.lastError().text();
            }

        }
        QSqlDatabase::database().commit();
        db.close();
    }
    else{
        qDebug() << "DB error for withdraw" << db.lastError().text();
        db.close();
    }
}

void DashboardAfterLogin::set_Balance(){
    db.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(db);
    query.prepare("SELECT Balance FROM Account_info WHERE Username = :username");
    query.bindValue(":username", globalUserName);
    if (query.exec() && query.next()) {
        QString Balance = query.value(0).toString();
        ui->Total_Balance_label->setText(Balance + "$");
    }
}

void DashboardAfterLogin::set_settingsData(){
    ui->SettingsUsernameLabel->setText(globalUserName);
    db.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(db);
    query.prepare("SELECT Email FROM Account_info WHERE Username = :username");
    query.bindValue(":username", globalUserName);
    if (query.exec() && query.next()) {
        QString Email = query.value(0).toString();
        ui->SettingsEmailLabel->setText(Email);
    }
    query.prepare("SELECT Password FROM Login_info WHERE Username = :username");
    query.bindValue(":username", globalUserName);
    if (query.exec() && query.next()) {
        QString Password = query.value(0).toString();
        ui->SettingsPasswordLabel->setText(Password);
    }

}



void DashboardAfterLogin::on_SettingsEmailChangeButton_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Change_Email_Page);
    ui->Heading->setText("Change Email");

}


void DashboardAfterLogin::on_SettingsPasswordChangeButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Change_Password_Page);
    ui->Heading->setText("Change Password");
}


void DashboardAfterLogin::on_SettingsUsernameChangeButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Change_Username_Page);
    ui->Heading->setText("Change Username");

}


void DashboardAfterLogin::on_BackButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Settings_Page);
    ui->Heading->setText("Settings");

}


void DashboardAfterLogin::on_BackButton_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Settings_Page);
    ui->Heading->setText("Settings");

}


void DashboardAfterLogin::on_BackButton_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Settings_Page);
    ui->Heading->setText("Settings");
}


void DashboardAfterLogin::on_BackButton_4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Settings_Page);
    ui->Heading->setText("Settings");
}


void DashboardAfterLogin::on_ConfirmButton_clicked() // for changing email
{
    db.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(db);
    QString Email = ui->NewEmail_Label->text();
    ui->SettingsEmailLabel->setText(ui->NewEmail_Label->text());
    query.prepare("UPDATE Account_Info SET Email = :email WHERE Username = :username");
    query.bindValue(":username", globalUserName);
    query.bindValue(":email", Email);
    if(query.exec()){
        qDebug() << "Email Changed Successfully";
        QMessageBox::information(this, "Operation Succesful" , "Your Email was Changed");
    }
    QSqlDatabase::database().commit();
    db.close();

}


void DashboardAfterLogin::on_ConfirmButton_2_clicked() //for changing password
{
    db.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(db);
    QString password = ui->NewPasswordLabel->text();
    QString cpassword = ui->CNewPasswordLabel->text();
    ui->SettingsPasswordLabel->setText(ui->NewPasswordLabel->text());
    if(password != cpassword){
        QMessageBox::information(this, "Mismatched Password", "The Password and Confirm Password field do not match");
    }
    else{
        query.prepare("UPDATE Login_Info SET password  = :password WHERE username = :username");
        query.bindValue(":username", globalUserName);
        query.bindValue(":password", password);
        if(query.exec()){
            qDebug() << "Password Changed Successfully";
            QMessageBox::information(this, "Operation Succesful" , "Your Password was Changed");
        }
    }
    QSqlDatabase::database().commit();
    db.close();
}



void DashboardAfterLogin::on_ConfirmButton_3_clicked() // for Changing Username
{
    db.open();
    QSqlDatabase::database().transaction();
    QSqlQuery query(db);
    QString User = ui->NewUsernameLabel->text();
    ui->SettingsUsernameLabel->setText(ui->NewUsernameLabel->text());
    query.prepare("UPDATE Login_Info SET username = :user WHERE username = :username");
    query.bindValue(":username", globalUserName);
    qDebug() << globalUserName;
    query.bindValue(":user", User);
    if(query.exec()){
        qDebug() << "Username Changed Successfully";
        QMessageBox::information(this, "Operation Succesful" , "Your Username was Changed");
    }
    else{
        qDebug() << "Error: " << db.lastError().text();
    }
    query.prepare("UPDATE Account_Info SET Username = :user WHERE Username = :username");
    query.bindValue(":username", globalUserName);
    qDebug() << globalUserName;
    query.bindValue(":user", User);
    if(query.exec()){
        qDebug() << "Username Changed Successfully 2";
    }
    else{
        qDebug() << "Error: " << db.lastError().text();
    }
    QSqlDatabase::database().commit();
    db.close();
    globalUserName = ui->NewUsernameLabel->text();
}



