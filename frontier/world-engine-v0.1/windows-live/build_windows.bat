@echo off
setlocal

if not exist build mkdir build
if not exist dist mkdir dist

rc /nologo /fo build\resources.res resources.rc
if errorlevel 1 exit /b 1

cl /nologo /std:c++17 /EHsc /O2 /MT /W4 /WX ^
  /c world2_network_dragon.cpp /Fobuild\world2_network_dragon.obj
if errorlevel 1 exit /b 1

link /nologo /SUBSYSTEM:WINDOWS build\world2_network_dragon.obj build\resources.res ^
  gdiplus.lib gdi32.lib iphlpapi.lib user32.lib bcrypt.lib shell32.lib ole32.lib ^
  /OUT:dist\World2NetworkDragon.exe
if errorlevel 1 exit /b 1

echo Built dist\World2NetworkDragon.exe
