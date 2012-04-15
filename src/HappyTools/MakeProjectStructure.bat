@echo off 
echo -----------------------------------
echo - Happy Project Structure Creater -
echo -----------------------------------
set /P name="Project Name = "
mkdir %name%
mkdir %name%\bin
mkdir %name%\bin(x64)
mkdir %name%\build
mkdir %name%\code
mkdir %name%\data