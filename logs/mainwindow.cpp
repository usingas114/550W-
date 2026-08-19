#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QTimerEvent>
#include <QFontDatabase>
#include <QPalette>
#include <QColor>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(Simulator550W *simulator, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    simulator(simulator),
    statusTimer(nullptr)
{
    ui->setupUi(this);
    
    // 设置窗口标题和图标
    setWindowTitle(QString::fromStdString("550W模拟器 v6.0 - " + simulator->getCodename()));
    
    // 设置科技感字体
    QFontDatabase::addApplicationFont("://fonts/roboto.ttf");
    
    // 设置样式
    setupStyles();
    
    // 初始化界面组件
    initializeUI();
    
    // 连接信号和槽
    connectSignals();
    
    // 启动状态更新定时器
    statusTimer = new QTimer(this);
    connect(statusTimer, &QTimer::timeout, this, &MainWindow::updateSystemStatus);
    statusTimer->start(2000); // 每2秒更新一次
    
    // 显示启动信息
    logMessage("[系统] 550W量子计算机系统已启动");
    logMessage("[用户] 已登录: root (超级用户)");
    
    // 更新系统信息
    updateSystemInfo();
}

MainWindow::~MainWindow()
{
    if (statusTimer) {
        statusTimer->stop();
        delete statusTimer;
    }
    delete ui;
}

