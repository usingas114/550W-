#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "simulator550w.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(Simulator550W *simulator, QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    void loginSuccessful(const ::User &user);

private slots:
    void on_btnLogin_clicked();
    void on_btnCancel_clicked();

private:
    Ui::LoginDialog *ui;
    Simulator550W *m_simulator;
    void updateStatus(const QString &message, bool error = false);
};

#endif // LOGIN_H