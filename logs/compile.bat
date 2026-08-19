@echo off
REM 设置环境变量
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"

REM 编译项目
cl /DUNICODE /D_UNICODE /I. /I"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\include" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\ucrt" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\shared" /link /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\lib\x86" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x86" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\ucrt\x86" comctl32.lib gdi32.lib user32.lib kernel32.lib winmm.lib 550w.cpp simulator550w.cpp