void MainWindow::setupStyles()
{
    // 创建深色主题样式
    QPalette palette;
    QColor background = QColor(30, 30, 40);
    QColor foreground = QColor(200, 200, 255);
    QColor accent = QColor(100, 150, 255);
    QColor warning = QColor(255, 150, 100);
    QColor error = QColor(255, 100, 100);
    QColor success = QColor(100, 255, 150);
    
    palette.setColor(QPalette::Window, background);
    palette.setColor(QPalette::WindowText, foreground);
    palette.setColor(QPalette::Text, foreground);
    palette.setColor(QPalette::Button, QColor(40, 40, 50));
    palette.setColor(QPalette::ButtonText, foreground);
    palette.setColor(QPalette::Highlight, accent);
    palette.setColor(QPalette::HighlightedText, QColor(20, 20, 30));
    palette.setColor(QPalette::Base, QColor(25, 25, 35));
    palette.setColor(QPalette::AlternateBase, QColor(35, 35, 45));
    palette.setColor(QPalette::ToolTipBase, QColor(50, 50, 60));
    palette.setColor(QPalette::ToolTipText, foreground);
    
    setPalette(palette);
    
    // 应用全局样式表
    QString styleSheet = R"(
        /* 主窗口和基础组件 */
        QMainWindow {
            background-color: rgb(15, 15, 25);
            color: rgb(0, 255, 255); /* 霓虹青色 */
            font-family: 'Roboto', sans-serif;
        }
        
        QWidget {
            background-color: rgb(15, 15, 25);
            color: rgb(150, 255, 255);
            font-family: 'Roboto', sans-serif;
        }
        
        /* 按钮样式 */
        QPushButton {
            background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(30, 50, 80, 255), stop:1 rgba(40, 60, 100, 255));
            border: 1px solid rgba(0, 200, 255, 80);
            border-radius: 4px;
            padding: 8px 14px;
            min-width: 90px;
            font-size: 12px;
            color: rgb(150, 255, 255);
            box-shadow: 0 0 5px rgba(0, 200, 255, 0.3);
        }
        
        QPushButton:hover {
            background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(40, 60, 100, 255), stop:1 rgba(50, 70, 120, 255));
            border: 1px solid rgba(0, 255, 255, 150);
            box-shadow: 0 0 10px rgba(0, 255, 255, 0.5);
        }
        
        QPushButton:pressed {
            background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(50, 70, 120, 255), stop:1 rgba(60, 80, 140, 255));
            box-shadow: 0 0 5px rgba(0, 200, 255, 0.7) inset;
        }
        
        QPushButton#btnExecute {
            background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(0, 150, 255, 255), stop:1 rgba(0, 200, 255, 255));
            color: rgb(10, 20, 30);
            font-weight: bold;
            border-color: rgba(0, 255, 255, 200);
        }
        
        QPushButton#btnExecute:hover {
            background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(0, 200, 255, 255), stop:1 rgba(0, 255, 255, 255));
            box-shadow: 0 0 15px rgba(0, 255, 255, 0.7);
        }
        
        /* 输入框样式 */
        QLineEdit {
            background-color: rgba(20, 30, 45, 200);
            border: 1px solid rgba(0, 200, 255, 80);
            border-radius: 4px;
            padding: 8px;
            color: rgb(0, 255, 255);
            font-family: 'Consolas', monospace;
            box-shadow: 0 0 5px rgba(0, 200, 255, 0.2) inset;
        }
        
        QLineEdit:focus {
            border: 1px solid rgba(0, 255, 255, 150);
            box-shadow: 0 0 10px rgba(0, 255, 255, 0.5) inset;
        }
        
        /* 文本编辑框样式 */
        QTextEdit {
            background-color: rgba(20, 30, 45, 200);
            border: 1px solid rgba(0, 200, 255, 80);
            border-radius: 4px;
            color: rgb(150, 255, 255);
            font-family: 'Consolas', monospace;
            font-size: 12px;
            box-shadow: 0 0 5px rgba(0, 200, 255, 0.2) inset;
        }
        
        /* 树状控件样式 */
        QTreeWidget {
            background-color: rgba(20, 30, 45, 200);
            border: 1px solid rgba(0, 200, 255, 80);
            border-radius: 4px;
            color: rgb(150, 255, 255);
            alternate-background-color: rgba(30, 40, 60, 150);
        }
        
        QTreeWidget::item {
            height: 26px;
            padding: 2px;
        }
        
        QTreeWidget::item:selected {
            background-color: rgba(0, 200, 255, 50);
            color: rgb(0, 255, 255);
            border: 1px solid rgba(0, 255, 255, 100);
        }
        
        /* 标签页样式 */
        QTabWidget::pane {
            background-color: rgba(20, 30, 45, 200);
            border: 1px solid rgba(0, 200, 255, 80);
            border-radius: 6px;
            margin: 2px;
        }
        
        QTabBar::tab {
            background-color: rgba(30, 40, 60, 200);
            border: 1px solid rgba(0, 200, 255, 50);
            border-radius: 4px 4px 0 0;
            padding: 8px 14px;
            margin-right: 3px;
            color: rgb(100, 200, 255);
        }
        
        QTabBar::tab:selected {
            background-color: rgba(20, 30, 45, 200);
            border-bottom-color: rgba(20, 30, 45, 200);
            color: rgb(0, 255, 255);
            border-top: 2px solid rgba(0, 255, 255, 150);
        }
        
        /* 状态栏样式 */
        QStatusBar {
            background-color: rgba(30, 50, 80, 200);
            border-top: 1px solid rgba(0, 200, 255, 80);
            color: rgb(0, 255, 255);
        }
        
        /* 进度条样式 */
        QProgressBar {
            background-color: rgba(20, 30, 45, 200);
            border: 1px solid rgba(0, 200, 255, 80);
            border-radius: 10px;
            text-align: center;
            color: rgb(0, 255, 255);
        }
        
        QProgressBar::chunk {
            background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 150, 255, 255), stop:1 rgba(0, 255, 255, 255));
            border-radius: 10px;
            box-shadow: 0 0 8px rgba(0, 255, 255, 0.8);
        }
        
        /* 标签样式 */
        QLabel {
            color: rgb(150, 255, 255);
        }
        
        QLabel#statusValue {
            font-weight: bold;
            color: rgb(0, 255, 255);
        }
        
        QLabel#statusLabel {
            color: rgb(100, 200, 255);
        }
        
        /* 分组框样式 */
        QGroupBox {
            border: 1px solid rgba(0, 200, 255, 50);
            border-radius: 4px;
            margin-top: 8px;
        }
        
        QGroupBox::title {
            color: rgb(0, 255, 255);
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 5px 0 5px;
        }
    );
    
    setStyleSheet(styleSheet);
}

