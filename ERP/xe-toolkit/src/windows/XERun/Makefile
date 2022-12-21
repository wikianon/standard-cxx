CXX = cl
LD = link
OPTIMIZE = /O2
SRC = stdafx.cpp XERun.cpp XERun.rc
OBJ = stdafx.obj XERun.obj XERun.res
CFLAGS = $(OPTIMIZE) /O2 /GL /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /EHsc /MT /Fp"XERun.pch" /Fo".\\" /Fd"vc80.pdb" /W3 /nologo /Wp64 /Zi /TP /errorReport:prompt
LDFLAGS = /OUT:"XERun.exe" /INCREMENTAL:NO /NOLOGO /MANIFEST /MANIFESTFILE:"XERun.exe.manifest" /PDB:"XERun.pdb" /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /LTCG /MACHINE:X86 /ERRORREPORT:PROMPT kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
.SUFFIXES: .cpp .obj .rc .res

.cpp.obj:
	$(CXX) /c $(CFLAGS) $*.cpp

.rc.res:
	$(RC) /fo $*.res $*.rc

XERun.exe:	$(OBJ)
	$(LD) $(LDFLAGS) $(OBJ)

all:	XERun.exe

install:	XERun.exe
	copy XERun.exe ..\..\..\Windows\CacheStat.exe
	copy XERun.exe ..\..\..\Windows\Disks.exe
	copy XERun.exe ..\..\..\Windows\Explain.exe
	copy XERun.exe ..\..\..\Windows\IoStat.exe
	copy XERun.exe ..\..\..\Windows\MpStat.exe
	copy XERun.exe ..\..\..\Windows\NicStat.exe
	copy XERun.exe ..\..\..\Windows\OSFlavor.exe
	copy XERun.exe ..\..\..\Windows\SysInfo.exe
	copy XERun.exe ..\..\..\Windows\SysStat.exe
	copy XERun.exe ..\..\..\Windows\TcpStat.exe
	copy XERun.exe ..\..\..\Windows\VirtualGuru.exe

clean:
	del $(OBJ) XERun.exe *.manifest *.idb *.pdb
