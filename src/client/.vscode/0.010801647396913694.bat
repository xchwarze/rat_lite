@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat" -no_logo -arch=x86 || exit
echo CL := %CL%
echo _CL_ := %_CL_%
echo INCLUDE := %INCLUDE%
echo LIBPATH := %LIBPATH%
echo LINK := %LINK%
echo _LINK_ := %_LINK_%
echo LIB := %LIB%
echo PATH := %PATH%
echo TMP := %TMP%
echo FRAMEWORKDIR := %FRAMEWORKDIR%
echo FRAMEWORKDIR64 := %FRAMEWORKDIR64%
echo FRAMEWORKVERSION := %FRAMEWORKVERSION%
echo FRAMEWORKVERSION64 := %FRAMEWORKVERSION64%
echo UCRTCONTEXTROOT := %UCRTCONTEXTROOT%
echo UCRTVERSION := %UCRTVERSION%
echo UNIVERSALCRTSDKDIR := %UNIVERSALCRTSDKDIR%
echo VCINSTALLDIR := %VCINSTALLDIR%
echo VCTARGETSPATH := %VCTARGETSPATH%
echo WINDOWSLIBPATH := %WINDOWSLIBPATH%
echo WINDOWSSDKDIR := %WINDOWSSDKDIR%
echo WINDOWSSDKLIBVERSION := %WINDOWSSDKLIBVERSION%
echo WINDOWSSDKVERSION := %WINDOWSSDKVERSION%