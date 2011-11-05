
@rem @if exist "HappyCooker\HappyCookerGUIv2\HappyCookerGUIv2\bin\Release\HappyCookerGUIv2.exe" goto end
@call :GetVSInstallDir
@call :build
@goto end


@REM -----------------------------------------------------------------------
:GetVSInstallDir
@set VSINSTALLDIR=
@call :GetVSInstallDirHelper64 HKLM > nul 2>&1
@if errorlevel 1 call :GetVSInstallDirHelper64 HKCU > nul 2>&1
@if errorlevel 1 call :GetVSInstallDirHelper32  HKLM > nul 2>&1
@if errorlevel 1 call :GetVSInstallDirHelper32  HKCU > nul 2>&1
@exit /B 0

:GetVSInstallDirHelper32
@for /F "tokens=1,2*" %%i in ('reg query "%1\SOFTWARE\Microsoft\VisualStudio\SxS\VS7" /v "10.0"') DO (
	@if "%%i"=="10.0" (
		@SET "VSINSTALLDIR=%%k"
	)
)
@if "%VSINSTALLDIR%"=="" exit /B 1
@exit /B 0

:GetVSInstallDirHelper64
@for /F "tokens=1,2*" %%i in ('reg query "%1\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7" /v "10.0"') DO (
	@if "%%i"=="10.0" (
		@SET "VSINSTALLDIR=%%k"
	)
)
@if "%VSINSTALLDIR%"=="" exit /B 1
@exit /B 0


@REM -----------------------------------------------------------------------
:build
"%VSINSTALLDIR%Common7\IDE\devenv" "HappyCooker\HappyCookerGUIv2\HappyCookerGUIv2\HappyCookerGUIv2.sln" /build Release 
@exit /B 0

@REM -----------------------------------------------------------------------
:end
cd HappyCooker/HappyCookerGUIv2/HappyCookerGUIv2/bin/Release
start HappyCookerGUIv2.exe