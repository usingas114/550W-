#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <chrono>
#include <random>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

// 时间工具函数
time_t getCurrentTime() {
    return time(nullptr);
}

std::string formatTime(time_t t) {
    std::tm now;
    localtime_s(&now, &t);
    std::stringstream ss;
    ss << std::put_time(&now, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// 基础传感器类
class Sensor {
protected:
    std::string name;
    bool active;

public:
    Sensor(const std::string& name) : name(name), active(true) {}
    virtual ~Sensor() = default;
    
    virtual bool checkStatus() = 0;
    std::string getName() const { return name; }
    bool isActive() const { return active; }
    void setActive(bool status) { active = status; }
};

// 门状态传感器
class DoorSensor : public Sensor {
private:
    bool doorOpen;

public:
    DoorSensor() : Sensor("主门状态传感器"), doorOpen(false) {}
    
    bool checkStatus() override {
        return doorOpen;
    }
    
    void openDoor() {
        doorOpen = true;
        std::cout << "[" << formatTime(getCurrentTime()) << "] 主门被打开!" << std::endl;
    }
    
    void closeDoor() {
        doorOpen = false;
        std::cout << "[" << formatTime(getCurrentTime()) << "] 主门已关闭" << std::endl;
    }
};

// 生命体征监测器
class VitalSignsMonitor : public Sensor {
private:
    int heartRate;
    bool retinaFocus;
    double alphaWave;
    bool isAlive;

public:
    VitalSignsMonitor() : Sensor("生命体征监测"), heartRate(72), retinaFocus(true), alphaWave(9.5), isAlive(true) {}
    
    bool checkStatus() override {
        return !isAlive || heartRate < 40 || heartRate > 180 || !retinaFocus || alphaWave < 7.0 || alphaWave > 13.0;
    }
    
    void setHeartRate(int rate) {
        heartRate = rate;
    }
    
    void setRetinaFocus(bool focus) {
        retinaFocus = focus;
    }
    
    void setAlphaWave(double wave) {
        alphaWave = wave;
    }
    
    void setAlive(bool alive) {
        isAlive = alive;
    }
    
    void simulateStress() {
        heartRate = 120;
        alphaWave = 6.5;
        std::cout << "[" << formatTime(getCurrentTime()) << "] 生理监测: 压力反应 - 心率升高, α波异常" << std::endl;
    }
    
    void calmDown() {
        heartRate = 72;
        alphaWave = 9.5;
        std::cout << "[" << formatTime(getCurrentTime()) << "] 生理监测: 恢复正常状态" << std::endl;
    }
};

// 信号发射器
class SignalTransmitter : public Sensor {
private:
    bool transmitting;

public:
    SignalTransmitter() : Sensor("信号发射器持续脉冲"), transmitting(true) {}
    
    bool checkStatus() override {
        return !transmitting;
    }
    
    void stopTransmitting() {
        transmitting = false;
        std::cout << "[" << formatTime(getCurrentTime()) << "] 信号发射器停止工作!" << std::endl;
    }
    
    void startTransmitting() {
        transmitting = true;
        std::cout << "[" << formatTime(getCurrentTime()) << "] 信号发射器恢复工作" << std::endl;
    }
};

// 地理围栏
class Geofence : public Sensor {
private:
    double currentDistance;
    const double maxDistance = 20.0;

public:
    Geofence() : Sensor("地理围栏"), currentDistance(0.0) {}
    
    bool checkStatus() override {
        return currentDistance > maxDistance;
    }
    
    void setDistance(double distance) {
        currentDistance = distance;
        if (checkStatus()) {
            std::cout << "[" << formatTime(getCurrentTime()) << "] 地理围栏警报: 移动距离超过20米!" << std::endl;
        }
    }
};

// 电源监测
class PowerMonitor : public Sensor {
private:
    bool mainPower;
    bool backupPower;

public:
    PowerMonitor() : Sensor("电源完整性监测"), mainPower(true), backupPower(false) {}
    
    bool checkStatus() override {
        return !mainPower && !backupPower;
    }
    
    void cutMainPower() {
        mainPower = false;
        activateBackup();
        std::cout << "[" << formatTime(getCurrentTime()) << "] 主电源被切断!" << std::endl;
    }
    
    void restoreMainPower() {
        mainPower = true;
        backupPower = false;
        std::cout << "[" << formatTime(getCurrentTime()) << "] 主电源恢复" << std::endl;
    }
    
    void activateBackup() {
        backupPower = true;
        std::cout << "[" << formatTime(getCurrentTime()) << "] 备用电源激活 - 镁箔燃烧产生干扰" << std::endl;
    }
};

// 空气分析器
class AirAnalyzer : public Sensor {
private:
    double anesthesiaGas;
    double tearGas;
    const double threshold = 0.1;

public:
    AirAnalyzer() : Sensor("空气颗粒物成分分析"), anesthesiaGas(0.0), tearGas(0.0) {}
    
    bool checkStatus() override {
        return anesthesiaGas > threshold || tearGas > threshold;
    }
    
    void detectAnesthesia(double level) {
        anesthesiaGas = level;
        if (checkStatus()) {
            std::cout << "[" << formatTime(getCurrentTime()) << "] 空气分析: 检测到麻醉气体!" << std::endl;
        }
    }
    
    void detectTearGas(double level) {
        tearGas = level;
        if (checkStatus()) {
            std::cout << "[" << formatTime(getCurrentTime()) << "] 空气分析: 检测到催泪剂!" << std::endl;
        }
    }
};

// 压感地板
class PressureFloor : public Sensor {
private:
    std::vector<bool> pressurePoints;
    bool intrusionDetected;

public:
    PressureFloor(int points = 100) : Sensor("压感地板格阵列"), pressurePoints(points, false), intrusionDetected(false) {}
    
    bool checkStatus() override {
        return intrusionDetected;
    }
    
    void triggerPressurePoint(int index) {
        if (index >= 0 && index < pressurePoints.size()) {
            pressurePoints[index] = true;
            detectIntrusion();
        }
    }
    
    void detectIntrusion() {
        // 模拟检测到入侵路径
        int triggered = 0;
        for (bool point : pressurePoints) {
            if (point) triggered++;
        }
        
        if (triggered > 5) {
            intrusionDetected = true;
            std::cout << "[" << formatTime(getCurrentTime()) << "] 压感地板: 检测到入侵路径!" << std::endl;
        }
    }
};

// 化学物质类
class ChemicalSubstance {
protected:
    std::string name;
    double temperature;
    bool isStable;

public:
    ChemicalSubstance(const std::string& name, double temp) : name(name), temperature(temp), isStable(true) {}
    
    virtual ~ChemicalSubstance() = default;
    virtual void heat(double degrees) = 0;
    virtual void cool(double degrees) = 0;
    virtual bool isDangerous() = 0;
    
    std::string getName() const { return name; }
    double getTemperature() const { return temperature; }
    bool getStability() const { return isStable; }
};

// 硝化甘油
class Nitroglycerin : public ChemicalSubstance {
private:
    const double volatilizationTemp = 8.0;
    const double unstableTemp = 50.0;

public:
    Nitroglycerin() : ChemicalSubstance("硝化甘油", -10.0) {}
    
    void heat(double degrees) override {
        temperature += degrees;
        checkStability();
    }
    
    void cool(double degrees) override {
        temperature -= degrees;
        checkStability();
    }
    
    bool isDangerous() override {
        return temperature > unstableTemp;
    }
    
    void checkStability() {
        isStable = temperature <= unstableTemp;
        if (!isStable) {
            std::cout << "[" << formatTime(getCurrentTime()) << "] 硝化甘油: 温度过高 - 不稳定!" << std::endl;
        }
    }
};

// 铝热剂
class Thermite : public ChemicalSubstance {
private:
    double burnRate;
    const double ignitionTemp = 2500.0;

public:
    Thermite() : ChemicalSubstance("铝热剂", 25.0), burnRate(0.0) {}
    
    void heat(double degrees) override {
        temperature += degrees;
        checkIgnition();
    }
    
    void cool(double degrees) override {
        temperature -= degrees;
    }
    
    bool isDangerous() override {
        return burnRate > 0.0;
    }
    
    void checkIgnition() {
        if (temperature >= ignitionTemp) {
            burnRate = 120.0; // 米/秒
            isStable = false;
            std::cout << "[" << formatTime(getCurrentTime()) << "] 铝热剂: 点燃! 燃烧波速度: " << burnRate << "m/s" << std::endl;
        }
    }
    
    double getBurnRate() const { return burnRate; }
};

// 临界协议系统
class CriticalProtocol {
private:
    std::vector<Sensor*> sensors;
    std::vector<ChemicalSubstance*> chemicals;
    bool isArmed;
    bool isTriggered;
    time_t armTime;
    
    // 传感器指针方便直接访问
    DoorSensor* doorSensor;
    VitalSignsMonitor* vitalSigns;
    SignalTransmitter* signalTransmitter;
    Geofence* geofence;
    PowerMonitor* powerMonitor;
    AirAnalyzer* airAnalyzer;
    PressureFloor* pressureFloor;
    
    // 化学物质指针
    Nitroglycerin* nitroglycerin;
    Thermite* thermite;

public:
    CriticalProtocol() : isArmed(false), isTriggered(false) {
        // 初始化传感器
        doorSensor = new DoorSensor();
        vitalSigns = new VitalSignsMonitor();
        signalTransmitter = new SignalTransmitter();
        geofence = new Geofence();
        powerMonitor = new PowerMonitor();
        airAnalyzer = new AirAnalyzer();
        pressureFloor = new PressureFloor();
        
        sensors.push_back(doorSensor);
        sensors.push_back(vitalSigns);
        sensors.push_back(signalTransmitter);
        sensors.push_back(geofence);
        sensors.push_back(powerMonitor);
        sensors.push_back(airAnalyzer);
        sensors.push_back(pressureFloor);
        
        // 初始化化学物质
        nitroglycerin = new Nitroglycerin();
        thermite = new Thermite();
        
        chemicals.push_back(nitroglycerin);
        chemicals.push_back(thermite);
    }
    
    ~CriticalProtocol() {
        for (Sensor* sensor : sensors) delete sensor;
        for (ChemicalSubstance* chemical : chemicals) delete chemical;
    }
    
    void arm() {
        isArmed = true;
        isTriggered = false;
        armTime = getCurrentTime();
        
        for (Sensor* sensor : sensors) {
            sensor->setActive(true);
        }
        
        std::cout << "[" << formatTime(getCurrentTime()) << "] 临界协议: 系统已激活 - 精密的钟表机芯开始运转" << std::endl;
    }
    
    void disarm() {
        isArmed = false;
        isTriggered = false;
        
        std::cout << "[" << formatTime(getCurrentTime()) << "] 临界协议: 系统已解除" << std::endl;
    }
    
    bool checkTrigger() {
        if (!isArmed || isTriggered) return false;
        
        for (Sensor* sensor : sensors) {
            if (sensor->isActive() && sensor->checkStatus()) {
                trigger();
                return true;
            }
        }
        
        // 检查化学物质状态
        for (ChemicalSubstance* chemical : chemicals) {
            if (chemical->isDangerous()) {
                trigger();
                return true;
            }
        }
        
        return false;
    }
    
    void trigger() {
        isTriggered = true;
        
        std::cout << "\n[" << formatTime(getCurrentTime()) << "] === 临界协议触发! ===" << std::endl;
        std::cout << "[" << formatTime(getCurrentTime()) << "] 系统响应: 铝热剂引燃 -> 硝化甘油起爆 -> 连锁反应启动" << std::endl;
        std::cout << "[" << formatTime(getCurrentTime()) << "] 预测后果: 2500°C高温, 冲击波传播, 化学能完全释放" << std::endl;
        std::cout << "[" << formatTime(getCurrentTime()) << "] 系统状态: 不可逆触发, 倒计时终止\n" << std::endl;
    }
    
    // 获取系统状态
    std::string getStatus() {
        std::stringstream ss;
        ss << "\n=== 临界协议系统状态 ===\n";
        ss << "激活状态: " << (isArmed ? "已激活" : "未激活") << std::endl;
        ss << "触发状态: " << (isTriggered ? "已触发" : "未触发") << std::endl;
        ss << "激活时间: " << formatTime(armTime) << std::endl;
        ss << "\n传感器状态:\n";
        
        for (Sensor* sensor : sensors) {
            ss << "- " << sensor->getName() << ": " << (sensor->isActive() ? "正常" : "失效") << 
              (sensor->checkStatus() ? " [异常]" : "") << std::endl;
        }
        
        ss << "\n化学物质状态:\n";
        for (ChemicalSubstance* chemical : chemicals) {
            ss << "- " << chemical->getName() << ": 温度=" << chemical->getTemperature() << "°C, 稳定性=" << 
              (chemical->getStability() ? "稳定" : "不稳定") << std::endl;
        }
        
        ss << "========================\n";
        return ss.str();
    }
    
    // 提供外部访问接口
    DoorSensor* getDoorSensor() { return doorSensor; }
    VitalSignsMonitor* getVitalSignsMonitor() { return vitalSigns; }
    SignalTransmitter* getSignalTransmitter() { return signalTransmitter; }
    Geofence* getGeofence() { return geofence; }
    PowerMonitor* getPowerMonitor() { return powerMonitor; }
    AirAnalyzer* getAirAnalyzer() { return airAnalyzer; }
    PressureFloor* getPressureFloor() { return pressureFloor; }
    Nitroglycerin* getNitroglycerin() { return nitroglycerin; }
    Thermite* getThermite() { return thermite; }
};

// 模拟系统运行
void simulateProtocol() {
    CriticalProtocol protocol;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);
    
    // 模拟陈昭博士设置系统
    std::cout << "[" << formatTime(getCurrentTime()) << "] 陈昭博士: 开始设置临界协议系统" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    std::cout << "[" << formatTime(getCurrentTime()) << "] 陈昭博士: 准备硝化甘油 - 冷却至-10°C" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::cout << "[" << formatTime(getCurrentTime()) << "] 陈昭博士: 配置铝热剂 - 添加硝酸钡和超细镁粉" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::cout << "[" << formatTime(getCurrentTime()) << "] 陈昭博士: 设置触发条件 - 七扇区传感器网络已就绪" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // 激活系统
    protocol.arm();
    std::cout << protocol.getStatus() << std::endl;
    
    // 模拟时间流逝
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    // 模拟B国部队入侵
    std::cout << "[" << formatTime(getCurrentTime()) << "] 外部动静: 爆炸声接近, 震动强度增加" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    std::cout << "[" << formatTime(getCurrentTime()) << "] B国工兵: 开始切割防爆门" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    // 门被打开
    protocol.getDoorSensor()->openDoor();
    
    // 检查触发条件
    if (protocol.checkTrigger()) {
        std::cout << "[" << formatTime(getCurrentTime()) << "] 陈昭博士: 协议已触发, 准备谈判" << std::endl;
    }
    
    // 模拟谈判过程
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "[" << formatTime(getCurrentTime()) << "] 陈昭博士: 这不是虚张声势, 系统已完全激活" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "[" << formatTime(getCurrentTime()) << "] B国铁砧: 我们接受协议, 放下武器" << std::endl;
    
    // 模拟撤离
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "[" << formatTime(getCurrentTime()) << "] B国部队: 开始撤离" << std::endl;
    
    // 模拟拆除过程
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "[" << formatTime(getCurrentTime()) << "] 陈昭博士: 开始拆除 - 物理隔离, 低温维持, 化学惰化" << std::endl;
    
    // 关闭系统
    std::this_thread::sleep_for(std::chrono::seconds(3));
    protocol.disarm();
    std::cout << protocol.getStatus() << std::endl;
    
    std::cout << "[" << formatTime(getCurrentTime()) << "] 陈昭博士: 拆除完成, 实验室恢复三级安全状态" << std::endl;
    std::cout << "[" << formatTime(getCurrentTime()) << "] 陈昭博士: 所有受限材料安全, 实验数据完整" << std::endl;
}

int main() {
    std::cout << "=======================" << std::endl;
    std::cout << "临界协议模拟系统 v0.1" << std::endl;
    std::cout << "作者: 陈昭 (模拟)" << std::endl;
    std::cout << "日期: 2033年11月7日" << std::endl;
    std::cout << "=======================" << std::endl;
    
    simulateProtocol();
    
    std::cout << "\n模拟结束。" << std::endl;
    return 0;
}