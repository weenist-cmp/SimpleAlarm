::SimpleAlarm_make_tcc.bat
:: 2019.10.03. Make with Tiny C Compiler
::
::

@echo off

:: 현재 폴더 저장
set this_DIR=%CD%

:: resource file .rc .res
set this_FN_RC=SimpleAlarm_res

:: main c source .c
set this_FN_CM=SimpleAlarm


:: lib def 더 추가해도 exe dll 결과물의 크기가 늘어나지않는걸로 봐서는 사용한 함수들만 링크하는걸로 보인다
:: tcc에만 없는것 (mingw에선 std)
set this_FN_DF0=\_PB\tcc\lib\Shell32.def
::\_PB\tcc\lib\Advapi32.def

:: 공통으로 없는것
set this_FN_DF1=\_PB\tcc\lib\winmm.def
::\_PB\tcc\lib\gdi32.def \_PB\tcc\lib\wsock32.def \_PB\tcc\lib\user32.def \_PB\tcc\lib\Advapi32.def
::comctl32
::shlwapi
::comdlg32

:: program name  .exe
set this_FN_FR=SimpleAlarm

:: 폴더 변경
cd \MinGW\bin\

echo ...Resource Compile (%this_FN_RC%.res)
windres.exe -i %this_DIR%\%this_FN_RC%.rc -o %this_DIR%\%this_FN_RC%.res -J rc -O coff

:: 현재 폴더로 복귀
cd %this_DIR%

if not exist %this_FN_RC%.res goto err

echo ...C Source Compile (%this_FN_CM%.o %this_FN_C1%.o)
d:\_PB\tcc\tcc.exe -Wall -O2 -ID:\_PB\tcc\include -c %this_FN_CM%.c -o %this_FN_CM%.o
if not exist %this_FN_CM%.o goto err

echo ...Delete (%this_FN_FR%.exe)
Del ..\%this_FN_FR%.exe

echo ...Build (%this_FN_FR%.exe)
d:\_PB\tcc\tcc.exe -mwindows -LD:\_PB\tcc\lib -o ..\%this_FN_FR%.exe %this_FN_CM%.o %this_FN_RC%.res %this_FN_DF0% %this_FN_DF1%
::-rdynamic 
if not exist ..\%this_FN_FR%.exe goto err

echo.
echo.
echo ...OK !!
goto end

:err
echo.
echo.
echo ...Error !! 

:end
set this_DIR=
set this_FN_RC=
set this_FN_CM=
set this_FN_DF0=
set this_FN_DF1=
set this_FN_FR=
pause
