cls
@echo off
@echo *******************************************************************************
@echo * D‚veloppement TETRIS	                                                    *
@echo *******************************************************************************
if %1.==e. goto edit
if %1.==c. goto compile
if %1.==all. goto compall
if %1.==d. goto deb
me
:compile
nmake tetris.mak
goto exec

:compall
nmake tetris.mak
goto exec

:deb
wd /swap /vga50 /in=setup.dbg tetris.exe

:exec
if errorlevel 1 goto fin
tetris

goto fin
:edit
me
cls
:fin
