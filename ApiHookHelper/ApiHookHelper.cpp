// ApiHookHelper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ApiHookHelper.h"
#include "ProcessMonitor.h"
#include <TlHelp32.h>
#include <vector>
using std::vector;

APIHOOKHELPER_API void* MonitorAllProcessCreation(ProcessMonitorCallback callback)
{
	return myEventSink::registerCreationCallback("", callback);
}


APIHOOKHELPER_API void* MonitorProcessCreation(char* strProcessName, ProcessMonitorCallback callback)
{
	return myEventSink::registerCreationCallback(strProcessName, callback);
}

BOOL SetPrivilege(
	HANDLE hToken,          // access token handle
	LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
	BOOL bEnablePrivilege   // to enable or disable privilege
)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		return 11;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		return 12;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
		return 1;
	}

	return 0;
}

APIHOOKHELPER_API BOOL InjectDLL(DWORD dwPID, CTString dllPath)
{
	//http://www.coderbag.com/Threading/DLL-Injection-Using-Remote-Thread
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	if (processHandle == NULL)
	{
		return 1;
	}

	HMODULE dllHandle = GetModuleHandle(dllPath.ToWString());
	HANDLE loadLibraryAddress = GetProcAddress(dllHandle, "LoadLibraryW");

	if (loadLibraryAddress == NULL)
	{
		//error
		return 2;
	}

	HANDLE baseAddress = VirtualAllocEx(
		processHandle,
		NULL,
		256,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE);

	if (baseAddress == NULL)
	{
		return 3;
	}

	BOOL isSucceeded = WriteProcessMemory(
		processHandle,
		baseAddress,
		dllPath.ToWString(),
		dllPath.length() + 1,
		NULL);
	if (isSucceeded == 0)
	{
		return 4;
	}
	HANDLE threadHandle = CreateRemoteThread(
		processHandle,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)loadLibraryAddress,
		baseAddress,
		NULL,
		0);
	if (threadHandle != NULL)
	{
		return 5;
	}
	return 0;
}

bool FindProcess(PCWSTR exeName, DWORD& pid, vector<DWORD>& tids) {
	auto hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return false;

	pid = 0;

	PROCESSENTRY32 pe = { sizeof(pe) };
	if (::Process32First(hSnapshot, &pe)) {
		do {
			if (_wcsicmp(pe.szExeFile, exeName) == 0) {
				pid = pe.th32ProcessID;
				THREADENTRY32 te = { sizeof(te) };
				if (::Thread32First(hSnapshot, &te)) {
					do {
						if (te.th32OwnerProcessID == pid) {
							tids.push_back(te.th32ThreadID);
						}
					} while (::Thread32Next(hSnapshot, &te));
				}
				break;
			}
		} while (::Process32Next(hSnapshot, &pe));
	}

	::CloseHandle(hSnapshot);
	return pid > 0 && !tids.empty();
}

APIHOOKHELPER_API BOOL AdvInjectDLL(DWORD dwPID, CTString dllPath)
{
http://blogs.microsoft.co.il/pavely/2017/03/14/injecting-a-dll-without-a-remote-thread/
	DWORD pid;
	std::vector<DWORD> tids;
	if (FindProcess(L"explorer.exe", pid, tids)) {
		HANDLE hProcess = ::OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, dwPID);
		auto p = ::VirtualAllocEx(hProcess, nullptr, 1 << 12, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		wchar_t buffer[] = L"g:\\temp\\MyLibrary.Dll";
		::WriteProcessMemory(hProcess, p, buffer, sizeof(buffer), nullptr);

		for (const auto& tid : tids) {
			HANDLE hThread = ::OpenThread(THREAD_SET_CONTEXT, FALSE, tid);
			if (hThread) {
				::QueueUserAPC((PAPCFUNC)::GetProcAddress(GetModuleHandle(L"kernel32"), "LoadLibraryW"), hThread, (ULONG_PTR)p);
			}
		}
		::VirtualFreeEx(hProcess, p, 0, MEM_RELEASE | MEM_DECOMMIT);
	}
	return TRUE;
}