@echo off

set PROJECT_DIR=%~dp0
cd %PROJECT_DIR%
Powershell.exe -File build_and_run.ps1
