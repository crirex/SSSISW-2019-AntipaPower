@setlocal
 
@if "%1"=="" (
  @echo ERROR: Argument expected
  @echo ERROR: Please supply the relative folder where the CMake output sits
  @echo ERROR: Bailing out ..
  @exit /B -1
)
 
@echo Changing location to "%~f1" ..
@cd "%~f1"
@if %ERRORLEVEL% NEQ 0 (
  @echo Folder "%~f1" does not exist. Will be created.
  @mkdir %~f1
  @cd "%~f1"
)
@pwd
 
@del /s /f /q .\*
@for /f %%f in ('dir /ad /b .\') do @(
  @rmdir /s /q .\%%f
)