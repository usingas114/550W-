// framework.h: 定义应用程序的全局函数和变量

#pragma once

#include <windows.h>
#include <tchar.h>

// 全局变量
extern HINSTANCE hInst;

// 窗口类名
extern const TCHAR szWindowClass[];

// 应用程序标题
extern const TCHAR szTitle[];

// 对话框 ID
#define IDD_ABOUTBOX 103

// 菜单 ID
#define IDM_ABOUT 100
#define IDM_EXIT 101
#define IDM_STATUS 102
#define IDM_DIAGNOSTICS 103
#define IDM_SHUTDOWN 104
#define IDM_ADDTASK 105
#define IDM_COMPLETETASK 106
#define IDM_ACTIVATEQUANTUMLINK 107
#define IDM_DEACTIVATEQUANTUMLINK 108
#define IDM_AIDIALOGUE 109
#define IDM_UPGRADEKNOWLEDGE 110
#define IDM_CHANGEPASSWORD 111
#define IDM_SYSTEMINFO 112

// 工具栏按钮 ID
#define IDC_BUTTON1 1000
#define IDC_BUTTON2 1001
#define IDC_BUTTON3 1002
#define IDC_BUTTON4 1003
#define IDC_BUTTON5 1004
#define IDC_BUTTON6 1005
#define IDC_BUTTON7 1006
#define IDC_BUTTON8 1007
#define IDC_BUTTON9 1008
#define IDC_BUTTON10 1009

// 状态栏 ID
#define IDC_STATIC  -1
#define IDC_SYSLOAD 2000
#define IDC_CORETEMP 2001
#define IDC_QUANTUMSTABILITY 2002
#define IDC_THREATLEVEL 2003

// 函数声明
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    MainWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL                CreateMainControls(HWND hWnd);
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
void                ChangePassword();