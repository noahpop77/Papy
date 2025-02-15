@echo off
setlocal enabledelayedexpansion

:: Change to script directory
pushd "%~dp0"

:: Default values
set THREADS=1
set RUNS=4
set COUNT=100

:: Parse command-line arguments
:parse_args
if "%~1"=="" goto run_benchmark
if "%~1"=="-threads" (set THREADS=%~2 & shift & shift & goto parse_args)
if "%~1"=="-runs" (set RUNS=%~2 & shift & shift & goto parse_args)
if "%~1"=="-count" (set COUNT=%~2 & shift & shift & goto parse_args)
echo Unknown option: %~1
popd
exit /b 1

:run_benchmark
:: Kill any existing Node servers first
taskkill /IM node.exe /F > nul 2>&1

:: Install dependencies
echo Installing Node.js dependencies...
call :install_dependencies
if errorlevel 1 (
    echo Failed to install dependencies
    popd
    exit /b 1
)

echo Starting Node.js server...
start "PapyServer" /B node nodejs_server.js
call :get_node_pid

echo Checking server status...
call :wait_for_server
if errorlevel 1 (
    echo Failed to start server
    popd
    exit /b 1
)

:: Function to benchmark Papy
call :benchmark "..\bin\dev\papy.exe"
call :benchmark "..\bin\rel\papy.exe"

:: Cleanup
echo Stopping Node.js server...
taskkill /PID !NODE_PID! /F > nul 2>&1
popd
exit /b

:install_dependencies
:: Check if node_modules exists
if exist "node_modules" (
    echo node_modules already exists, skipping install
    exit /b 0
)

:: Run npm install
echo Running npm install...
npm install --silent
if errorlevel 1 (
    echo npm install failed
    exit /b 1
)
exit /b 0

:benchmark
set PAPY_PATH=%~1
set TOTAL_TIME=0
echo Benchmarking %PAPY_PATH% for %RUNS% runs (Threads: %THREADS%, Count: %COUNT%)...

for /L %%i in (1,1,%RUNS%) do (
    set "startTime=!time!"
    %PAPY_PATH% --threads %THREADS% --target "http://127.0.0.1:5000" --endpoint "/benchmark" --count %COUNT% >nul 2>&1
    set "stopTime=!time!"
    
    call :time_diff TIME_TAKEN startTime stopTime
    set /A TOTAL_TIME+=TIME_TAKEN
    echo Run #%%i: !TIME_TAKEN! ms
)

echo Total time for %PAPY_PATH%: %TOTAL_TIME% ms
echo ==================================
exit /b

:get_node_pid
set NODE_PID=
for /f "tokens=5" %%a in (
    'netstat -ano ^| findstr ":5000 " ^| findstr "LISTENING"'
) do set NODE_PID=%%a
exit /b

:wait_for_server
echo Waiting for server to start...
set /a attempts=0
:retry
powershell -Command "$response = try { Invoke-WebRequest -Uri http://127.0.0.1:5000/benchmark -UseBasicParsing -TimeoutSec 5 -ErrorAction Stop } catch {}; if ($response.StatusCode -eq 200) { exit 0 } else { exit 1 }"
if %errorlevel% equ 0 (
    echo Server is ready!
    exit /b 0
)
set /a attempts+=1
if !attempts! geq 10 (
    echo Failed to start server after 10 attempts
    exit /b 1
)
timeout /t 1 > nul
goto retry

:time_diff
setlocal
call :time_to_ms time1 "%~2"
call :time_to_ms time2 "%~3"
set /a diff=time2-time1
(
  ENDLOCAL
  set "%~1=%diff%"
  goto :eof
)

:time_to_ms
SETLOCAL EnableDelayedExpansion
FOR /F "tokens=1,2,3,4 delims=:,.^ " %%a IN ("!%~2!") DO (
  set /a "ms=(((30%%a%%100)*60+7%%b)*60+3%%c-42300)*1000+(1%%d0 %% 1000)"
)
(
  ENDLOCAL
  set %~1=%ms%
  goto :eof
)