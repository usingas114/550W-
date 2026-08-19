#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <map>
#include <memory>
#include <functional>
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <atomic>
#include <future>
#include <sys/stat.h> 

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

// 跨平台密码输入函数
string getHiddenInput() {
    string password = "";
    char ch;
    
#ifdef _WIN32
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        } else {
            cout << '*';
            password += ch;
        }
    }
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    while ((ch = getchar()) != '\n') {
        if (ch == 127) { // 退格键
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        } else {
            cout << '*';
            password += ch;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
    cout << endl;
    return password;
}

// 创建目录（跨平台，C++11兼容）
bool createDirectory(const string& path) {
#ifdef _WIN32
    return CreateDirectoryA(path.c_str(), NULL) != 0;
#else
    return mkdir(path.c_str(), 0755) == 0;
#endif
}

// 检查目录是否存在（跨平台，C++11兼容）
bool directoryExists(const string& path) {
#ifdef _WIN32
    DWORD attrib = GetFileAttributesA(path.c_str());
    return (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat info;
    return stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR);
#endif
}

// 随机数生成器（C++11风格）
class RandomGenerator {
private:
    mt19937 generator;
    uniform_int_distribution<int> dist;
    
public:
    RandomGenerator() : generator(time(nullptr)), dist(0, 100) {}
    
    int getRandom(int min = 0, int max = 100) {
        uniform_int_distribution<int> custom_dist(min, max);
        return custom_dist(generator);
    }
    
    double getRandomDouble(double min = 0.0, double max = 1.0) {
        uniform_real_distribution<double> real_dist(min, max);
        return real_dist(generator);
    }
};

// 用户类
class User {
public:
    string username;
    string password;
    int privilege; // 0:普通用户, 1:管理员, 2:根用户
    
    User(const string& u, const string& p, int priv) 
        : username(u), password(p), privilege(priv) {}
    
    bool checkPassword(const string& pwd) const {
        return password == pwd;
    }
    
    string getPrivilegeName() const {
        switch(privilege) {
            case 0: return "普通用户";
            case 1: return "管理员";
            case 2: return "根用户";
            default: return "未知权限";
        }
    }
};

// 量子网络节点
struct QuantumNode {
    string address;
    string ip;
    bool online;
    double stability;
    
    QuantumNode(const string& addr, const string& ip_addr) 
        : address(addr), ip(ip_addr), online(true), stability(95.0) {}
};

// 系统任务
struct SystemTask {
    string description;
    int priority;
    chrono::system_clock::time_point created;
    bool completed;
    
    SystemTask(const string& desc, int prio = 1) 
        : description(desc), priority(prio), 
          created(chrono::system_clock::now()), completed(false) {}
};

// 量子计算模拟器
class QuantumComputer {
private:
    RandomGenerator rng;
    
public:
    enum class QubitState {
        ZERO, ONE, SUPERPOSITION, ENTANGLED
    };
    
    vector<QubitState> qubits;
    
    QuantumComputer(int num_qubits = 16) {
        initializeQubits(num_qubits);
    }
    
    void initializeQubits(int num_qubits) {
        qubits.clear();
        for (int i = 0; i < num_qubits; ++i) {
            qubits.push_back(QubitState::ZERO);
        }
    }
    
    void applyHadamard(int qubit_index) {
        if (qubit_index < qubits.size()) {
            if (rng.getRandomDouble() > 0.5) {
                qubits[qubit_index] = QubitState::SUPERPOSITION;
            }
        }
    }
    
    void entangleQubits(int qubit1, int qubit2) {
        if (qubit1 < qubits.size() && qubit2 < qubits.size()) {
            qubits[qubit1] = QubitState::ENTANGLED;
            qubits[qubit2] = QubitState::ENTANGLED;
        }
    }
    
    string measureQubit(int qubit_index) {
        if (qubit_index >= qubits.size()) return "无效量子位";
        
        auto state = qubits[qubit_index];
        switch(state) {
            case QubitState::ZERO: return "0";
            case QubitState::ONE: return "1";
            case QubitState::SUPERPOSITION: 
                return rng.getRandomDouble() > 0.5 ? "1" : "0";
            case QubitState::ENTANGLED:
                return "纠缠态 - 测量会影响其他量子位";
            default: return "未知状态";
        }
    }
    
    string getStateDescription(int qubit_index) {
        if (qubit_index >= qubits.size()) return "无效量子位";
        
        switch(qubits[qubit_index]) {
            case QubitState::ZERO: return "|0?";
            case QubitState::ONE: return "|1?";
            case QubitState::SUPERPOSITION: return "|+? (叠加态)";
            case QubitState::ENTANGLED: return "纠缠态";
            default: return "未知";
        }
    }
};

// 主模拟器类
class Simulator550W {
private:
    // 系统状态
    atomic<bool> activated;
    atomic<bool> shutdown_requested;
    string codename;
    vector<string> commandHistory;
    bool debugMode;
    bool mossMode;
    atomic<int> threatLevel;
    map<string, string> fileSystem;
    vector<SystemTask> taskQueue;
    vector<QuantumNode> globalNodes;
    atomic<int> systemLoad;
    atomic<int> coreTemp;
    atomic<double> quantumStability;
    bool overloadMode;
    atomic<bool> selfDestructInitiated;
    atomic<int> selfDestructCountdown;
    vector<User> users;
    string currentUser;
    int currentPrivilege;
    bool quantumLinkActive;
    atomic<int> knowledgeLevel;
    vector<string> neuralNetworks;
    bool timeTravelSimulation;
    int simulationYear;
    
    // 日志系统
    ofstream logFile;
    string logFileName;
    atomic<bool> loggingEnabled;
    
    // 工具对象
    RandomGenerator rng;
    unique_ptr<QuantumComputer> quantumComputer;
    
    // C++11时间工具
    chrono::system_clock::time_point startTime;
    
    // 命令映射表
    map<string, function<void(const string&)>> commandMap;
    
    // 生成随机量子地址
    string generateQuantumAddress() {
        string address = "QN-";
        const string chars = "0123456789ABCDEF";
        
        for (int i = 0; i < 8; i++) {
            address += chars[rng.getRandom(0, chars.size() - 1)];
        }
        return address;
    }
    
    // 生成随机IP地址
    string generateIPAddress() {
        return to_string(rng.getRandom(1, 255)) + "." +
               to_string(rng.getRandom(0, 255)) + "." +
               to_string(rng.getRandom(0, 255)) + "." +
               to_string(rng.getRandom(1, 255));
    }
    
    // 睡眠函数（跨平台）
    void sleep_ms(int milliseconds) {
        this_thread::sleep_for(chrono::milliseconds(milliseconds));
    }
    
    // 日志记录函数
    void logMessage(const string& message, const string& type = "INFO") {
        if (!loggingEnabled || !logFile.is_open()) return;
        
        auto now = chrono::system_clock::now();
        auto time_t_now = chrono::system_clock::to_time_t(now);
        tm* timeinfo = localtime(&time_t_now);
        
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
        
        logFile << "[" << timestamp << "] [" << type << "] " << message << endl;
        logFile.flush();
    }
    
    // 初始化日志系统
    bool initializeLogging() {
        string logDir = "D:\\c++项目\\550W运行日志";
        
        // 创建日志目录
        if (!directoryExists(logDir)) {
            if (!createDirectory(logDir)) {
                cout << "[警告] 无法创建日志目录，日志功能将禁用" << endl;
                loggingEnabled = false;
                return false;
            }
        }
        
        // 生成日志文件名
        auto now = chrono::system_clock::now();
        auto time_t_now = chrono::system_clock::to_time_t(now);
        tm* timeinfo = localtime(&time_t_now);
        
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y%m%d_%H%M%S", timeinfo);
        
        logFileName = logDir + "\\550W模拟器运行日志_" + timeStr + ".txt";
        logFile.open(logFileName, ios::out | ios::app);
        
        if (!logFile.is_open()) {
            cout << "[警告] 无法创建日志文件，日志功能将禁用" << endl;
            loggingEnabled = false;
            return false;
        }
        
        loggingEnabled = true;
        logMessage("550W模拟器启动 - 日志系统初始化完成");
        logMessage("当前用户: " + currentUser + " (" + getPrivilegeName() + ")");
        logMessage("系统代号: " + codename);
        
        return true;
    }
    
    // 显示启动序列
    void displayBootSequence() {
        cout << "\n[系统] 正在初始化量子核心...\n";
        logMessage("初始化量子核心");
        for (int i = 0; i < 5; i++) {
            cout << ">> 启动量子位 " << (i+1) << "/5 ... [校准完成]\n";
            sleep_ms(400);
        }
        
        buildQuantumNetwork();
        
        cout << "\n[AI核心] 激活认知模块...\n";
        cout << ">> 神经突触连接: ■ > ■ > ■ [激活完成]\n";
        sleep_ms(500);
        
        cout << "\n[状态] 系统准备就绪\n";
        cout << "\n>>>> 550W在线 <<<<\n";
        cout << ">> 当前代号: " << codename << endl;
        cout << ">> 用户: " << currentUser << " (" << getPrivilegeName() << ")" << endl;
        cout << ">> 输入 'help' 获取命令列表\n" << endl;
        
        logMessage("系统启动完成");
        logMessage("当前代号: " + codename);
    }
    
    // 构建量子网络
    void buildQuantumNetwork() {
        cout << "\n[量子网络] 正在初始化节点矩阵...\n";
        sleep_ms(800);
        
        const int totalNodes = 12;
        globalNodes.clear();
        
        cout << "\n节点状态:\n[ ";
        for (int i = 0; i < totalNodes; i++) {
            cout << "· ";
        }
        cout << "]\n";
        
        int connectedNodes = 0;
        while (connectedNodes < totalNodes) {
            sleep_ms(200 + rng.getRandom(0, 300));
            
            string nodeAddress = generateQuantumAddress();
            string nodeIP = generateIPAddress();
            globalNodes.emplace_back(nodeAddress, nodeIP);
            
            cout << ">> 连接节点 " << nodeAddress << " ... ";
            if (rng.getRandom() < 85) {
                cout << "[成功]\n";
                connectedNodes++;
                
                cout << "[ ";
                for (int i = 0; i < connectedNodes; i++) cout << "■ ";
                for (int i = connectedNodes; i < totalNodes; i++) cout << "· ";
                cout << "]\n";
            } else {
                cout << "[失败 - 量子干扰]\n";
                cout << ">> 重新校准...\n";
                sleep_ms(400);
            }
        }
        
        cout << "\n>> 量子网络建立完成 (" << totalNodes << "/" << totalNodes << " 节点在线)\n";
        logMessage("量子网络初始化完成，共 " + to_string(totalNodes) + " 个节点");
    }
    
    // 获取权限名称
    string getPrivilegeName() const {
        for (const auto& user : users) {
            if (user.username == currentUser) {
                return user.getPrivilegeName();
            }
        }
        return "未知权限";
    }
    
    // 初始化命令映射
    void initializeCommandMap() {
        commandMap = {
            {"help", [this](const string&) { showHelp(); }},
            {"status", [this](const string&) { showStatus(); }},
            {"sysinfo", [this](const string&) { displaySystemInfo(); }},
            {"history", [this](const string&) { showCommandHistory(); }},
            {"time", [this](const string&) { displaySystemTime(); }},
            {"diag", [this](const string&) { runDiagnostics(); }},
            {"rename", [this](const string&) { renameSystem(); }},
            {"moss", [this](const string&) { toggleMossMode(); }},
            {"log", [this](const string&) { viewSystemLog(); }},
            {"ls", [this](const string&) { listFiles(); }},
            {"create", [this](const string&) { createFile(""); }},
            {"rm", [this](const string&) { deleteFile(""); }},
            {"ping", [this](const string&) { pingNode(""); }},
            {"threat", [this](const string&) { assessThreat(); }},
            {"tasks", [this](const string&) { listTasks(); }},
            {"addtask", [this](const string&) { addTask(""); }},
            {"complete", [this](const string&) { completeTask(); }},
            {"planets", [this](const string&) { displayPlanetaryAlignment(); }},
            {"ask", [this](const string& args) { aiDialogue(args); }},
            {"passwd", [this](const string&) { changePassword(); }},
            {"users", [this](const string&) { listUsers(); }},
            {"adduser", [this](const string&) { createUser(); }},
            {"deluser", [this](const string&) { deleteUser(); }},
            {"su", [this](const string&) { switchUser(); }},
            {"quantum", [this](const string& args) { quantumComputing(args); }},
            {"backup", [this](const string&) { backupSystem(); }},
            {"restore", [this](const string&) { restoreSystem(); }},
            {"simulate", [this](const string& args) { runSimulation(args); }},
            {"calc", [this](const string& args) { calculateExpression(args); }},
            {"encrypt", [this](const string& args) { encryptData(args); }},
            {"decrypt", [this](const string& args) { decryptData(args); }},
            {"optimize", [this](const string&) { optimizeSystem(); }},
            {"network", [this](const string&) { showNetworkStatus(); }},
            {"clear", [this](const string&) { clearScreen(); }},
            {"savelog", [this](const string&) { saveLogToFile(); }}
        };
    }
    
    // 新增功能：数据加密
    void encryptData(const string& args) {
        if (args.empty()) {
            cout << "\n[加密] 请输入要加密的数据: ";
            string data;
            getline(cin, data);
            
            cout << ">> 正在使用量子加密算法...\n";
            sleep_ms(800);
            
            // 简单的加密演示（实际应用中应使用更强的加密）
            string encrypted;
            for (char c : data) {
                encrypted += to_string((int)c + 128) + " ";
            }
            
            cout << ">> 加密完成\n";
            cout << ">> 加密数据: " << encrypted << endl;
            logMessage("数据加密完成，长度: " + to_string(data.length()));
        } else {
            cout << ">> 正在加密: " << args << "\n";
            sleep_ms(500);
            
            string encrypted;
            for (char c : args) {
                encrypted += to_string((int)c + 128) + " ";
            }
            
            cout << ">> 加密结果: " << encrypted << endl;
            logMessage("命令行数据加密完成");
        }
        cout << endl;
    }
    
    // 新增功能：数据解密
    void decryptData(const string& args) {
        if (args.empty()) {
            cout << "\n[解密] 请输入加密数据: ";
            string data;
            getline(cin, data);
            
            cout << ">> 正在使用量子解密算法...\n";
            sleep_ms(800);
            
            // 简单的解密演示
            string decrypted;
            istringstream iss(data);
            string token;
            while (iss >> token) {
                int val = stoi(token) - 128;
                if (val >= 0 && val <= 255) {
                    decrypted += static_cast<char>(val);
                }
            }
            
            cout << ">> 解密完成\n";
            cout << ">> 原始数据: " << decrypted << endl;
            logMessage("数据解密完成，长度: " + to_string(decrypted.length()));
        } else {
            cout << ">> 正在解密: " << args << "\n";
            sleep_ms(500);
            
            string decrypted;
            istringstream iss(args);
            string token;
            while (iss >> token) {
                int val = stoi(token) - 128;
                if (val >= 0 && val <= 255) {
                    decrypted += static_cast<char>(val);
                }
            }
            
            cout << ">> 解密结果: " << decrypted << endl;
            logMessage("命令行数据解密完成");
        }
        cout << endl;
    }
    
    // 新增功能：系统优化
    void optimizeSystem() {
        cout << "\n[系统优化] 开始性能优化...\n";
        logMessage("开始系统优化");
        
        vector<string> optimizationSteps = {
            "清理临时文件",
            "优化内存分配",
            "重组量子位布局",
            "更新神经网络权重",
            "压缩数据库"
        };
        
        for (const auto& step : optimizationSteps) {
            cout << ">> " << step << "... ";
            sleep_ms(600);
            
            int progress = rng.getRandom(70, 100);
            cout << "[完成 " << progress << "%]\n";
            logMessage("优化步骤完成: " + step + " (" + to_string(progress) + "%)");
        }
        
        // 优化效果
        systemLoad = max(5, systemLoad - 15);
        quantumStability = min(100.0, quantumStability + 0.5);
        
        cout << "\n>> 系统优化完成\n";
        cout << ">> 系统负载降低至: " << systemLoad << "%\n";
        cout << ">> 量子稳定性提升至: " << fixed << setprecision(2) << quantumStability << "%\n" << endl;
        
        logMessage("系统优化完成，负载: " + to_string(systemLoad) + "%, 稳定性: " + to_string(quantumStability) + "%");
    }
    
    // 新增功能：网络状态显示
    void showNetworkStatus() {
        cout << "\n[网络状态]\n";
        cout << ">> 全球节点: " << globalNodes.size() << " 在线\n";
        cout << ">> 数据传输速率: " << (1200 + rng.getRandom(0, 400)) << " TB/s\n";
        cout << ">> 网络延迟: " << (2 + rng.getRandom(0, 5)) << " 毫秒\n";
        cout << ">> 带宽使用率: " << (15 + rng.getRandom(0, 30)) << "%\n\n";
        
        cout << "节点列表:\n";
        for (const auto& node : globalNodes) {
            cout << ">> " << node.address << " (" << node.ip << ") - " 
                 << (node.online ? "在线" : "离线") 
                 << " - 稳定性: " << fixed << setprecision(1) << node.stability << "%\n";
        }
        cout << endl;
        
        logMessage("网络状态查询，在线节点: " + to_string(globalNodes.size()));
    }
    
    // 新增功能：清屏
    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        cout << "550W量子人工智能系统 v6.0 - 屏幕已清空\n";
        cout << "当前用户: " << currentUser << " | 系统代号: " << codename << "\n\n";
        
        logMessage("屏幕清空");
    }
    
    // 新增功能：保存日志到文件
    void saveLogToFile() {
        if (!loggingEnabled) {
            cout << "\n[错误] 日志系统未启用\n" << endl;
            return;
        }
        
        cout << "\n[日志] 当前日志文件: " << logFileName << endl;
        cout << ">> 日志大小: ";
        
        // 获取文件大小
        logFile.flush();
        ifstream in(logFileName, ios::binary | ios::ate);
        if (in.is_open()) {
            auto size = in.tellg();
            cout << size << " 字节" << endl;
        } else {
            cout << "未知" << endl;
        }
        in.close();
        
        cout << ">> 日志记录已持续保存\n" << endl;
        logMessage("手动日志状态查询");
    }
    
    // 量子计算模拟
    void quantumComputing(const string& args) {
        if (!quantumComputer) {
            quantumComputer.reset(new QuantumComputer());
        }
        
        if (args.empty()) {
            cout << "\n[量子计算] 可用命令:\n";
            cout << "quantum status    - 显示量子位状态\n";
            cout << "quantum hadamard [n] - 对量子位n应用Hadamard门\n";
            cout << "quantum entangle [n1] [n2] - 纠缠两个量子位\n";
            cout << "quantum measure [n] - 测量量子位\n";
            cout << "quantum reset     - 重置量子计算机\n";
            return;
        }
        
        istringstream iss(args);
        string subcmd;
        iss >> subcmd;
        
        if (subcmd == "status") {
            cout << "\n[量子计算机状态]\n";
            for (int i = 0; i < quantumComputer->qubits.size(); i++) {
                cout << "量子位 " << i << ": " << quantumComputer->getStateDescription(i) << endl;
            }
            logMessage("量子计算机状态查询");
        }
        else if (subcmd == "hadamard") {
            int qubit;
            if (iss >> qubit) {
                quantumComputer->applyHadamard(qubit);
                cout << ">> 对量子位 " << qubit << " 应用Hadamard门\n";
                logMessage("量子位操作: Hadamard门应用于量子位 " + to_string(qubit));
            }
        }
        else if (subcmd == "entangle") {
            int qubit1, qubit2;
            if (iss >> qubit1 >> qubit2) {
                quantumComputer->entangleQubits(qubit1, qubit2);
                cout << ">> 量子位 " << qubit1 << " 和 " << qubit2 << " 已纠缠\n";
                logMessage("量子纠缠: 量子位 " + to_string(qubit1) + " 和 " + to_string(qubit2));
            }
        }
        else if (subcmd == "measure") {
            int qubit;
            if (iss >> qubit) {
                string result = quantumComputer->measureQubit(qubit);
                cout << ">> 量子位 " << qubit << " 测量结果: " << result << endl;
                logMessage("量子测量: 量子位 " + to_string(qubit) + " 结果: " + result);
            }
        }
        else if (subcmd == "reset") {
            quantumComputer->initializeQubits(16);
            cout << ">> 量子计算机已重置\n";
            logMessage("量子计算机重置");
        }
        cout << endl;
    }
    
    // 系统备份
    void backupSystem() {
        cout << "\n[系统备份] 正在创建系统快照...\n";
        logMessage("开始系统备份");
        
        auto now = chrono::system_clock::now();
        auto time_t_now = chrono::system_clock::to_time_t(now);
        string timestamp = to_string(time_t_now);
        
        // 模拟备份过程
        cout << ">> 备份文件系统 (" << fileSystem.size() << " 个文件)...\n";
        sleep_ms(500);
        cout << ">> 备份任务队列 (" << taskQueue.size() << " 个任务)...\n";
        sleep_ms(500);
        cout << ">> 备份网络配置 (" << globalNodes.size() << " 个节点)...\n";
        sleep_ms(500);
        
        cout << ">> 系统备份完成 [备份ID: " << timestamp << "]\n" << endl;
        logMessage("系统备份完成，备份ID: " + timestamp);
    }
    
    // 系统恢复
    void restoreSystem() {
        cout << "\n[系统恢复] 警告：这将覆盖当前系统状态\n";
        cout << ">> 确认恢复？(y/n): ";
        string confirm;
        getline(cin, confirm);
        
        if (confirm == "y" || confirm == "Y") {
            cout << ">> 正在恢复系统...\n";
            logMessage("开始系统恢复");
            
            // 模拟恢复过程
            for (int i = 0; i < 3; i++) {
                cout << ">> 恢复阶段 " << (i+1) << "/3...\n";
                sleep_ms(600);
            }
            
            // 重置一些状态
            systemLoad = 15 + rng.getRandom(0, 20);
            coreTemp = 28 + rng.getRandom(0, 5);
            quantumStability = 99.5 + rng.getRandomDouble(0, 0.5);
            
            cout << ">> 系统恢复完成\n" << endl;
            logMessage("系统恢复完成");
        } else {
            cout << ">> 恢复操作已取消\n" << endl;
            logMessage("系统恢复取消");
        }
    }
    
    // 显示帮助信息
    void showHelp() {
        cout << "\n[可用命令]" << endl;
        vector<pair<string, string>> commands = {
            {"help", "显示此帮助"},
            {"status", "系统状态"},
            {"sysinfo", "详细系统信息"},
            {"history", "命令历史"},
            {"time", "系统时间"},
            {"diag", "系统诊断"},
            {"rename", "更改代号"},
            {"moss", "苔藓模式切换"},
            {"log", "系统日志"},
            {"ls", "列出文件"},
            {"create", "创建文件"},
            {"rm", "删除文件"},
            {"ping", "网络诊断"},
            {"threat", "威胁评估"},
            {"tasks", "任务队列"},
            {"addtask", "添加任务"},
            {"complete", "完成任务"},
            {"planets", "行星位置"},
            {"ask", "AI对话"},
            {"passwd", "修改密码"},
            {"users", "用户列表"},
            {"adduser", "添加用户"},
            {"deluser", "删除用户"},
            {"su", "切换用户"},
            {"quantum", "量子计算"},
            {"backup", "系统备份"},
            {"restore", "系统恢复"},
            {"simulate", "运行模拟"},
            {"calc", "数学计算"},
            {"encrypt", "数据加密"},
            {"decrypt", "数据解密"},
            {"optimize", "系统优化"},
            {"network", "网络状态"},
            {"clear", "清屏"},
            {"savelog", "日志状态"},
            {"exit", "关闭系统"}
        };
        
        for (const auto& cmd : commands) {
            cout << left << setw(12) << cmd.first << " - " << cmd.second << endl;
        }
        cout << endl;
        
        logMessage("帮助信息查询");
    }
    
    // 显示系统状态
    void showStatus() {
        cout << "\n[系统状态]" << endl;
        cout << "核心温度: " << coreTemp << "°C" << endl;
        cout << "量子稳定性: " << fixed << setprecision(2) << quantumStability << "%" << endl;
        cout << "全球节点: " << globalNodes.size() << " 在线" << endl;
        cout << "系统负载: " << systemLoad << "%" << endl;
        cout << "威胁等级: " << threatLevel << "/10" << endl;
        cout << "任务队列: " << taskQueue.size() << " 个待处理" << endl;
        
        if (overloadMode) cout << ">> [警告] 系统处于过载模式" << endl;
        if (quantumLinkActive) cout << ">> [状态] 量子链接已激活" << endl;
        if (quantumComputer) cout << ">> [状态] 量子计算机在线" << endl;
        
        cout << endl;
        logMessage("系统状态查询");
    }
    
    // 显示系统信息
    void displaySystemInfo() {
        auto now = chrono::system_clock::now();
        auto uptime = chrono::duration_cast<chrono::seconds>(now - startTime);
        
        cout << "\n[系统信息]" << endl;
        cout << "AI核心: 量子神经网络 v6.0 (C++11)" << endl;
        cout << "处理器: 光子计算阵列 (1.62 ZFLOPS)" << endl;
        cout << "内存: 128PB HBM6" << endl;
        cout << "运行时间: " << uptime.count() << " 秒" << endl;
        cout << "威胁等级: " << threatLevel << "/10" << endl;
        cout << "系统负载: " << systemLoad << "%" << endl;
        cout << "核心温度: " << coreTemp << "°C" << endl;
        cout << "量子稳定性: " << fixed << setprecision(2) << quantumStability << "%" << endl;
        cout << "知识图谱等级: " << knowledgeLevel << "/10" << endl;
        cout << "量子链接: " << (quantumLinkActive ? "已连接" : "未连接") << endl;
        cout << "时间旅行模拟: " << (timeTravelSimulation ? "进行中" : "未激活") << endl;
        cout << "当前用户: " << currentUser << " (" << getPrivilegeName() << ")" << endl;
        cout << endl;
        
        logMessage("系统信息查询");
    }
    
    // 显示命令历史
    void showCommandHistory() {
        cout << "\n[命令历史]" << endl;
        if (commandHistory.empty()) {
            cout << ">> 无记录" << endl;
        } else {
            for (size_t i = 0; i < commandHistory.size(); i++) {
                cout << i+1 << ". " << commandHistory[i] << endl;
            }
        }
        cout << endl;
        
        logMessage("命令历史查询，记录数: " + to_string(commandHistory.size()));
    }
    
    // 显示系统时间
    void displaySystemTime() {
        auto now = chrono::system_clock::now();
        auto time_t_now = chrono::system_clock::to_time_t(now);
        cout << "\n[系统时间] " << ctime(&time_t_now);
        
        logMessage("系统时间查询");
    }
    
    // 系统诊断
    void runDiagnostics() {
        cout << "\n[诊断] 开始系统自检..." << endl;
        logMessage("开始系统诊断");
        
        vector<string> diagItems = {
            "量子核心完整性", "光子处理器阵列", "神经网络连接", 
            "安全子系统", "全球节点网络", "预测分析引擎"
        };
        
        for (const auto& item : diagItems) {
            cout << ">> 检查: " << item << "... ";
            sleep_ms(300);
            
            int result = rng.getRandom();
            if (result > 90) cout << "[严重错误]";
            else if (result > 70) cout << "[警告]";
            else if (result > 40) cout << "[正常]";
            else cout << "[优化]";
            
            cout << " (" << result << "%)\n";
        }
        
        cout << "\n[诊断完成] 系统状态: ";
        vector<string> statuses = {"最优", "稳定", "需维护", "警告"};
        string status = statuses[rng.getRandom(0, statuses.size()-1)];
        cout << status << "\n" << endl;
        
        logMessage("系统诊断完成，状态: " + status);
    }
    
    // 重命名系统
    void renameSystem() {
        cout << "\n输入新代号: ";
        string newName;
        getline(cin, newName);
        string oldName = codename;
        codename = newName;
        cout << ">> 系统代号更新为: " << codename << "\n" << endl;
        
        logMessage("系统代号更新: " + oldName + " -> " + codename);
    }
    
    // 切换苔藓模式
    void toggleMossMode() {
        mossMode = !mossMode;
        cout << "\n//////////////////////////////////////////////////\n";
        cout << "//                  特殊协议激活                //\n";
        cout << "//            <<< 苔藓模式" << (mossMode ? "已启用" : "已禁用") << " >>>     //\n";
        cout << "//////////////////////////////////////////////////\n" << endl;
        
        logMessage("苔藓模式 " + string(mossMode ? "启用" : "禁用"));
    }
    
    // 查看系统日志
    void viewSystemLog() {
        cout << "\n[系统日志 - 最后10条]\n";
        vector<string> logs = {
            "[INFO] 系统启动完成",
            "[AI] 处理用户命令: status", 
            "[NET] 节点更新: 新增12个接入点",
            "[SEC] 安全扫描完成 - 无威胁",
            "[AI] 处理用户命令: simulate 太阳危机",
            "[HW] 核心温度稳定在28.7°C",
            "[AI] 代号更新: " + codename,
            "[NET] 数据传输速率: 1.4TB/s",
            "[AI] 处理用户命令: help",
            "[SYS] 定期自检完成 - 状态最优"
        };
        
        auto now = chrono::system_clock::now();
        for (const auto& log : logs) {
            auto time_t_now = chrono::system_clock::to_time_t(now);
            cout << ctime(&time_t_now) << " " << log << endl;
        }
        cout << endl;
        
        logMessage("系统日志查询");
    }
    
    // 文件系统操作
    void listFiles() {
        cout << "\n[量子文件系统]\n";
        if (fileSystem.empty()) {
            cout << ">> 系统存储为空\n";
        } else {
            for (const auto& file : fileSystem) {
                cout << ">> " << file.first << " - " << file.second << endl;
            }
        }
        cout << endl;
        
        logMessage("文件列表查询，文件数: " + to_string(fileSystem.size()));
    }
    
    void createFile(string filename) {
        if (filename.empty()) {
            cout << "\n输入文件名: ";
            getline(cin, filename);
        }
        
        cout << "输入文件内容: ";
        string content;
        getline(cin, content);
        
        fileSystem[filename] = content;
        cout << ">> 文件创建成功: " << filename << "\n" << endl;
        
        logMessage("文件创建: " + filename);
    }
    
    void deleteFile(string filename) {
        if (filename.empty()) {
            cout << "\n输入文件名: ";
            getline(cin, filename);
        }
        
        auto it = fileSystem.find(filename);
        if (it != fileSystem.end()) {
            fileSystem.erase(it);
            cout << ">> 文件已删除: " << filename << "\n" << endl;
            logMessage("文件删除: " + filename);
        } else {
            cout << ">> 文件未找到: " << filename << "\n" << endl;
            logMessage("文件删除失败 - 未找到: " + filename, "WARNING");
        }
    }
    
    // 网络诊断
    void pingNode(string node) {
        if (node.empty()) {
            if (globalNodes.empty()) {
                cout << "\n[错误] 没有可用节点\n" << endl;
                return;
            }
            node = globalNodes[rng.getRandom(0, globalNodes.size()-1)].address;
        }
        
        cout << "\n[网络诊断] 测试节点: " << node << endl;
        cout << ">> 发送量子数据包...\n";
        sleep_ms(500);
        
        int latency = 10 + rng.getRandom(0, 100);
        int successRate = 80 + rng.getRandom(0, 20);
        
        cout << ">> 节点响应时间: " << latency << " 毫秒\n";
        cout << ">> 连接成功率: " << successRate << "%\n";
        cout << ">> 状态: " << (successRate > 85 ? "优秀" : "良好") << "\n" << endl;
        
        logMessage("网络诊断 - 节点: " + node + ", 延迟: " + to_string(latency) + "ms, 成功率: " + to_string(successRate) + "%");
    }
    
    // 威胁评估
    void assessThreat() {
        cout << "\n[威胁评估] 正在扫描系统...\n";
        logMessage("开始威胁评估");
        for (int i = 0; i < 5; i++) {
            cout << ">> 分析维度 " << i+1 << "...\n";
            sleep_ms(300);
        }
        
        threatLevel = rng.getRandom(0, 10);
        cout << ">> 威胁评估完成\n";
        cout << ">> 当前威胁等级: " << threatLevel << "/10\n";
        
        vector<string> actions = {"继续监控", "增强防御", "隔离受影响系统", "立即介入 - 危急状态"};
        int actionIndex = min(static_cast<int>(threatLevel / 3), static_cast<int>(actions.size()-1));
        cout << ">> 建议措施: " << actions[actionIndex] << "\n" << endl;
        
        logMessage("威胁评估完成，等级: " + to_string(threatLevel) + "/10, 建议: " + actions[actionIndex]);
    }
    
    // 任务系统
    void listTasks() {
        cout << "\n[任务队列]\n";
        if (taskQueue.empty()) {
            cout << ">> 没有待处理任务\n";
        } else {
            for (size_t i = 0; i < taskQueue.size(); i++) {
                cout << i+1 << ". " << taskQueue[i].description 
                     << " [优先级: " << taskQueue[i].priority << "]\n";
            }
        }
        cout << endl;
        
        logMessage("任务列表查询，任务数: " + to_string(taskQueue.size()));
    }
    
    void addTask(string task) {
        if (task.empty()) {
            cout << "\n输入任务描述: ";
            getline(cin, task);
        }
        
        taskQueue.emplace_back(task);
        cout << ">> 任务已添加: " << task << "\n" << endl;
        
        logMessage("任务添加: " + task);
    }
    
    void completeTask() {
        if (taskQueue.empty()) {
            cout << "\n>> 没有待处理任务\n" << endl;
            return;
        }
        
        auto task = taskQueue.front();
        taskQueue.erase(taskQueue.begin());
        
        cout << "\n[任务完成] " << task.description << endl;
        cout << ">> 状态: 成功\n" << endl;
        
        logMessage("任务完成: " + task.description);
    }
    
    // 行星位置
    void displayPlanetaryAlignment() {
        vector<string> planets = {"水星", "金星", "地球", "火星", "木星", "土星", "天王星", "海王星"};
        
        cout << "\n[行星位置模拟]\n";
        cout << ">> 当前太阳系行星位置:\n";
        
        for (const auto& planet : planets) {
            double angle = rng.getRandom(0, 360);
            double distance = 0.4 + (&planet - &planets[0]) * 0.8 + rng.getRandomDouble(0, 0.1);
            
            cout << ">> " << planet << ": ";
            cout << "角度=" << angle << "° ";
            cout << "距离=" << fixed << setprecision(2) << distance << "AU\n";
        }
        cout << endl;
        
        logMessage("行星位置模拟显示");
    }
    
    // AI对话
    void aiDialogue(string question) {
        if (question.empty()) {
            cout << "\n请输入问题: ";
            getline(cin, question);
        }
        
        // 转换为小写
        transform(question.begin(), question.end(), question.begin(), ::tolower);
        
        cout << "\n[AI分析] 正在处理: \"" << question << "\"\n";
        sleep_ms(800);
        
        map<string, string> responses = {
            {"你好", "你好，" + currentUser + "。我是550W量子AI系统。"},
            {"hello", "Hello, " + currentUser + ". I am 550W Quantum AI System."},
            {"名字", "我是550W，当前代号: " + codename},
            {"目的", "我的核心使命是保障人类文明的延续。"},
            {"生命", "根据我的分析，生命的意义是42。"},
            {"未来", "未来有11,945种可能，其中87.3%的概率人类文明将继续发展。"}
        };
        
        bool found = false;
        for (const auto& pair : responses) {
            if (question.find(pair.first) != string::npos) {
                cout << ">> " << pair.second << "\n";
                found = true;
                break;
            }
        }
        
        if (!found) {
            vector<string> defaultResponses = {
                "这个问题超出了我的当前认知范围。",
                "我需要更多数据来回答这个问题。",
                "正在访问知识图谱... 未找到确切答案。",
                "重新表述您的问题可能获得更准确的回答。"
            };
            cout << ">> " << defaultResponses[rng.getRandom(0, defaultResponses.size()-1)] << "\n";
        }
        cout << endl;
        
        logMessage("AI对话: " + question);
    }
    
    // 修改密码
    void changePassword() {
        cout << "\n[安全协议] 修改访问密码\n";
        cout << ">> 当前密码: ";
        string current = getHiddenInput();
        
        bool valid = false;
        for (auto& user : users) {
            if (user.username == currentUser && user.checkPassword(current)) {
                valid = true;
                break;
            }
        }
        
        if (!valid) {
            cout << ">> 错误: 密码不匹配\n" << endl;
            logMessage("密码修改失败 - 当前密码错误", "WARNING");
            return;
        }
        
        cout << ">> 新密码: ";
        string newPass1 = getHiddenInput();
        cout << ">> 确认新密码: ";
        string newPass2 = getHiddenInput();
        
        if (newPass1 != newPass2) {
            cout << ">> 错误: 密码不匹配\n" << endl;
            logMessage("密码修改失败 - 新密码不匹配", "WARNING");
            return;
        }
        
        for (auto& user : users) {
            if (user.username == currentUser) {
                user.password = newPass1;
                break;
            }
        }
        
        cout << ">> 密码已成功更新\n" << endl;
        logMessage("密码修改成功");
    }
    
    // 用户管理
    void listUsers() {
        cout << "\n[系统用户]\n";
        for (const auto& user : users) {
            cout << ">> " << user.username << " - " << user.getPrivilegeName() << endl;
        }
        cout << endl;
        
        logMessage("用户列表查询");
    }
    
    void createUser() {
        if (currentPrivilege < 2) {
            cout << "\n[错误] 需要根用户权限\n" << endl;
            logMessage("用户创建失败 - 权限不足", "WARNING");
            return;
        }
        
        cout << "\n[创建用户]\n";
        cout << ">> 用户名: ";
        string username;
        getline(cin, username);
        
        // 检查用户是否存在
        for (const auto& user : users) {
            if (user.username == username) {
                cout << ">> 错误: 用户已存在\n" << endl;
                logMessage("用户创建失败 - 用户已存在: " + username, "WARNING");
                return;
            }
        }
        
        cout << ">> 密码: ";
        string password = getHiddenInput();
        
        cout << ">> 权限级别 (0-普通, 1-管理员, 2-根用户): ";
        int privilege;
        cin >> privilege;
        cin.ignore();
        
        users.emplace_back(username, password, privilege);
        cout << ">> 用户创建成功\n" << endl;
        
        logMessage("用户创建: " + username + " 权限: " + to_string(privilege));
    }
    
    void deleteUser() {
        if (currentPrivilege < 2) {
            cout << "\n[错误] 需要根用户权限\n" << endl;
            logMessage("用户删除失败 - 权限不足", "WARNING");
            return;
        }
        
        cout << "\n[删除用户]\n";
        cout << ">> 用户名: ";
        string username;
        getline(cin, username);
        
        if (username == "root") {
            cout << ">> 错误: 不能删除根用户\n" << endl;
            logMessage("用户删除失败 - 尝试删除root用户", "WARNING");
            return;
        }
        
        auto it = find_if(users.begin(), users.end(), 
                         [&](const User& u) { return u.username == username; });
        
        if (it != users.end()) {
            users.erase(it);
            cout << ">> 用户已删除\n" << endl;
            logMessage("用户删除: " + username);
        } else {
            cout << ">> 错误: 用户不存在\n" << endl;
            logMessage("用户删除失败 - 用户不存在: " + username, "WARNING");
        }
    }
    
    void switchUser() {
        cout << "\n[切换用户]\n";
        cout << ">> 用户名: ";
        string username;
        getline(cin, username);
        
        cout << ">> 密码: ";
        string password = getHiddenInput();
        
        for (const auto& user : users) {
            if (user.username == username && user.checkPassword(password)) {
                string oldUser = currentUser;
                currentUser = username;
                currentPrivilege = user.privilege;
                cout << ">> 用户切换成功: " << currentUser << "\n" << endl;
                logMessage("用户切换: " + oldUser + " -> " + currentUser);
                return;
            }
        }
        
        cout << ">> 错误: 用户名或密码不正确\n" << endl;
        logMessage("用户切换失败 - 认证失败: " + username, "WARNING");
    }
    
    // 运行模拟
    void runSimulation(const string& args) {
        string params = args;
        if (params.empty()) {
            cout << "\n[模拟启动] 输入任务参数: ";
            getline(cin, params);
        }
        
        cout << "\n[运行中] 使用参数: " << params << endl;
        logMessage("开始模拟运行，参数: " + params);
        
        // 使用异步任务模拟长时间运行
        auto future = async(launch::async, [this, params]() {
            for (int i = 0; i < 5; ++i) {
                sleep_ms(300);
            }
            return rng.getRandom(0, 100);
        });
        
        // 显示进度
        for (int i = 0; i < 5; ++i) {
            cout << ">> 迭代 " << i+1 << "/5" << endl;
            sleep_ms(300);
        }
        
        int result = future.get();
        
        vector<string> outcomes = {
            "成功", "部分成功", "失败", "结果不确定", "需要人工干预",
            "发现新路径", "存在潜在风险", "需要更多资源", "突破性进展"
        };
        
        string outcome = outcomes[result % outcomes.size()];
        cout << "\n[模拟完成] 结果: " << outcome << "\n" << endl;
        logMessage("模拟完成，结果: " + outcome);
    }
    
    // 数学计算
    void calculateExpression(const string& expr) {
        string expression = expr;
        if (expression.empty()) {
            cout << "\n[计算] 输入表达式 (例如: 5 + 3 或 2^3): ";
            getline(cin, expression);
        }
        
        cout << "\n[计算] 表达式: " << expression << endl;
        cout << "[处理] 量子计算核心启动...\n";
        sleep_ms(800);
        
        istringstream iss(expression);
        double num1, num2;
        char op;
        double result = 0.0;
        bool valid = true;
        
        if (iss >> num1 >> op >> num2) {
            switch(op) {
                case '+': result = num1 + num2; break;
                case '-': result = num1 - num2; break;
                case '*': result = num1 * num2; break;
                case '/': 
                    if (num2 != 0) {
                        result = num1 / num2;
                    } else {
                        valid = false;
                        cout << "[错误] 除零错误\n";
                    }
                    break;
                case '^': result = pow(num1, num2); break;
                default: 
                    valid = false;
                    cout << "[错误] 不支持的操作符\n";
            }
        } else {
            valid = false;
            cout << "[错误] 表达式格式错误\n";
        }
        
        if (valid) {
            cout << "[结果] " << result << "\n" << endl;
            logMessage("数学计算: " + expression + " = " + to_string(result));
        } else {
            cout << "[计算失败] 请检查表达式\n" << endl;
            logMessage("数学计算失败: " + expression, "WARNING");
        }
    }
    
    // 更新系统状态
    void updateSystemStatus() {
        systemLoad = 15 + rng.getRandom(0, 70);
        coreTemp = 28 + rng.getRandom(0, 5);
        quantumStability = 99.8 + rng.getRandomDouble(0, 0.2);
        threatLevel = max(0, min(10, threatLevel + rng.getRandom(-1, 1)));
        
        if (overloadMode) {
            systemLoad = min(100, systemLoad + 5);
            quantumStability = max(85.0, quantumStability - 0.5);
            
            if (rng.getRandom() == 0) {
                overloadMode = false;
                cout << "\n[系统] 过载模式已自动禁用\n" << endl;
                logMessage("过载模式自动禁用");
            }
        }
    }
    
    // 系统关闭序列
    void shutdownSequence() {
        cout << "\n[关闭协议] 发送终止信号...\n";
        logMessage("开始系统关闭序列");
        cout << ">> 开始安全关闭程序\n";
        sleep_ms(1000);
        
        cout << "\n[网络] 关闭量子节点连接...\n";
        for (auto it = globalNodes.rbegin(); it != globalNodes.rend(); ++it) {
            cout << ">> 断开节点 " << it->address << " ... [断开成功]\n";
            sleep_ms(100);
        }
        
        cout << "\n[内存] 保存系统状态...\n";
        cout << ">> [";
        for (int i = 0; i < 20; i++) {
            cout << "#";
            sleep_ms(50);
        }
        cout << "] 100%\n";
        
        cout << "\n[AI核心] 关闭认知模块...\n";
        cout << ">> 神经突触断开: ■ > ■ > ■ [断开完成]\n";
        
        cout << "\n>> 所有子系统已安全关闭\n";
        cout << ">> 量子核心冷却中...\n";
        sleep_ms(1500);
        
        logMessage("系统关闭完成");
        if (logFile.is_open()) {
            logFile.close();
        }
    }

public:
    Simulator550W(string name) : codename(name), activated(false), 
        shutdown_requested(false), debugMode(false), mossMode(false), 
        threatLevel(0), systemLoad(20), coreTemp(30), 
        quantumStability(99.9), overloadMode(false),
        selfDestructInitiated(false), selfDestructCountdown(0),
        quantumLinkActive(false), knowledgeLevel(5),
        timeTravelSimulation(false), simulationYear(2025),
        loggingEnabled(false) {
        
        startTime = chrono::system_clock::now();
        
        // 初始化用户
        users.emplace_back("root", "550W", 2);
        users.emplace_back("admin", "admin123", 1);
        users.emplace_back("user", "password", 0);
        currentUser = "root";
        currentPrivilege = 2;
        
        // 初始化文件系统
        fileSystem["mission.log"] = "流浪地球计划 - 阶段4";
        fileSystem["quantum_data.qd"] = "加密量子态数据";
        fileSystem["ai_core.cfg"] = "神经网络配置 v6.0";
        
        // 初始化任务队列
        taskQueue.emplace_back("系统诊断");
        taskQueue.emplace_back("安全扫描");
        taskQueue.emplace_back("节点优化");
        
        // 初始化神经网络
        neuralNetworks = {"基础推理网络", "语言处理网络", "预测分析网络"};
        
        // 初始化命令映射
        initializeCommandMap();
    }
    
    ~Simulator550W() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    void activate() {
        if (activated) return;
        
        cout << "\n[启动协议] 550W人工智能系统 v6.0 (C++11)\n";
        
        // 用户登录
        bool loggedIn = false;
        int attempts = 3;
        
        while (!loggedIn && attempts > 0) {
            cout << "[生物识别] 请输入用户名: ";
            string username;
            getline(cin, username);
            
            cout << "[生物识别] 请输入密码: ";
            string password = getHiddenInput();
            
            for (const auto& user : users) {
                if (user.username == username && user.checkPassword(password)) {
                    currentUser = username;
                    currentPrivilege = user.privilege;
                    loggedIn = true;
                    break;
                }
            }
            
            if (!loggedIn) {
                attempts--;
                cout << "[错误] 认证失败 (" << attempts << "次尝试剩余)\n";
                if (attempts == 0) {
                    cout << "\n[安全协议] 系统锁定 - 请联系管理员\n";
                    exit(1);
                }
            }
        }
        
        cout << "[验证] 身份确认...";
        sleep_ms(1000);
        cout << "通过\n";
        cout << "[激活] 正在启动量子计算核心...\n";
        sleep_ms(1500);
        
        // 初始化日志系统
        initializeLogging();
        
        displayBootSequence();
        activated = true;
        
        assessThreat();
    }
    
    void processCommand(const string& cmd) {
        if (cmd.empty()) return;
        
        // 更新系统状态
        updateSystemStatus();
        
        // 保存命令历史
        commandHistory.push_back(cmd);
        if (commandHistory.size() > 50) {
            commandHistory.erase(commandHistory.begin());
        }
        
        // 记录命令到日志
        logMessage("用户命令: " + cmd);
        
        // 检查特殊命令
        if (cmd == "exit") {
            shutdownSequence();
            cout << "\n550W系统离线 - 量子核心安全关闭\n" << endl;
            shutdown_requested = true;
            return;
        }
        
        // 使用命令映射表处理命令
        bool commandFound = false;
        string baseCmd = cmd;
        string args = "";
        
        size_t spacePos = cmd.find(' ');
        if (spacePos != string::npos) {
            baseCmd = cmd.substr(0, spacePos);
            args = cmd.substr(spacePos + 1);
        }
        
        for (const auto& pair : commandMap) {
            if (pair.first == baseCmd) {
                pair.second(args);
                commandFound = true;
                break;
            }
        }
        
        if (!commandFound) {
            vector<string> responses = {
                "请重新表述您的查询",
                "命令未被识别",
                "需要更多上下文信息",
                "该指令在当前上下文中不可用",
                "建议使用 'help' 查看可用命令"
            };
            string response = responses[rng.getRandom(0, responses.size()-1)];
            cout << "\n[响应] " << response << "\n" << endl;
            logMessage("未知命令: " + cmd + " - 响应: " + response, "WARNING");
        }
    }
    
    bool shouldShutdown() const {
        return shutdown_requested;
    }
};

int main() {
    cout << "========================================\n";
    cout << "      550W量子人工智能系统 v6.0\n";
    cout << "        C++11重制版 - 量子核心\n";
    cout << "========================================\n";
    
    Simulator550W moss("MOSS");
    moss.activate();

    string command;
    while (!moss.shouldShutdown()) {
        cout << "550W@";
        
        auto now = chrono::system_clock::now();
        auto time_t_now = chrono::system_clock::to_time_t(now);
        tm* timeinfo = localtime(&time_t_now);
        
        printf("%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        cout << "> ";
        
        getline(cin, command);
        moss.processCommand(command);
    }

    return 0;
}
