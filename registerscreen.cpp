#include "registerscreen.h"
#include "ui_registerscreen.h"
#include<QMessageBox>
RegisterScreen::RegisterScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterScreen)
{
    ui->setupUi(this);
}

RegisterScreen::~RegisterScreen()
{
    delete ui;
}


void RegisterScreen::on_RegisterButton_clicked()
{
    db.open();
    QSqlDatabase::database().transaction();
    QString username = ui->Username->text();
    QString password = ui->Password->text();
    QString cpassword = ui->Password_2->text();
    QString email= ui->Email->text();
    QString CurrentAccount = "Current Account";
    QString SavingsAccount = "Savings Account";
    QString StatusActive = "Active";
    bool CurrentAccountCheck = ui->Curr_Acc_RadioButton->isChecked();
    bool SavingsAccountCheck = ui->Sav_Acc_RadioButton->isChecked();


    if(password != cpassword){
        QMessageBox::information(this, "Mismatched Password", "The Password and Confirm Password field do not match");
    }
    else{

        QSqlQuery query(db);
        query.prepare("INSERT INTO login_info (username, password) VALUES(:username,:password)");
        query.bindValue(":username",username);
        query.bindValue(":password",password);

        if (query.exec()) {
            qDebug() << "Registered Successfully";
        }
        else {

            qDebug() << "Error executing query: " << query.lastError().text();

            QMessageBox::critical(this, "Database Error", "Database error occurred.");
        }

        query.prepare("INSERT INTO Account_info (Username, Account_Type , Status, Email) VALUES(:Username,:Account_Type ,:Status , :Email)");
        query.bindValue(":Username",username);
        if(CurrentAccountCheck == true){
            query.bindValue(":Account_Type", CurrentAccount);
        }
        else if (SavingsAccountCheck == true){
            query.bindValue(":Account_Type" , SavingsAccount);
        }
        else{
            QMessageBox::critical(this, "No Account Type Chosen ", "Please Choose an account type to continue");

        }
        query.bindValue(":Status", StatusActive);
        query.bindValue(":Email",email);


        if (query.exec()) {
            qDebug() << "Registered Successfully";
            QMessageBox::information(this, "Registration Successful", "You have been registered , please login to your account to continue");
        }
        else {

            qDebug() << "Error executing query: " << query.lastError().text();

            QMessageBox::critical(this, "Database Error", "Database error occurred.");
        }

        QSqlDatabase::database().commit();
        db.close();

    }
}

