CXX = cl
LD = link
OPTIMIZE = /O2
SRC = stdafx.cpp Top.cpp Top.rc
OBJ = stdafx.obj Top.obj Top.res
CFLAGS = $(OPTIMIZE) /O2 /GL /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /EHsc /MT /Fp"Top.pch" /Fo".\\" /Fd"vc80.pdb" /W3 /nologo /Wp64 /Zi /TP /errorReport:prompt
LDFLAGS = /OUT:"Top.exe" /INCREMENTAL:NO /NOLOGO /MANIFEST /MANIFESTFILE:"Top.exe.manifest" /PDB:"Top.pdb" /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /ENTRY:"mainCRTStartup" /MACHINE:X86 /ERRORREPORT:PROMPT kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
.SUFFIXES: .cpp .obj .rc .res

.cpp.obj:
	$(CXX) /c $(CFLAGS) $*.cpp

.rc.res:
	$(RC) /fo $*.res $*.rc

Top.exe:	$(OBJ)
	$(LD) $(LDFLAGS) $(OBJ)

all:	Top.exe

install:	Top.exe
	copy Top.exe ..\..\..\Windows

clean:
	del $(OBJ) Top.exe *.manifest *.idb *.pdb
