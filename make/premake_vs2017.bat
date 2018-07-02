@echo off

set PREMAKE_BASE=..\tools\premake5
set PREMAKE=%PREMAKE_BASE%\premake5.exe 

%PREMAKE% --file=_VulkanAppSolution.lua vs2017
pause