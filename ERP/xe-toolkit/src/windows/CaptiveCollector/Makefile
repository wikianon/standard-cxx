CXX = cl
LD = link
OPTIMIZE = /O2
SRC = stdafx.cpp CaptiveCollector.cpp CaptiveCollector.rc
OBJ = stdafx.obj CaptiveCollector.obj CaptiveCollector.res
CFLAGS = $(OPTIMIZE) /GL /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /EHsc /MT /W3 /nologo /Wp64 /Zi /TP /errorReport:prompt
LDFLAGS = /OUT:"CaptiveCollector.exe" /INCREMENTAL:NO /NOLOGO /MANIFEST /MANIFESTFILE:"CaptiveCollector.exe.manifest" /PDB:"CaptiveControl.pdb" /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /LTCG /ENTRY:"mainCRTStartup" /MACHINE:X86 /ERRORREPORT:PROMPT wsock32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
.SUFFIXES: .cpp .obj .rc .res

.cpp.obj:
	$(CXX) /c $(CFLAGS) $*.cpp

.rc.res:
	$(RC) /fo $*.res $*.rc

CaptiveCollector.exe:	$(OBJ)
	$(LD) $(LDFLAGS) $(OBJ)

all:	CaptiveCollector.exe

install:	CaptiveCollector.exe
	copy CaptiveCollector.exe ..\..\..\System

clean:
	del $(OBJ) CaptiveCollector.exe *.manifest *.idb *.pdb
