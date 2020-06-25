# mbbsemu Makefile to build MBBSEMU.DLL

.autodepend
.path.c=c:\wgserv\src
.path.h=c:\wgserv\src
.path.dll=c:\wgserv
.path.msg=c:\wgserv
.path.obj=c:\wgserv\phobj

mbbsemu.dll : mbbsmain.obj submod.obj topmenu.obj echo.obj timer.obj fsdata.obj fsedit.obj \wgserv\dlib\galimp.lib \run286\bc3\lib\phapi.lib
        ltdll mbbsemu

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

topmenu.obj : topmenu.c topmenu.h submod.h mbbsemu.h
        ctdll topmenu

mbbsmain.obj : mbbsmain.c mbbsemu.h submod.h globals.h topmenu.h echo.h timer.h fsdata.h fsedit.h
        ctdll mbbsmain

mbbsemu.h: mbbsemu.msg
        cd c:\WGSERV
        bbsmsx mbbsemu.msg -osrc
        cd c:\WGSERV\SRC
