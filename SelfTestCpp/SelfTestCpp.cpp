// SelfTestCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../ApiHookHelper/ApiHookHelper.h"
#include "../ApiHookHelper/TString.h"
#include "../ApiHookHelper/Win32Process.h"
#include <iostream>

void MonitorCallback(void* caller, int event_id, Win32_Process process)
{
	event_id == PROCESS_EVENT_CREATE ? wprintf(_T("event : Create ")) : wprintf(_T("event : Exit "));
	std::wcout << _T("Process Name: ") << process.Name.ToWString();
	std::wcout<<" Handle: " << process.Handle.ToWString() << std::endl;
}
int main()
{
	MonitorAllProcessCreation(MonitorCallback);
	std::cin.get();
    return 0;
}