void MainWindow::initializeUI()
{
    // 初始化系统状态显示
    ui->lblSystemLoad->setText("15%");
    ui->lblCoreTemp->setText("28.7°C");
    ui->lblQuantumStability->setText("99.80%");
    ui->lblThreatLevel->setText("0/10");
    ui->lblNodes->setText("12/12");
    
    // 初始化命令历史
    ui->treeCommandHistory->setColumnCount(2);
    QStringList headers;
    headers << "命令" << "时间";
    ui->treeCommandHistory->setHeaderLabels(headers);
    ui->treeCommandHistory->header()->resizeSection(0, 200);
    
    // 连接历史命令选择信号
    connect(ui->treeCommandHistory, &QTreeWidget::itemDoubleClicked,
            this, &MainWindow::on_treeCommandHistory_itemDoubleClicked);
    
    // 初始化文件系统
    ui->treeFileSystem->setColumnCount(2);
    headers.clear();
    headers << "文件名" << "描述";
    ui->treeFileSystem->setHeaderLabels(headers);
    ui->treeFileSystem->header()->resizeSection(0, 150);
    
    // 初始化神经网络列表
    ui->treeNeuralNetworks->setColumnCount(1);
    headers.clear();
    headers << "神经网络";
    ui->treeNeuralNetworks->setHeaderLabels(headers);
    
    // 初始化任务列表
    ui->treeTasks->setColumnCount(2);
    headers.clear();
    headers << "任务" << "状态";
    ui->treeTasks->setHeaderLabels(headers);
    ui->treeTasks->header()->resizeSection(0, 200);
    
    // 更新文件系统
    updateFileSystem();
    
    // 更新神经网络
    updateNeuralNetworks();
    
    // 更新任务列表
    updateTasks();
}

void MainWindow::connectSignals()
{
    // 命令输入
    connect(ui->btnExecute, &QPushButton::clicked, this, &MainWindow::executeCommand);
    connect(ui->leCommand, &QLineEdit::returnPressed, this, &MainWindow::executeCommand);
    
    // 系统控制按钮
    connect(ui->btnStatus, &QPushButton::clicked, this, &MainWindow::showStatus);
    connect(ui->btnSimulate, &QPushButton::clicked, this, &MainWindow::showSimulate);
    connect(ui->btnDiagnostics, &QPushButton::clicked, this, &MainWindow::runDiagnostics);
    connect(ui->btnShutdown, &QPushButton::clicked, this, &MainWindow::shutdownSystem);
    
    // 文件系统功能
    connect(ui->btnCreateFile, &QPushButton::clicked, this, &MainWindow::on_btnCreateFile_clicked);
    connect(ui->btnDeleteFile, &QPushButton::clicked, this, &MainWindow::on_btnDeleteFile_clicked);
    
    // 菜单操作
    connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->actionChange_Password, &QAction::triggered, this, &MainWindow::changePassword);
    connect(ui->actionSystem_Info, &QAction::triggered, this, &MainWindow::updateSystemInfo);
    
    // 网络功能
    connect(ui->btnPingNode, &QPushButton::clicked, this, &MainWindow::on_btnPingNode_clicked);
    connect(ui->btnActivateQuantumLink, &QPushButton::clicked, this, &MainWindow::activateQuantumLink);
    connect(ui->btnDeactivateQuantumLink, &QPushButton::clicked, this, &MainWindow::deactivateQuantumLink);
    
    // 任务管理
    connect(ui->btnAddTask, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(ui->btnCompleteTask, &QPushButton::clicked, this, &MainWindow::completeTask);
    
    // AI功能
    connect(ui->btnAIDialogue, &QPushButton::clicked, this, &MainWindow::aiDialogue);
    connect(ui->btnUpgradeKnowledge, &QPushButton::clicked, this, &MainWindow::on_btnUpgradeKnowledge_clicked);
    connect(ui->btnNeuralScan, &QPushButton::clicked, this, &MainWindow::on_btnNeuralScan_clicked);
    
    // 模拟功能
    connect(ui->btnTimeTravel, &QPushButton::clicked, this, &MainWindow::on_btnTimeTravel_clicked);
    connect(ui->btnSimulateFuture, &QPushButton::clicked, this, &MainWindow::on_btnSimulateFuture_clicked);
    connect(ui->btnPlanetaryAlignment, &QPushButton::clicked, this, &MainWindow::on_btnPlanetaryAlignment_clicked);
    connect(ui->btnAssessThreat, &QPushButton::clicked, this, &MainWindow::on_btnAssessThreat_clicked);
    
    // 预设模拟按钮
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_clicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::on_pushButton_3_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_4_clicked);
}

