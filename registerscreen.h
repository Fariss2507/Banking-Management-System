#ifndef REGISTERSCREEN_H
#define REGISTERSCREEN_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class RegisterScreen;
}

class RegisterScreen : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterScreen(QWidget *parent = nullptr);
    ~RegisterScreen();

private slots:

    void on_RegisterButton_clicked();

private:
    Ui::RegisterScreen *ui;
    QSqlDatabase db;
};

#endif // REGISTERSCREEN_H
