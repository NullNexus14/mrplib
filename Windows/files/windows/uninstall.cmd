@echo off
echo Uninstalling mrplib...
reg delete "HKEY_CLASSES_ROOT\.mrp" /f 2>nul
reg delete "HKEY_CLASSES_ROOT\application/x-mrp" /f 2>nul
rmdir /S /Q "%ProgramFiles%\mrplib" 2>nul
echo Done.
