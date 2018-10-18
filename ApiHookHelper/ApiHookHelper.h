// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the APIHOOKHELPER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// APIHOOKHELPER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef APIHOOKHELPER_EXPORTS
#define APIHOOKHELPER_API __declspec(dllexport)
#else
#define APIHOOKHELPER_API __declspec(dllimport)
#endif

#include <Windows.h>
#include "Win32Process.h"
class CTString;

typedef void(*ProcessMonitorCallback)(void* caller, int event, Win32_Process process);

extern "C"
{
	APIHOOKHELPER_API void* MonitorAllProcessCreation(ProcessMonitorCallback callback);
	APIHOOKHELPER_API void* MonitorProcessCreation(char* strProcessName, ProcessMonitorCallback callback);
	APIHOOKHELPER_API BOOL InjectDLL(DWORD dwPID, CTString dllPath);
	APIHOOKHELPER_API BOOL AdvInjectDLL(DWORD dwPID, CTString dllPath);
}