void MainWindow::executeCommand()
{
    QString command = ui->leCommand->text().trimmed();
    executeCommand(command);
    
    // 清除输入框
    ui->leCommand->clear();
}

void MainWindow::executeCommand(const QString &command)
{
    if (command.isEmpty()) return;
    
    // 记录命令到历史
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");
    QTreeWidgetItem* item = new QTreeWidgetItem;
    item->setText(0, command);
    item->setText(1, timestamp);
    ui->treeCommandHistory->insertTopLevelItem(0, item);
    
    // 显示执行信息
    logMessage(QString("[命令] > %1").arg(command));
    
    // 执行命令
    string cmd = command.toStdString();
    
    // 特殊命令处理
    if (cmd == "exit" || cmd == "shutdown") {
        shutdownSystem();
        return;
    }
    
    // 处理命令
    simulator->processCommand(cmd);
    
    // 更新界面
    updateSystemInfo();
    updateSystemStatus();
    updateFileSystem();
    updateTasks();
    
    // 如果是文件相关命令，更新文件系统
    if (cmd.find("create") != string::npos || 
        cmd.find("rm") != string::npos ||
        cmd.find("ls") != string::npos) {
        updateFileSystem();
    }
    
    // 如果是任务相关命令，更新任务列表
    if (cmd.find("tasks") != string::npos ||
        cmd.find("addtask") != string::npos ||
        cmd.find("complete") != string::npos) {
        updateTasks();
    }
}

