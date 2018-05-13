@echo off

SET /A errno=0

echo time now: %time%
set PROJECT_DIR=%~dp0
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cd %PROJECT_DIR%
echo time now: %time%

set GLEW_INCLUDE_PATH="%USERPROFILE%\Downloads\glew-2.0.0-win32\glew-2.0.0\include"
set GLEW_LIB_PATH="%USERPROFILE%\Downloads\glew-2.0.0-win32\glew-2.0.0\lib\Release\x64"
set GLFW_INCLUDE_PATH="%USERPROFILE%\Downloads\glfw-3.2.1.bin.WIN64\glfw-3.2.1.bin.WIN64\include"
set GLFW_LIB_PATH="%USERPROFILE%\Downloads\glfw-3.2.1.bin.WIN64\glfw-3.2.1.bin.WIN64\lib-vc2015"

IF NOT EXIST build mkdir build
pushd build
echo time now: %time%
cl -FC -Zc:strictStrings -Zi -EHsc ..\hello_win32.cpp -I %GLEW_INCLUDE_PATH% -I %GLFW_INCLUDE_PATH% /link %GLEW_LIB_PATH%\glew32.lib %GLFW_LIB_PATH%\glfw3.lib opengl32.lib glu32.lib %GLFW_LIB_PATH%\glfw3dll.lib /SUBSYSTEM:CONSOLE
IF %ERRORLEVEL% NEQ 0 SET /A errno=%ERRORLEVEL%
echo time now: %time%
popd

EXIT /B %errno%
