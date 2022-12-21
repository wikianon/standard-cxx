CXX = cl
LD = link
OPTIMIZE = /O2
SRC = stdafx.cpp CpuGraph.cpp CpuGraph.rc
OBJ = stdafx.obj CpuGraph.obj CpuGraph.res
CFLAGS = $(OPTIMIZE) /O2 /GL /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /EHsc /MT /Fp"CpuGraph.pch" /Fo".\\" /Fd"vc80.pdb" /W3 /nologo /Wp64 /Zi /TP /errorReport:prompt
LDFLAGS = /OUT:"CpuGraph.exe" /INCREMENTAL:NO /NOLOGO /MANIFEST /MANIFESTFILE:"CpuGraph.exe.manifest" /PDB:"CpuGraph.pdb" /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /ENTRY:"mainCRTStartup" /MACHINE:X86 /ERRORREPORT:PROMPT kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
.SUFFIXES: .cpp .obj .rc .res

.cpp.obj:
	$(CXX) /c $(CFLAGS) $*.cpp

.rc.res:
	$(RC) /fo $*.res $*.rc

CpuGraph.exe:	$(OBJ)
	$(LD) $(LDFLAGS) $(OBJ)

all:	CpuGraph.exe

install:	CpuGraph.exe
	copy CpuGraph.exe ..\..\..\Windows

clean:
	del $(OBJ) CpuGraph.exe *.manifest *.idb *.pdb
