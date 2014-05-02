@echo off
echo %time%
@echo on
..\Release\DES.EXE -D 'PA$$word' .\Shakespeare.DES .\Shakespeare.OUT
@echo off
echo %time%
PAUSE