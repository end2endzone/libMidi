@echo off

:: Validate appveyor's environment
if "%APPVEYOR_BUILD_FOLDER%"=="" (
  echo Please define 'APPVEYOR_BUILD_FOLDER' environment variable.
  exit /B 1
)

set GTEST_ROOT=%APPVEYOR_BUILD_FOLDER%\third_parties\googletest\install
set rapidassist_DIR=%APPVEYOR_BUILD_FOLDER%\third_parties\RapidAssist\install
set INSTALL_LOCATION=%APPVEYOR_BUILD_FOLDER%\install

echo ============================================================================
echo Generating...
echo ============================================================================
cd /d %APPVEYOR_BUILD_FOLDER%
mkdir build >NUL 2>NUL
cd build
cmake -DCMAKE_INSTALL_PREFIX=%INSTALL_LOCATION% -DLIBMIDI_BUILD_TEST=ON -DBUILD_SHARED_LIBS=OFF ..
if %errorlevel% neq 0 exit /b %errorlevel%

echo ============================================================================
echo Compiling...
echo ============================================================================
cmake --build . --config %Configuration%
if %errorlevel% neq 0 exit /b %errorlevel%
echo.

echo ============================================================================
echo Installing into %INSTALL_LOCATION%
echo ============================================================================
cmake --build . --config %Configuration% --target INSTALL
if %errorlevel% neq 0 exit /b %errorlevel%
echo.

echo ============================================================================
echo Creating install package
echo ============================================================================
cmake --build . --config %Configuration% --target PACKAGE
if %errorlevel% neq 0 exit /b %errorlevel%
echo.

::Delete all temporary environment variable created
set GTEST_ROOT=
set rapidassist_DIR=
set INSTALL_LOCATION=

::Return to launch folder
cd /d %~dp0
