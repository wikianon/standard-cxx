// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the JMDR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// JMDR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef JMDR_EXPORTS
#define JMDR_API __declspec(dllexport)
#else
#define JMDR_API __declspec(dllimport)
#endif

// This class is exported from the jmdr.dll
class JMDR_API Cjmdr {
public:
	Cjmdr(void);
	// TODO: add your methods here.
};

extern JMDR_API int njmdr;

JMDR_API int fnjmdr(void);
