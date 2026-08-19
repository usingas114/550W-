QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 550W_Qt
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    simulator550w.cpp \
    login.cpp

HEADERS += \
    mainwindow.h \
    simulator550w.h \
    login.h

FORMS += \
    mainwindow.ui \
    login.ui

RESOURCES += resources.qrc

# 设置C++标准
CONFIG += c++11

# 添加Windows依赖
win32 {
    LIBS += -luser32 -lgdi32 -lcomctl32
}
