@echo off
setlocal enabledelayedexpansion

:input_loop
:: Prompt for the number of processes
set /p loop_count=Enter how many processes to run: 

:: Validate if loop_count is a valid number
for /f "delims=0123456789" %%a in ("%loop_count%") do (
    echo Invalid input. Please enter a valid number for processes.
    goto input_loop
)

:input_iter
:: Prompt for the number of iterations
set /p iter=Enter how many iterations for each process: 

:: Validate if iter is a valid number
for /f "delims=0123456789" %%a in ("%iter%") do (
    echo Invalid input. Please enter a valid number for iterations.
    goto input_iter
)
REM Get the directory where the batch file is located
set "batch_dir=%~dp0"
set "output_dir=%batch_dir%process_outputs"
mkdir %output_dir%

REM Initialize variables
set /a total=0


REM Start the processes concurrently
for /l %%i in (1,1,%loop_count%) do (
    REM Use a unique filename for each process to avoid conflicts
    start "Process_%%i" /b cmd /c "CPU-Process.exe %iter%
)

REM Optionally, wait for all processes to finish
REM timeout /t 30 >nul

IF %iter% LEQ 500 (
    timeout /t 10 >nul
) ELSE IF %iter% LEQ 1000 (
    timeout /t 30 >nul
) ELSE IF %iter% LEQ 5000 (
    timeout /t 60 >nul
) ELSE (
    timeout /t 200 >nul
)



REM Loop through each file in the directory
for %%f in ("%output_dir%\*") do (
    REM Read the numeric value from each file
    for /f "delims=" %%a in (%%f) do (
        set /a value=%%a
        set /a total+=value
    )
)

for /f "delims=" %%a in ('powershell -command "[math]::Round([double](!total!) / [double](!loop_count!), 2)"') do set "average_time=%%a"

echo Average execution time: %average_time% ms

REM Remove a Directory and Its Contents
rd /s /q "%output_dir%"

endlocal
