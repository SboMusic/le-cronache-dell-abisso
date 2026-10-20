@echo off
setlocal
set "FRONTIERE_DATA_DIR=%LOCALAPPDATA%\ChroniclesOfTheAbyss-2-preview"
if not exist "%~dp0gioco\ChroniclesOfTheAbyss.exe" (
 echo Anteprima 2.0 non compilata. Questo ramo contiene per ora documenti e sorgenti della base 1.0.
 pause
 exit /b 1
)
start "Le Cronache dell Abisso - sviluppo 2.0" /D "%~dp0gioco" "%~dp0gioco\ChroniclesOfTheAbyss.exe"
