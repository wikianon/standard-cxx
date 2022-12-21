CXX = cl
LD = link
OPTIMIZE = /O2
SRC = CCwinplugin.cpp DiskInfoJNI.cpp NetifInfoJNI.cpp SystemInfo.cpp TcpMibInfoJNI.cpp diskinfo.cpp netifinfo.cpp stdafx.cpp sysinfo.cpp tcp_mib.cpp
OBJ = CCwinplugin.obj DiskInfoJNI.obj NetifInfoJNI.obj SystemInfo.obj TcpMibInfoJNI.obj diskinfo.obj netifinfo.obj stdafx.obj sysinfo.obj tcp_mib.obj
CFLAGS = $(OPTIMIZE) /GL /I "..\..\..\jmdr\windows" /I "C:\Program Files\Java\jdk1.6.0\include" /I "C:\Program Files\Java\jdk1.6.0\include\win32" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "CCWINPLUGIN_EXPORTS" /D "_WINDLL" /D "_MBCS" /FD /EHsc /MD /Fp"CCwinplugin.pch" /Fo".\\" /Fd"vc80.pdb" /W3 /nologo /Wp64 /Zi /TP /errorReport:prompt
LDFLAGS = /OUT:"CCwinplugin.dll" /INCREMENTAL:NO /NOLOGO /DLL /MANIFEST /MANIFESTFILE:"CCwinplugin.dll.manifest" /PDB:"CCwinplugin.pdb" /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /MACHINE:X86 /ERRORREPORT:PROMPT
LIBS = IpHlpApi.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
.SUFFIXES: .cpp .obj .rc .res

.cpp.obj:
	$(CXX) /c $(CFLAGS) $*.cpp

CCwinplugin.dll:	$(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) $(LIBS)
	mt -nologo -manifest CCwinplugin.dll.manifest -outputresource:CCwinplugin.dll;2

all:	CCwinplugin.dll

install:	CCwinplugin.dll
	copy CCwinplugin.dll ..\..\..\..\lib\windows\x86

clean:
	del $(OBJ) CCwinplugin.dll *.manifest *.idb *.pdb *.exp *.lib
