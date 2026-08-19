#ifndef SIMULATOR550W_H
#define SIMULATOR550W_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <map>
#include <iomanip>
#include <cstdio>
#include <fstream>
#include <cctype>

using namespace std;

// 用户结构体
struct User {
    string username;
    string password;
    int privilege; // 0:普通用户, 1:管理员, 2:超级用户

    User(const string& u, const string& p, int priv)
        : username(u), password(p), privilege(priv) {
    }
};

class Simulator550W {
private:
    bool activated;
    string codename;
    vector<string> commandHistory;
    bool debugMode;
    bool mossMode;
    int threatLevel;
    map<string, string> fileSystem;
    vector<string> taskQueue;
    vector<string> globalNodes;
    vector<string> connectedNodes;
    int systemLoad;
    int coreTemp;
    double quantumStability;
    bool overloadMode;
    bool selfDestructInitiated;
    int selfDestructCountdown;
    vector<User> users;
    string currentUser;
    int currentPrivilege;
    bool quantumLinkActive;
    int knowledgeLevel;
    vector<string> neuralNetworks;
    bool timeTravelSimulation;
    int simulationYear;

    // 私有辅助方法
    string getHiddenInput();
    void buildQuantumNetwork();
    void displayBootSequence();
    void shutdownSequence();
    void displaySystemTime();
    void runDiagnostics();
    void calculateExpression(const string& expr);
    void showCommandHistory();
    void displaySystemInfo();
    string getPrivilegeName();
    void easterEgg(const string& cmd);
    void viewSystemLog();
    void listFiles();
    void createFile(string filename, string content = "");
    void deleteFile(string filename);
    void pingNode(string node = "");
    void assessThreat();
    void listTasks();
    void addTask(string task);
    void completeTask();
    void displayPlanetaryAlignment();
    void aiDialogue(string question);
    void updateSystemStatus();
    void changePassword();
    void selfDestructSequence();
    void listUsers();
    void createUser();
    void deleteUser();
    void switchUser();

public:
    Simulator550W(string name);
    string getCodename();
    void activate();
    void processCommand(string cmd);
    string getCurrentUser() const { return currentUser; }
    int getCurrentPrivilege() const { return currentPrivilege; }
    int getSystemLoad() const { return systemLoad; }
    int getCoreTemp() const { return coreTemp; }
    double getQuantumStability() const { return quantumStability; }
    int getThreatLevel() const { return threatLevel; }
    bool isQuantumLinkActive() const { return quantumLinkActive; }
    bool isOverloadMode() const { return overloadMode; }
    bool isSelfDestructInitiated() const { return selfDestructInitiated; }
    int getSelfDestructCountdown() const { return selfDestructCountdown; }
    vector<string> getCommandHistory() const { return commandHistory; }
    vector<string> getTasks() const { return taskQueue; }
    vector<string> getFiles() const;
    vector<string> getGlobalNodes() const { return globalNodes; }
    vector<string> getNeuralNetworks() const { return neuralNetworks; }
    int getActiveNodes() const { return connectedNodes.size(); }
    int getTotalNodes() const { return globalNodes.size(); }
    string getSystemStatus() const;
    bool authenticateUser(const string& username, const string& password, User& user);
};

#endif // SIMULATOR550W_H
#pragma once
