@echo off
echo %time%
@echo on
..\Release\DES.EXE -E "PA$$word" .\Shakespeare.txt .\Shakespeare.DES
@echo off
echo %time%
PAUSE