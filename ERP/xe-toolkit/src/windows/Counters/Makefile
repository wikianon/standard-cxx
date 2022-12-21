CXX = cl
LD = link
OPTIMIZE = /O2
SRC = stdafx.cpp Counters.cpp Counters.rc
OBJ = stdafx.obj Counters.obj Counters.res
CFLAGS = $(OPTIMIZE) /O2 /GL /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /EHsc /MT /Fp"Counters.pch" /Fo".\\" /Fd"vc80.pdb" /W3 /nologo /Wp64 /Zi /TP /errorReport:prompt
LDFLAGS = /OUT:"Counters.exe" /INCREMENTAL:NO /NOLOGO /MANIFEST /MANIFESTFILE:"Counters.exe.manifest" /PDB:"Counters.pdb" /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /ENTRY:"mainCRTStartup" /MACHINE:X86 /ERRORREPORT:PROMPT kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
.SUFFIXES: .cpp .obj .rc .res

.cpp.obj:
	$(CXX) /c $(CFLAGS) $*.cpp

.rc.res:
	$(RC) /fo $*.res $*.rc

Counters.exe:	$(OBJ)
	$(LD) $(LDFLAGS) $(OBJ)

all:	Counters.exe

install:	Counters.exe
	copy Counters.exe ..\..\..\Windows

clean:
	del $(OBJ) Counters.exe *.manifest *.idb *.pdb
