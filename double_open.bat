@echo off
if "%1"=="" (
    echo �����뵱ǰ�Ŀͻ���Ŀ¼
    echo �÷�: double_open.bat �ͻ���Ŀ¼
    pause
    exit /b 1
)

mkdir %1_2
cd %1_2
mklink /j Assets ..\%1\Assets
mklink /j Packages ..\%1\Packages
mklink /j ProjectSettings ..\%1\ProjectSettings
echo �����µĿͻ���Ŀ¼���,�뽫Ŀ¼����unity hub