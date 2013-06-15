@echo off
set /p vc12=Install for Visual Studio 12? (y/n): 
installers\win\Assimp.exe
installers\win\Awesomium.exe
IF /I %vc12% == y (
installers\win\Boost-msvc11.exe
installers\win\PhysX-msvc11.exe
installers\win\RakNet-msvc11.exe
installers\win\SFML2.0-msvc11.exe
) ELSE (
installers\win\Boost.exe
installers\win\PhysX.exe
installers\win\RakNet.exe
installers\win\SFML2.0.exe
)
installers\win\DevIL.exe
installers\win\FreeType.exe
installers\win\glew.exe
installers\win\lbsndfile.exe
installers\win\OpenAl.exe
installers\win\Cairo.exe
installers\win\fontconfig.exe
installers\win\LibOVR.exe
