#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "simulator550w.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Simulator550W *simulator, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 命令执行
    void on_btnExecute_clicked();
    void executeCommand(const QString &command);
    
    // 系统命令
    void on_btnStatus_clicked();
    void on_btnSimulate_clicked();
    void on_btnDiagnostics_clicked();
    void on_btnShutdown_clicked();
    
    // 文件系统命令
    void on_btnCreateFile_clicked();
    void on_btnDeleteFile_clicked();
    
    // 任务管理命令
    void on_btnAddTask_clicked();
    void on_btnCompleteTask_clicked();
    
    // 网络命令
    void on_btnPingNode_clicked();
    void on_btnActivateQuantumLink_clicked();
    void on_btnDeactivateQuantumLink_clicked();
    
    // 高级功能命令
    void on_btnTimeTravel_clicked();
    void on_btnSimulateFuture_clicked();
    void on_btnPlanetaryAlignment_clicked();
    void on_btnAssessThreat_clicked();
    
    // 神经网络命令
    void on_btnNeuralScan_clicked();
    void on_btnUpgradeKnowledge_clicked();
    
    // AI对话命令
    void on_btnAIDialogue_clicked();
    
    // 模拟功能命令
    void on_pushButton_clicked();       // 人类文明计划
    void on_pushButton_2_clicked();     // 量子网络扩展
    void on_pushButton_3_clicked();     // 意识模型训练
    void on_pushButton_4_clicked();     // 安全协议测试
    
    // 菜单命令
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_actionChange_Password_triggered();
    void on_actionSystem_Info_triggered();
    
    // 系统状态更新
    void updateSystemStatus();
    
    // 历史命令槽
    void on_treeCommandHistory_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
    Simulator550W *simulator;
    QTimer *statusTimer;
    void appendOutput(const QString &text, const QColor &color = Qt::white);
    void initializeUI();
    void setupStatusBar();
    void setupMenuBar();
    void displaySystemInfo();
    void updateSystemInfo();
    void updateFileSystem();
    void updateNeuralNetworks();
    void updateTasks();
    void showStatus();
    void showSimulate();
    void startSimulation(const QString &simulationName);
    void updateSimulationProgress(int progress);
    void appendSimulationResult(const QString &result);
    void runDiagnostics();
    void shutdownSystem();
    void activateQuantumLink();
    void deactivateQuantumLink();
    void addTask();
    void completeTask();
    void aiDialogue();
    void showAbout();
    void changePassword();
    void logMessage(const QString &message);
};

#endif // MAINWINDOW_H
