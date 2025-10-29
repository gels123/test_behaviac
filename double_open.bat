@echo off
if "%1"=="" (
    echo 请输入当前的客户端目录
    echo 用法: double_open.bat 客户端目录
    pause
    exit /b 1
)

mkdir %1_2
cd %1_2
mklink /j Assets ..\%1\Assets
mklink /j Packages ..\%1\Packages
mklink /j ProjectSettings ..\%1\ProjectSettings
echo 创建新的客户端目录完成,请将目录导入unity hub