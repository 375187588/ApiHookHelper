#include "stdafx.h"
#include "ProcessMonitor.h"
#include "TString.h"

CProcessMonitor::CProcessMonitor(ProcessMonitorCallback OnProcessMonitorCallback)
{
	this->OnProcessMonitorCallback = OnProcessMonitorCallback;
}

CProcessMonitor::CProcessMonitor(CTString ProcessName, ProcessMonitorCallback OnProcessMonitorCallback)
{
	this->m_strProcessName = ProcessName;
	this->OnProcessMonitorCallback = OnProcessMonitorCallback;
}

CProcessMonitor::~CProcessMonitor()
{

}

void CProcessMonitor::StartMonitor()
{
	this->m_pEventSink = myEventSink::registerCreationCallback(this->m_strProcessName, this->OnProcessMonitorCallback);
}

void CProcessMonitor::StopMonitor()
{
	if (m_pEventSink != NULL)
		if(SUCCEEDED(this->m_pEventSink->pSvc->CancelAsyncCall(m_pEventSink->pStubSink)))
			m_pEventSink = NULL;
}
