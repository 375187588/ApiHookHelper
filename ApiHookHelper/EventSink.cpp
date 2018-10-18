#include "stdafx.h"
#include "EventSink.h"
#include "Win32Process.h"
#pragma comment(lib, "wbemuuid.lib")

ULONG STDMETHODCALLTYPE myEventSink::AddRef() {
	return InterlockedIncrement(&m_IRef);
}

ULONG STDMETHODCALLTYPE myEventSink::Release() {
	LONG IRef = InterlockedDecrement(&m_IRef);
	if (IRef == 0)
		delete this;
	return IRef;
}

HRESULT STDMETHODCALLTYPE myEventSink::QueryInterface(REFIID riid, void** ppv) {
	if (riid == IID_IUnknown || riid == IID_IWbemObjectSink) {
		*ppv = (IWbemObjectSink*) this;
		AddRef();
		return WBEM_S_NO_ERROR;
	}
	else return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE myEventSink::Indicate(
	LONG lObjectCount,
	IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray
)
{
	HRESULT hr = S_OK;
	_variant_t vtProp;
	int event_id = PROCESS_EVENT_CREATE;
	for (int i = 0; i < lObjectCount; i++)
	{
		bool CreateorDel = false;
		_variant_t cn;
		hr = apObjArray[i]->Get(_bstr_t(L"__Class"), 0, &cn, 0, 0);
		/*When query __InstanceOperationEvent, there are 3 different type of event
			__InstanceDeletionEvent: process exit
			__InstanceCreationEvent: process create / start
			__InstanceModificationEvent: a file is changed / edited / deleted
		*/
		if (SUCCEEDED(hr))
		{
			CTString LClassStr(cn.bstrVal);
			if (0 == LClassStr.compare(L"__InstanceDeletionEvent"))
			{
				//this is deletion
				event_id = PROCESS_EVENT_TERMINATE;
				CreateorDel = true;
			}
			else if (0 == LClassStr.compare(L"__InstanceCreationEvent"))
			{
				//This is creation
				CreateorDel = true;
			}
			else
			{
				CreateorDel = false;
				//wcout << "Modification " << endl;             
			}
		}
		VariantClear(&cn);
		
		if (CreateorDel)
		{
			//CWin32Process p(apObjArray[i]);
			hr = apObjArray[i]->Get(_bstr_t(L"TargetInstance"), 0, &vtProp, 0, 0);
			if (!FAILED(hr))
			{/*
			 All Available attributes for creation and deletion event
			 https://msdn.microsoft.com/en-us/library/aa394372(v=vs.85).aspx
			 */
				IUnknown* str = vtProp;
				Win32_Process p;
				p.CreationClassName = GetString(apObjArray[i],str, L"CreationClassName");
				p.Caption = GetString(apObjArray[i], str, L"Caption");
				p.CommandLine = GetString(apObjArray[i], str, L"CommandLine");
				p.CSCreationClassName = GetString(apObjArray[i], str, L"CSCreationClassName");
				p.CSName = GetString(apObjArray[i], str, L"CSName");
				p.Description = GetString(apObjArray[i], str, L"Description");
				p.ExecutionState = GetUint16(apObjArray[i], str, L"ExecutionState");
				p.HandleCount = GetUint32(apObjArray[i], str, L"HandleCount");
				p.InstallDate = GetDateTime(apObjArray[i], str, L"InstallDate");
				p.KernelModeTime = GetUint64(apObjArray[i], str, L"KernelModeTime");
				p.MaximumWorkingSetSize = GetUint32(apObjArray[i], str, L"MaximumWorkingSetSize");
				p.MinimumWorkingSetSize = GetUint32(apObjArray[i], str, L"MinimumWorkingSetSize");
				p.OSCreationClassName = GetString(apObjArray[i], str, L"OSCreationClassName");
				p.OSName = GetString(apObjArray[i], str, L"OSName");
				p.OtherOperationCount = GetUint64(apObjArray[i], str, L"OtherOperationCount");
				p.OtherTransferCount = GetUint64(apObjArray[i], str, L"OtherTransferCount");
				p.PageFaults = GetUint32(apObjArray[i], str, L"PageFaults");
				p.PageFileUsage = GetUint32(apObjArray[i], str, L"PageFileUsage");
				p.ParentProcessId = GetUint32(apObjArray[i], str, L"ParentProcessId");
				p.PeakPageFileUsage = GetUint32(apObjArray[i], str, L"PeakPageFileUsage");
				p.PeakVirtualSize = GetUint64(apObjArray[i], str, L"PeakVirtualSize");
				p.PeakWorkingSetSize = GetUint32(apObjArray[i], str, L"PeakWorkingSetSize");
				p.Priority = GetUint32(apObjArray[i], str, L"Priority");
				p.PrivatePageCount = GetUint64(apObjArray[i], str, L"PrivatePageCount");
				p.QuotaNonPagedPoolUsage = GetUint32(apObjArray[i], str, L"QuotaNonPagedPoolUsage");
				p.QuotaPagedPoolUsage = GetUint32(apObjArray[i], str, L"QuotaPagedPoolUsage");
				p.QuotaPeakNonPagedPoolUsage = GetUint32(apObjArray[i], str, L"QuotaPeakNonPagedPoolUsage");
				p.QuotaPeakPagedPoolUsage = GetUint32(apObjArray[i], str, L"QuotaPeakPagedPoolUsage");
				p.ReadOperationCount = GetUint64(apObjArray[i], str, L"ReadOperationCount");
				p.ReadTransferCount = GetUint64(apObjArray[i], str, L"ReadTransferCount");
				p.SessionId = GetUint32(apObjArray[i], str, L"SessionId");
				p.Status = GetString(apObjArray[i], str, L"Status");
				p.ThreadCount = GetUint32(apObjArray[i], str, L"ThreadCount");
				p.VirtualSize = GetUint64(apObjArray[i], str, L"VirtualSize");
				p.WindowsVersion = GetString(apObjArray[i], str, L"WindowsVersion");
				p.WorkingSetSize = GetUint64(apObjArray[i], str, L"WorkingSetSize");
				p.WriteOperationCount = GetUint64(apObjArray[i], str, L"WriteOperationCount");
				p.WriteTransferCount = GetUint64(apObjArray[i], str, L"WriteTransferCount");
				p.Handle = GetString(apObjArray[i], str, L"Handle");
				p.ProcessId = GetUint32(apObjArray[i], str, L"ProcessId");
				p.Name = GetString(apObjArray[i], str, L"Name");
				p.ExecutablePath = GetString(apObjArray[i], str, L"ExecutablePath");
				p.UserModeTime = GetUint64(apObjArray[i], str, L"UserModeTime");
				p.TerminationDate = GetDateTime(apObjArray[i], str, L"TerminationDate");
				p.CreationDate = GetDateTime(apObjArray[i], str, L"CreationDate"); //Privileged instruction
				m_callback(this, event_id, p);
			}
			VariantClear(&vtProp);
		}

	}

	/* Unregister event sink */
	//pSvc->CancelAsyncCall(pStubSink);
	return WBEM_S_NO_ERROR;
}

HRESULT STDMETHODCALLTYPE myEventSink::SetStatus(LONG IFlags, HRESULT hResult, BSTR strParam, IWbemClassObject __RPC_FAR *pObjParam) {
	return WBEM_S_NO_ERROR;
}


CComPtr<myEventSink> myEventSink::registerCreationCallback(CTString ProcessName, ProcessMonitorCallback callback) {
	CComPtr<IWbemLocator> pLoc;
	CoInitializeEx(0, COINIT_MULTITHREADED);

	HRESULT hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);

	if (FAILED(hres)) {

		//("CreationEvent initialization failed");
		return NULL;
	}
	CComPtr<myEventSink> pSink(new myEventSink(callback));

	hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSink->pSvc);
	if (FAILED(hres)) {
		//cout << "Could not connect. Error code = 0x" << hex << hres << endl;
		//throw std::exception("CreationEvent initialization failed");
		return NULL;
	}
	hres = CoSetProxyBlanket(pSink->pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (FAILED(hres)) {
		//cout << "Coult not set proxy blanket, Error code =0x" << hex << hres << endl;
		//throw std::exception("CreationEvent initialization failed");
		return NULL;
	}

	CComPtr<IUnsecuredApartment> pUnsecApp;
	hres = CoCreateInstance(CLSID_UnsecuredApartment, NULL, CLSCTX_LOCAL_SERVER, IID_IUnsecuredApartment, (void**)&pUnsecApp);
	CComPtr<IUnknown> pStubUnk;
	pUnsecApp->CreateObjectStub(pSink, &pStubUnk);
	pStubUnk->QueryInterface(IID_IWbemObjectSink, (void**)&pSink->pStubSink);


	char buffer[512];
	if (ProcessName.length() == 0)
	{
		sprintf_s(buffer, "SELECT * FROM __InstanceOperationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'");
	}
	else {
		sprintf_s(buffer, "SELECT * FROM __InstanceOperationEvent  WITHIN 1 WHERE TargetInstance ISA 'Win32_Process' AND TargetInstance.Name = '%s'", ProcessName.ToAString());
	}
	hres = pSink->pSvc->ExecNotificationQueryAsync(_bstr_t("WQL"), _bstr_t(buffer), WBEM_FLAG_SEND_STATUS, NULL, pSink->pStubSink);

	if (FAILED(hres)) {
		//cout << "ExecNotificationQueryAsync failed with = 0x" << hex << hres << endl;
		//throw std::exception("CreationEvent initialization failed");
		return NULL;
	}
	return pSink;
}