void MainWindow::logMessage(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString logEntry = QString("%1 %2\n").arg(timestamp).arg(message);
    
    // 在UI中显示日志
    ui->txtLog->append(logEntry);
    ui->txtLog->moveCursor(QTextCursor::End);
    
    // 保存日志到文件
    static QFile logFile("simulator_log.txt");
    if (!logFile.isOpen()) {
        if (logFile.open(QIODevice::Append | QIODevice::Text)) {
            logMessage("[系统] 日志文件已打开");
        }
    }
    if (logFile.isOpen()) {
        QTextStream out(&logFile);
        out << logEntry;
    }
    
    // 限制日志行数，避免内存溢出
    if (ui->txtLog->document()->blockCount() > 1000) {
        QTextCursor cursor(ui->txtLog->document()->begin());
        cursor.movePosition(QTextCursor::EndOfBlock);
        cursor.movePosition(QTextCursor::StartOfNextBlock, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
    }
}

void MainWindow::updateSystemStatus()
{
    // 从模拟器核心获取最新系统状态
    simulator->updateSystemStatus();
    
    // 获取系统状态数据
    int load = simulator->getSystemLoad();
    double temp = simulator->getCoreTemperature();
    double stability = simulator->getQuantumStability();
    int threat = simulator->getThreatLevel();
    int nodes = simulator->getActiveNodes();
    int totalNodes = simulator->getTotalNodes();
    bool isQuantumLinkActive = simulator->isQuantumLinkActive();
    
    // 更新状态标签
    ui->lblSystemLoad->setText(QString::number(load) + "%");
    ui->lblCoreTemp->setText(QString::number(temp, 'f', 1) + "°C");
    ui->lblQuantumStability->setText(QString::number(stability, 'f', 2) + "%");
    ui->lblThreatLevel->setText(QString::number(threat) + "/10");
    ui->lblNodes->setText(QString::number(nodes) + "/" + QString::number(totalNodes));
    
    // 更新进度条
    ui->progressSystemLoad->setValue(load);
    ui->progressQuantumStability->setValue(int(stability));
    ui->progressThreatLevel->setValue(threat * 10);
    
    // 更新状态指示器颜色
    updateStatusIndicators(load, temp, stability, threat);
    
    // 更新量子链接状态
    if (isQuantumLinkActive) {
        ui->lblQuantumLinkStatus->setText("已激活");
        ui->lblQuantumLinkStatus->setStyleSheet("color: rgb(100, 255, 150); font-weight: bold;");
    } else {
        ui->lblQuantumLinkStatus->setText("未激活");
        ui->lblQuantumLinkStatus->setStyleSheet("color: rgb(255, 150, 100); font-weight: bold;");
    }
    
    // 更新系统状态指示器
    if (simulator->getSystemStatus() == "online") {
        ui->lblSystemState->setText("在线");
        ui->lblSystemState->setStyleSheet("color: rgb(100, 255, 150); font-weight: bold;");
    } else if (simulator.getSystemStatus() == "warning") {
        ui->lblSystemState->setText("警告");
        ui->lblSystemState->setStyleSheet("color: rgb(255, 200, 100); font-weight: bold;");
    } else {
        ui->lblSystemState->setText("离线");
        ui->lblSystemState->setStyleSheet("color: rgb(255, 100, 100); font-weight: bold;");
    }
    
    // 更新状态栏
    ui->statusBar->showMessage(
        QString("系统负载: %1% | 核心温度: %2°C | 量子稳定性: %3% | 威胁等级: %4/10 | 节点: %5/%6")
        .arg(load)
        .arg(temp, 0, 'f', 1)
        .arg(stability, 0, 'f', 2)
        .arg(threat)
        .arg(nodes)
        .arg(totalNodes)
    );
}

void MainWindow::updateStatusIndicators(int load, double temp, double stability, int threat)
{
    QPalette palette;
    
    // 系统负载指示器
    if (load < 40) {
        palette.setColor(QPalette::WindowText, QColor(100, 255, 150)); // 绿色
    } else if (load < 70) {
        palette.setColor(QPalette::WindowText, QColor(255, 200, 100)); // 黄色
    } else {
        palette.setColor(QPalette::WindowText, QColor(255, 100, 100)); // 红色
    }
    ui->lblSystemLoad->setPalette(palette);
    
    // 核心温度指示器
    if (temp < 30.0) {
        palette.setColor(QPalette::WindowText, QColor(100, 255, 150)); // 绿色
    } else if (temp < 35.0) {
        palette.setColor(QPalette::WindowText, QColor(255, 200, 100)); // 黄色
    } else {
        palette.setColor(QPalette::WindowText, QColor(255, 100, 100)); // 红色
    }
    ui->lblCoreTemp->setPalette(palette);
    
    // 量子稳定性指示器
    if (stability > 99.5) {
        palette.setColor(QPalette::WindowText, QColor(100, 255, 150)); // 绿色
    } else if (stability > 98.0) {
        palette.setColor(QPalette::WindowText, QColor(255, 200, 100)); // 黄色
    } else {
        palette.setColor(QPalette::WindowText, QColor(255, 100, 100)); // 红色
    }
    ui->lblQuantumStability->setPalette(palette);
    
    // 威胁等级指示器
    if (threat < 3) {
        palette.setColor(QPalette::WindowText, QColor(100, 255, 150)); // 绿色
    } else if (threat < 6) {
        palette.setColor(QPalette::WindowText, QColor(255, 200, 100)); // 黄色
    } else {
        palette.setColor(QPalette::WindowText, QColor(255, 100, 100)); // 红色
    }
    ui->lblThreatLevel->setPalette(palette);
}

// UI按钮和菜单的标准Qt槽函数实现
void MainWindow::on_btnCreateFile_clicked()
{
    QString fileName = ui->leFileName->text().trimmed();
    if (fileName.isEmpty()) {
        logMessage("请输入文件名");
        return;
    }
    executeCommand(QString("create %1").arg(fileName));
}

void MainWindow::on_btnDeleteFile_clicked()
{
    QString fileName = ui->leFileName->text().trimmed();
    if (fileName.isEmpty()) {
        logMessage("请输入文件名");
        return;
    }
    executeCommand(QString("rm %1").arg(fileName));
}

void MainWindow::on_btnPingNode_clicked()
{
    QString node = ui->leNodeAddress->text().trimmed();
    if (node.isEmpty()) {
        logMessage("请输入节点地址");
        return;
    }
    executeCommand(QString("ping %1").arg(node));
}

void MainWindow::on_btnTimeTravel_clicked()
{
    executeCommand("timetravel");
}

void MainWindow::on_btnSimulateFuture_clicked()
{
    executeCommand("simulatefuture");
}

void MainWindow::on_btnPlanetaryAlignment_clicked()
{
    executeCommand("planetaryalignment");
}

void MainWindow::on_btnAssessThreat_clicked()
{
    executeCommand("threat");
}

void MainWindow::on_btnNeuralScan_clicked()
{
    executeCommand("neuralscan");
}

void MainWindow::on_btnUpgradeKnowledge_clicked()
{
    executeCommand("upgrade");
}

void MainWindow::on_pushButton_clicked()
{
    ui->leSimulation->setText("人类文明计划");
    executeCommand("simulate 人类文明计划");
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->leSimulation->setText("量子网络扩展");
    executeCommand("simulate 量子网络扩展");
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->leSimulation->setText("意识模型训练");
    executeCommand("simulate 意识模型训练");
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->leSimulation->setText("安全协议测试");
    executeCommand("simulate 安全协议测试");
}

void MainWindow::on_treeCommandHistory_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // 从历史记录中选择命令并执行
    if (item) {
        QString command = item->text(0);
        ui->leCommand->setText(command);
        executeCommand(command);
    }
}

