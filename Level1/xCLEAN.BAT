@echo off
SET EXT3DEL= *.i *.#* *.EXE *.OBJ *.TDS *.~* *.ERR *.CGL *.DDP *.CSM *.DCU
echo cleaning...
FOR /F "usebackq delims==" %%i IN (`dir /b /w /s %EXT3DEL%`) DO if exist %%i erase %%i |echo %%i
echo .......done
