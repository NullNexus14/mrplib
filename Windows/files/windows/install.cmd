@echo off
echo Installing mrplib for Windows...
mkdir "%ProgramFiles%\mrplib" 2>nul
copy /Y libmrplib.dll "%ProgramFiles%\mrplib\"
copy /Y mrp-info.exe "%ProgramFiles%\mrplib\"
copy /Y mrp-check.exe "%ProgramFiles%\mrplib\"
copy /Y mrp-register.exe "%ProgramFiles%\mrplib\"
regedit /s mrp.reg
setx PATH "%PATH%;%ProgramFiles%\mrplib"
echo Done.
