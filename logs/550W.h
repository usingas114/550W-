// 550W.h: 定义应用程序的主要功能

#pragma once

#include "framework.h"
#include "simulator550w.h"

// 全局模拟器对象
extern Simulator550W* g_simulator;

// 控件句柄
extern HWND g_hWndMain;
extern HWND g_hWndCommandEdit;
extern HWND g_hWndCommandHistory;
extern HWND g_hWndOutput;
extern HWND g_hWndSystemLoad;
extern HWND g_hWndCoreTemp;
extern HWND g_hWndQuantumStability;
extern HWND g_hWndThreatLevel;
extern HWND g_hWndNeuralNetworks;
extern HWND g_hWndTasks;

// 函数声明
BOOL                CreateMainWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK    MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL                CreateStatusBar(HWND hWnd);
BOOL                CreateToolBar(HWND hWnd);
BOOL                CreateCommandArea(HWND hWnd);
BOOL                CreateOutputArea(HWND hWnd);
BOOL                CreateSystemMonitor(HWND hWnd);
BOOL                CreateNeuralNetworkView(HWND hWnd);
BOOL                CreateTaskView(HWND hWnd);

// 功能实现函数
void                ExecuteCommand(const TCHAR* command);
void                UpdateSystemMonitor();
void                UpdateNeuralNetworksView();
void                UpdateTaskView();
void                RunDiagnostics();
void                ShutdownSystem();
void                ActivateQuantumLink();
void                DeactivateQuantumLink();
void                AddTask();
void                CompleteTask();
void                AIDialogue();
void                ShowAbout();
void                ChangePassword();
