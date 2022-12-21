CXX = cl
LD = link
OPTIMIZE = /O2
SRC = stdafx.cpp MemoryPie.cpp MemoryPie.rc
OBJ = stdafx.obj MemoryPie.obj MemoryPie.res
CFLAGS = $(OPTIMIZE) /O2 /GL /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /EHsc /MT /Fp"MemoryPie.pch" /Fo".\\" /Fd"vc80.pdb" /W3 /nologo /Wp64 /Zi /TP /errorReport:prompt
LDFLAGS = /OUT:"MemoryPie.exe" /INCREMENTAL:NO /NOLOGO /MANIFEST /MANIFESTFILE:"MemoryPie.exe.manifest" /PDB:"MemoryPie.pdb" /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /ENTRY:"mainCRTStartup" /MACHINE:X86 /ERRORREPORT:PROMPT kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
.SUFFIXES: .cpp .obj .rc .res

.cpp.obj:
	$(CXX) /c $(CFLAGS) $*.cpp

.rc.res:
	$(RC) /fo $*.res $*.rc

MemoryPie.exe:	$(OBJ)
	$(LD) $(LDFLAGS) $(OBJ)

all:	MemoryPie.exe

install:	MemoryPie.exe
	copy MemoryPie.exe ..\..\..\Windows

clean:
	del $(OBJ) MemoryPie.exe *.manifest *.idb *.pdb