uint32 myEventSink::GetUint32(IWbemClassObject* apObj, IUnknown* str, CTString PropertyName)
{
	_variant_t cn;
	HRESULT hr = str->QueryInterface(IID_IWbemClassObject, reinterpret_cast<void**>(&apObj));
	if (SUCCEEDED(hr))
	{
		hr = apObj->Get(PropertyName.ToWString(), 0, &cn, NULL, NULL);

		if (SUCCEEDED(hr))
		{
			switch (cn.vt)
			{
			case VT_NULL:
				VariantClear(&cn);
				return 0;
			case VT_EMPTY:
				VariantClear(&cn);
				return 0;
				break;
			default:
				VariantClear(&cn);
				uint32 val = cn.uintVal;
				return val;
			}
		}
	}
	return 0;
}
uint64 myEventSink::GetUint64(IWbemClassObject* apObj, IUnknown* str, CTString PropertyName)
{
	HRESULT hr = S_OK;
	_variant_t cn;
	if (!FAILED(hr))
	{
		hr = str->QueryInterface(IID_IWbemClassObject, reinterpret_cast<void**>(&apObj));
		if (SUCCEEDED(hr))
		{
			hr = apObj->Get(PropertyName.ToTString(), 0, &cn, NULL, NULL);

			if (SUCCEEDED(hr))
			{
				switch (cn.vt)
				{
				case VT_NULL:
					VariantClear(&cn);
					return 0;
				case VT_EMPTY:
					VariantClear(&cn);
					return 0;
					break;
				default:
					VariantClear(&cn);
					return cn.ullVal;
					break;
				}
			}
		}
	}
	return 0;
}