void MainWindow::updateSystemInfo()
{
    // 更新系统信息
    vector<string> info;
    stringstream ss;
    
    ss << "AI系统: 意识量子模型 v6.0" << endl;
    ss << "处理器: 量子量子计算机 (1.62 ZFLOPS)" << endl;
    ss << "内存: 128PB HBM6" << endl;
    ss << "存储: 5.0YB 量子存储" << endl;
    ss << "网络: 全局量子通信网络 (12节点)" << endl;
    ss << "安全协议: 量子 (加密)" << endl;
    ss << "威胁等级: 0/10" << endl;
    ss << "系统负载: 15%" << endl;
    ss << "核心温度: 28.7°C" << endl;
    ss << "量子稳定性: 99.80%" << endl;
    ss << "知识图谱等级: 5/10" << endl;
    ss << "量子链接状态: 未激活" << endl;
    ss << "时间旅行模拟: 未激活" << endl;
    ss << "当前用户: root (超级用户)" << endl;
    
    ui->txtSystemInfo->setText(QString::fromStdString(ss.str()));
}

void MainWindow::updateFileSystem()
{
    // 清空文件系统树
    ui->treeFileSystem->clear();
    
    // 获取文件列表
    vector<string> files = simulator->getFiles();
    
    for (const auto& file : files) {
        size_t sep = file.find(" - ");
        if (sep != string::npos) {
            string name = file.substr(0, sep);
            string desc = file.substr(sep + 3);
            
            QTreeWidgetItem* item = new QTreeWidgetItem;
            item->setText(0, QString::fromStdString(name));
            item->setText(1, QString::fromStdString(desc));
            ui->treeFileSystem->addTopLevelItem(item);
        }
    }
}

void MainWindow::updateNeuralNetworks()
{
    // 清空神经网络树
    ui->treeNeuralNetworks->clear();
    
    // 从模拟器获取神经网络列表
    vector<string> networks = simulator->getNeuralNetworks();
    
    for (const auto& network : networks) {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, QString::fromStdString(network));
        ui->treeNeuralNetworks->addTopLevelItem(item);
    }
}

void MainWindow::updateTasks()
{
    // 清空任务树
    ui->treeTasks->clear();
    
    // 从模拟器获取任务列表
    vector<string> tasks = simulator->getTasks();
    
    for (const auto& task : tasks) {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, QString::fromStdString(task));
        item->setText(1, "待处理");
        ui->treeTasks->addTopLevelItem(item);
    }
}

void MainWindow::showStatus()
{
    logMessage("[系统] 显示系统状态");
    logMessage("[状态] 系统负载: 15% | 核心温度: 28.7°C | 量子稳定性: 99.80%");
    logMessage("[状态] 威胁等级: 0/10 | 全局节点: 12/12 | 节点在线");
}

