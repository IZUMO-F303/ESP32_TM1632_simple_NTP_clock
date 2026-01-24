@echo off
set "PWS_PATH=C:\Program Files\PowerShell\7\pwsh.exe"

if not exist "%PWS_PATH%" (
    echo PowerShell 7が見つかりませんでした。
    echo.
    echo 以下のパスを確認してください:
    echo %PWS_PATH%
    pause
    exit /b
)

start "" "%PWS_PATH%" -NoExit -Command "Set-Location -LiteralPath '%CD%'"