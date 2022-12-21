CXX = cl
LD = link
OPTIMIZE = /O2
SRC = jmdr.cpp mdr.cpp mdr_category.cpp mdr_category_t.cpp mdr_exception.cpp mdr_perf_counter.cpp mdr_perf_host.cpp mdr_performance_counter_t.cpp mdr_performance_host_t.cpp mdr_single.cpp mdr_tuple.cpp reg_query.cpp
OBJ = jmdr.obj mdr.obj mdr_category.obj mdr_category_t.obj mdr_exception.obj mdr_perf_counter.obj mdr_perf_host.obj mdr_performance_counter_t.obj mdr_performance_host_t.obj mdr_single.obj mdr_tuple.obj reg_query.obj
CFLAGS = $(OPTIMIZE) /GL /I "C:\Program Files\Java\jdk1.6.0\include" /I "C:\Program Files\Java\jdk1.6.0\include\win32" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "JMDR_EXPORTS" /D "_WINDLL" /FD /EHsc /MD /Fo".\\" /Fd"vc80.pdb" /W3 /nologo /Zi /TP /errorReport:prompt
LDFLAGS = /OUT:"jmdr.dll" /INCREMENTAL:NO /NOLOGO /DLL /MANIFEST /MANIFESTFILE:"jmdr.dll.manifest" /PDB:"jmdr.pdb" /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /MACHINE:X86 /ERRORREPORT:PROMPT
LIBS = wsock32.lib Pdh.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
.SUFFIXES: .cpp .obj .rc .res

.cpp.obj:
	$(CXX) /c $(CFLAGS) $*.cpp

jmdr.dll:	$(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) $(LIBS)
	mt -nologo -manifest jmdr.dll.manifest -outputresource:jmdr.dll;2

all:	jmdr.dll

install:	jmdr.dll
	copy jmdr.dll ..\..\..\lib\windows\x86

clean:
	del $(OBJ) jmdr.dll *.manifest *.idb *.pdb *.exp *.lib