void MainWindow::showSimulate()
{
    QString simulation = ui->leSimulation->text().trimmed();
    if (simulation.isEmpty()) {
        simulation = "人类文明计划 - 阶段4";
    }
    
    startSimulation(simulation);
    
    logMessage(QString("[模拟] 执行: %1").arg(simulation));
    logMessage("[模拟] 阶段 1/5... 正在初始化模拟环境");
    logMessage("[模拟] 阶段 2/5... 正在构建量子量子态模型");
    logMessage("[模拟] 阶段 3/5... 正在运行意识认知算法");
    logMessage("[模拟] 阶段 4/5... 正在分析模拟结果");
    logMessage("[模拟] 阶段 5/5... 正在生成报告");
    logMessage("[模拟结果] 结果: 成功");
    
    updateSimulationProgress(100);
    appendSimulationResult("[模拟结果] 结果: 成功");
}

void MainWindow::on_pushButton_clicked() {
    QString simulation = "人类文明计划";
    ui->leSimulation->setText(simulation);
    showSimulate();
}

void MainWindow::on_pushButton_2_clicked() {
    QString simulation = "量子网络扩展";
    ui->leSimulation->setText(simulation);
    showSimulate();
}

void MainWindow::on_pushButton_3_clicked() {
    QString simulation = "意识模型训练";
    ui->leSimulation->setText(simulation);
    showSimulate();
}

void MainWindow::on_pushButton_4_clicked() {
    QString simulation = "安全协议测试";
    ui->leSimulation->setText(simulation);
    showSimulate();
}

void MainWindow::startSimulation(const QString &simulationName) {
    // 重置进度条
    updateSimulationProgress(0);
    // 清空结果显示
    ui->textEdit_2->clear();
    appendSimulationResult("开始模拟: " + simulationName);
    
    // 模拟进度更新
    for (int i = 0; i <= 100; i += 10) {
        updateSimulationProgress(i);
        QThread::msleep(100);
        QApplication::processEvents();
    }
}

void MainWindow::updateSimulationProgress(int progress) {
    ui->progressBar_4->setValue(progress);
}

void MainWindow::appendSimulationResult(const QString &result) {
    ui->textEdit_2->append(result);
    // 滚动到底部
    QTextCursor cursor = ui->textEdit_2->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit_2->setTextCursor(cursor);
}

void MainWindow::runDiagnostics()
{
    logMessage("[诊断] 开始系统自检");
    logMessage("[诊断] 诊断: 量子计算机系统... 优秀 (35%)");
    logMessage("[诊断] 诊断: 量子内存系统... 正常 (56%)");
    logMessage("[诊断] 诊断: 意识系统... 优秀 (28%)");
    logMessage("[诊断] 诊断: 安全子系统... 正常 (65%)");
    logMessage("[诊断] 诊断: 全局节点网络... 优秀 (22%)");
    logMessage("[诊断] 诊断: 预测系统... 正常 (71%)");
    logMessage("[诊断] 诊断: 意识认知系统... 优秀 (42%)");
    logMessage("[诊断] 诊断: 模拟系统... 正常 (60%)");
    logMessage("[诊断结论] 系统状态: 良好");
}

void MainWindow::shutdownSystem()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "关闭系统", 
                                  "确定要关闭550W模拟器吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        logMessage("[关闭协议] 接收停止信号");
        logMessage("[网络] 关闭量子节点连接");
        logMessage("[内存] 保存系统状态");
        logMessage("[AI系统] 关闭意识模型");
        logMessage("[安全] 执行加密协议");
        logMessage("[系统] 550W量子计算机系统已安全关闭");
        
        close();
    }
}

void MainWindow::activateQuantumLink()
{
    logMessage("[量子通信] 正在激活量子链接");
    logMessage("[量子通信] 量子通信协议 1/5 ... [成功]");
    logMessage("[量子通信] 量子通信协议 2/5 ... [成功]");
    logMessage("[量子通信] 量子通信协议 3/5 ... [成功]");
    logMessage("[量子通信] 量子通信协议 4/5 ... [成功]");
    logMessage("[量子通信] 量子通信协议 5/5 ... [成功]");
    logMessage("[量子通信] 量子链接已激活");
}

void MainWindow::deactivateQuantumLink()
{
    logMessage("[量子通信] 正在关闭量子链接");
    logMessage("[量子通信] 协议 5 ... [完成]");
    logMessage("[量子通信] 协议 4 ... [完成]");
    logMessage("[量子通信] 协议 3 ... [完成]");
    logMessage("[量子通信] 协议 2 ... [完成]");
    logMessage("[量子通信] 协议 1 ... [完成]");
    logMessage("[量子通信] 量子链接已关闭");
}

