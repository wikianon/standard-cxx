CXX = cl
LD = link
OPTIMIZE = /Ox
SRC = CCdirect.cpp ProcSlob.cpp WinProcess.cpp stdafx.cpp
OBJ = CCdirect.obj ProcSlob.obj WinProcess.obj stdafx.obj
CFLAGS = $(OPTIMIZE) /GL /I "C:\Program Files\Java\jdk1.6.0\include" /I "C:\Program Files\Java\jdk1.6.0\include\win32" /I "..\..\jmdr\windows" /I "..\..\plugins\CaptiveCollector\windows" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "CCDIRECT_EXPORTS" /D "_WINDLL" /D "_MBCS" /FD /EHsc /MD /Fp"CCdirect.pch" /Fo".\\" /Fd"vc80.pdb" /W3 /nologo /Wp64 /Zi /TP /errorReport:prompt
LDFLAGS = /OUT:"CCdirect.dll" /INCREMENTAL:NO /NOLOGO /LIBPATH:"..\..\plugins\CaptiveCollector\windows" /LIBPATH:"..\..\jmdr\windows" /DLL /MANIFEST /MANIFESTFILE:"CCdirect.dll.manifest" /PDB:"CCdirect.pdb" /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /MACHINE:X86 /ERRORREPORT:PROMPT
LIBS = jmdr.lib CCwinplugin.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
.SUFFIXES: .cpp .obj .rc .res
.cpp.obj:
	$(CXX) /c $(CFLAGS) $*.cpp

CCdirect.dll:	$(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) $(LIBS)
	mt -nologo -manifest CCdirect.dll.manifest -outputresource:CCdirect.dll;2

all:	CCdirect.dll

install:	CCdirect.dll
	copy CCdirect.dll ..\..\..\lib\windows\x86

clean:
	del $(OBJ) CCdirect.dll *.manifest *.idb *.pdb *.exp *.lib
