#pragma once
#include "ApiHookHelper.h"
#include "TString.h"
#include "EventSink.h"
#include "RegisterCallBack.h"

class APIHOOKHELPER_API CProcessMonitor
{
private:
	CTString m_strProcessName;
	CComPtr<myEventSink> m_pEventSink;
public:
	CProcessMonitor(CTString strProcessName, ProcessMonitorCallback OnProcessMonitorCallback);
	CProcessMonitor(ProcessMonitorCallback OnProcessMonitorCallback);
	~CProcessMonitor();
private:
	void StartMonitor();
	void StopMonitor();
//Variables
private:
	ProcessMonitorCallback OnProcessMonitorCallback;
};

