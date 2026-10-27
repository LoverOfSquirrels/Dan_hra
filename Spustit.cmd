@echo off
chcp 65001 >nul
cd /d "%~dp0"
if exist "hra\vila_rubin.exe" (
    "hra\vila_rubin.exe"
) else if exist "build\Release\vila_rubin.exe" (
    "build\Release\vila_rubin.exe"
) else if exist "build\vila_rubin.exe" (
    "build\vila_rubin.exe"
) else (
    echo Chybi sestavena hra. Postup sestaveni je v README.md.
)
pause
