#include "mainwindow.h"
#include "login.h"
#include "simulator550w.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 设置应用程序信息
    a.setApplicationName("550W模拟器");
    a.setApplicationVersion("7.0.0");
    a.setOrganizationName("量子计算中心");
    a.setOrganizationDomain("quantum.com");
    
    // 创建模拟器实例
    Simulator550W simulator(TSTR("550W"));
    
    // 显示登录对话框
    LoginDialog login(&simulator);
    if (login.exec() == QDialog::Accepted) {
        // 登录成功，显示主窗口
        MainWindow w(&simulator);
        w.show();
        return a.exec();
    } else {
        // 登录取消，退出应用
        return 0;
    }
}
