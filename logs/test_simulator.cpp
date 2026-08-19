#include "simulator550w.h"

int main() {
    // 创建模拟器实例
    Simulator550W simulator(TSTR("550W"));
    
    // 激活模拟器
    simulator.activate();
    
    // 执行一些基本命令
    simulator.processCommand(TSTR("help"));
    simulator.processCommand(TSTR("status"));
    
    // 测试文件系统
    vector<tstring> files = simulator.getFiles();
    tcout << TSTR("\n文件系统:") << tendl;
    for (const auto& file : files) {
        tcout << TSTR("  ") << file << tendl;
    }
    
    // 测试任务列表
    vector<tstring> tasks = simulator.getTasks();
    tcout << TSTR("\n任务列表:") << tendl;
    for (const auto& task : tasks) {
        tcout << TSTR("  ") << task << tendl;
    }
    
    tcout << TSTR("\n测试完成!\n") << tendl;
    return 0;
}