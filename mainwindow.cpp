#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Global_data.h"

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QTableView>
#include <QApplication>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/MATEEN/Desktop/BankManagementSystem_UniProject/BankManagement.db"); //Add Database

    if (!db.open()) {
        qDebug() << "Error: Could not open database: " << db.lastError().text(); // Database not open , display last error in the console
    } else {
        qDebug() << "Database connection successful!";
    }
    db.close();

    RegScr = new RegisterScreen(this);
    connect(RegScr->findChild<QPushButton*>("pushButtonToLogin"), &QPushButton::clicked, this, &MainWindow::on_pushButtonToLogin_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_LoginButton_clicked() {
    db.open();
    QSqlDatabase::database().transaction();
    QString username = ui->Username->text();
    QString password = ui->Password->text();
    globalUserName = ui->Username->text();
    QSqlQuery query(db);
    query.prepare("SELECT username,password FROM login_info WHERE username=:username AND password=:password");
    query.bindValue(":username",username);
    query.bindValue(":password",password);

    if (query.exec()) {
        if (query.next()) {
            QMessageBox::information(this, "Success", "Login Successful");
            ui->Username->clear();
            ui->Password->clear();
            hide();
            DAL = new DashboardAfterLogin(this);
            DAL->show();
            connect(DAL->findChild<QPushButton*>("Logout_Button"), &QPushButton::clicked, this, &MainWindow::on_Logout_Button_clicked);

        } else {
            QMessageBox::warning(this, "Login Failed", "Failed to login , try registering for an account first");
        }

    } else {

        qDebug() << "Error executing query: " << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Database error occurred.");
    }
    QSqlDatabase::database().commit();
    db.close();
}



void MainWindow::on_pushButton_clicked()
{
    hide();
    RegScr->show();
}

void MainWindow::on_pushButtonToLogin_clicked(){
    RegScr->hide();
    show();
}


void MainWindow::on_Logout_Button_clicked(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to Logout ?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        DAL->hide();
        show();
    }
}




