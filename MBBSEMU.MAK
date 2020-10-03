# mbbsemu Makefile to build MBBSEMU.DLL

.autodepend
.path.c=c:\wgserv\src
.path.h=c:\wgserv\src
.path.dll=c:\wgserv
.path.msg=c:\wgserv
.path.obj=c:\wgserv\phobj

mbbsemu.dll : mbbsmain.obj submod.obj topmenu.obj float.obj echo.obj timer.obj fsdata.obj fsedit.obj btrieve.obj \wgserv\dlib\galimp.lib \run286\bc3\lib\phapi.lib
        ltdll mbbsemu

btrieve.obj : btrieve.c submod.h mbbsemu.h
        ctdll btrieve

fsdata.obj : fsdata.c fsdata.h globals.h submod.h mbbsemu.h
        ctdll fsdata

fsedit.obj : fsedit.c fsedit.h globals.h submod.h mbbsemu.h
        ctdll fsedit

timer.obj : timer.c timer.h globals.h submod.h mbbsemu.h
        ctdll timer

echo.obj : echo.c echo.h submod.h mbbsemu.h
        ctdll echo

submod.obj : submod.c submod.h globals.h
        ctdll submod

float.obj : float.c submod.h
        ctdll float

topmenu.obj : topmenu.c topmenu.h submod.h mbbsemu.h
        ctdll topmenu

mbbsmain.obj : mbbsmain.c mbbsemu.h submod.h globals.h topmenu.h float.h echo.h timer.h fsdata.h fsedit.h btrieve.h
        ctdll mbbsmain

mbbsemu.h: mbbsemu.msg
        cd c:\WGSERV
        bbsmsx mbbsemu.msg -osrc
        cd c:\WGSERV\SRC