void MainWindow::addTask()
{
    QString task = ui->leTask->text().trimmed();
    if (task.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入任务描述");
        return;
    }
    
    logMessage(QString("[任务管理] 添加任务: %1").arg(task));
    logMessage("[任务管理] 任务已添加到队列");
    
    // 清空输入框
    ui->leTask->clear();
    
    // 更新任务列表
    updateTasks();
}

void MainWindow::completeTask()
{
    QTreeWidgetItem* selected = ui->treeTasks->currentItem();
    if (!selected) {
        QMessageBox::warning(this, "错误", "请选择要完成的任务");
        return;
    }
    
    QString task = selected->text(0);
    logMessage(QString("[任务完成] %1").arg(task));
    logMessage("[任务完成] 状态: 成功");
    
    // 更新任务列表
    updateTasks();
}

void MainWindow::aiDialogue()
{
    QString question = ui->leAIQuestion->text().trimmed();
    if (question.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入您的问题");
        return;
    }
    
    logMessage(QString("[AI对话] 正在分析: \"%1\"").arg(question));
    
    // 简单的AI回复（实际应该调用模拟器的aiDialogue方法）
    QString response;
    QString lowerQuestion = question.toLower();
    
    if (lowerQuestion.contains("你好") || lowerQuestion.contains("hello")) {
        response = "你好, root!我是550W系统AI系统。";
    } else if (lowerQuestion.contains("名字")) {
        response = "我是550W系统，当前代号: 550W";
    } else if (lowerQuestion.contains("目的") || lowerQuestion.contains("用途")) {
        response = "我的主要用途是为人类提供量子计算服务。";
    } else if (lowerQuestion.contains("答案")) {
        response = "所有问题的最终答案可能是42。";
    } else if (lowerQuestion.contains("帮助")) {
        response = "输入'help'查看可用命令。";
    } else if (lowerQuestion.contains("未来")) {
        response = "未来11,945种可能性,其中87.3%的结果是人类的胜利。";
    } else {
        vector<string> responses = {
            "该问题超出了我的当前知识范围。",
            "我需要更多信息来回答这个问题。",
            "正在搜索知识图谱... 未找到确切答案。",
            "这个问题可能需要更复杂的分析。",
            "根据我的计算 - 需要更高权限。"
        };
        response = QString::fromStdString(responses[rand() % responses.size()]);
    }
    
    logMessage(QString("[AI回复] %1").arg(response));
    
    // 清空输入框
    ui->leAIQuestion->clear();
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "关于550W模拟器", 
                       "550W量子计算机模拟器 v6.0\n\n" 
                       "基于C++和Qt框架开发\n\n" 
                       "模拟未来量子计算机系统的功能和界面\n" 
                       "包含量子网络、意识模型、安全协议等先进技术\n\n" 
                       "© 2025 量子科技有限公司");
}

void MainWindow::changePassword()
{
    logMessage("[安全协议] 修改密码");
    logMessage("[安全协议] 密码修改功能暂时不可用");
}

// 自动生成的槽函数，调用已经实现的功能函数
void MainWindow::on_btnStatus_clicked()
{
    showStatus();
}

void MainWindow::on_btnDiagnostics_clicked()
{
    runDiagnostics();
}

void MainWindow::on_btnShutdown_clicked()
{
    shutdownSystem();
}

void MainWindow::on_btnAddTask_clicked()
{
    addTask();
}

void MainWindow::on_btnCompleteTask_clicked()
{
    completeTask();
}

void MainWindow::on_btnActivateQuantumLink_clicked()
{
    activateQuantumLink();
}

void MainWindow::on_btnDeactivateQuantumLink_clicked()
{
    deactivateQuantumLink();
}

void MainWindow::on_btnAIDialogue_clicked()
{
    aiDialogue();
}

void MainWindow::on_actionAbout_triggered()
{
    showAbout();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionChange_Password_triggered()
{
    changePassword();
}

void MainWindow::on_actionSystem_Info_triggered()
{
    updateSystemInfo();
}