CTString myEventSink::GetString(IWbemClassObject* apObj, IUnknown* str, CTString PropertyName)
{
	HRESULT hr = S_OK;
	_variant_t cn;
	CTString strVal;
	if (!FAILED(hr))
	{
		hr = str->QueryInterface(IID_IWbemClassObject, reinterpret_cast<void**>(&apObj));
		if (SUCCEEDED(hr))
		{
			hr = apObj->Get(PropertyName.ToTString(), 0, &cn, NULL, NULL);

			if (SUCCEEDED(hr))
			{
				switch (cn.vt)
				{
				case VT_NULL:
					VariantClear(&cn);
					return strVal;
				case VT_EMPTY:
					VariantClear(&cn);
					return strVal;
				default:
					VariantClear(&cn);
					strVal = cn.bstrVal;
					return strVal;
				}
			}
		}
	}
	VariantClear(&cn);

	return strVal;
}
uint16 myEventSink::GetUint16(IWbemClassObject* apObj, IUnknown* str, CTString PropertyName)
{
	HRESULT hr = S_OK;
	_variant_t cn;
	if (!FAILED(hr))
	{
		hr = str->QueryInterface(IID_IWbemClassObject, reinterpret_cast<void**>(&apObj));
		if (SUCCEEDED(hr))
		{
			hr = apObj->Get(PropertyName.ToTString(), 0, &cn, NULL, NULL);

			if (SUCCEEDED(hr))
			{
				switch (cn.vt)
				{
				case VT_NULL:
					VariantClear(&cn);
					return 0;
				case VT_EMPTY:
					VariantClear(&cn);
					return 0;
				default:
					VariantClear(&cn);
					return cn.uiVal;
				}
			}
		}
	}
	return 0;
}


datetime myEventSink::GetDateTime(IWbemClassObject* apObj, IUnknown* str, CTString PropertyName)
{
	return myEventSink::GetString(apObj, str, PropertyName);
}