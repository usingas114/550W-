#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

LoginDialog::LoginDialog(Simulator550W *simulator, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    m_simulator(simulator)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->leUsername->setFocus();
    updateStatus("请输入用户名和密码");
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_btnLogin_clicked()
{
    QString username = ui->leUsername->text().trimmed();
    QString password = ui->lePassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        updateStatus("用户名和密码不能为空", true);
        return;
    }

    User user;
    #ifdef UNICODE
    bool success = m_simulator->authenticateUser(username.toStdWString(), password.toStdWString(), user);
    #else
    bool success = m_simulator->authenticateUser(username.toStdString(), password.toStdString(), user);
    #endif

    if (success) {
        updateStatus("认证成功！正在加载系统...");
        emit loginSuccessful(user);
        accept();
    } else {
        updateStatus("认证失败：用户名或密码错误", true);
        ui->lePassword->clear();
        ui->lePassword->setFocus();
    }
}

void LoginDialog::on_btnCancel_clicked()
{
    reject();
}

void LoginDialog::updateStatus(const QString &message, bool error)
{
    ui->lblStatus->setText(message);
    if (error) {
        ui->lblStatus->setStyleSheet("color: rgb(255, 80, 80);");
    } else {
        ui->lblStatus->setStyleSheet("color: rgb(0, 180, 255);");
    }
}