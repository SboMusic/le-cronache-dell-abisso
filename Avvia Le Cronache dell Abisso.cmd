@echo off
if not exist "%~dp0gioco\Frontiere.exe" (
 echo Game not found. Restore the gioco folder from the backup.
 pause
 exit /b 1
)
start "Le Cronache dell Abisso" "%~dp0gioco\Frontiere.exe"
