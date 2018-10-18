#pragma once
#include <comdef.h>
#include <Wbemidl.h>
#include <atlcomcli.h>
#include "TString.h"
#include "RegisterCallBack.h"
#include "Win32Process.h"
#include "ApiHookHelper.h"

class myEventSink :
	public IWbemObjectSink
{
public:
	static CComPtr<myEventSink> registerCreationCallback(CTString strProcessName, ProcessMonitorCallback callback);

public:
	CComPtr<IWbemServices> pSvc;
	CComPtr<IWbemObjectSink> pStubSink;
	LONG m_IRef;
	ProcessMonitorCallback m_callback;

public:
	myEventSink(ProcessMonitorCallback callback) :m_IRef(0), m_callback(callback) {}
	~myEventSink() {
	}

	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
	virtual HRESULT STDMETHODCALLTYPE Indicate(LONG lObjectCount,
		IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray);
	virtual HRESULT STDMETHODCALLTYPE SetStatus(LONG IFlags, HRESULT hResult, 
		BSTR strParam, IWbemClassObject __RPC_FAR *pObjParam);

private:
	uint32 GetUint32(IWbemClassObject* apObj, IUnknown* str, CTString PropertyName);
	uint64 GetUint64(IWbemClassObject* apObj, IUnknown* str, CTString PropertyName);
	CTString GetString(IWbemClassObject* apObj, IUnknown* str, CTString PropertyName);
	uint16 GetUint16(IWbemClassObject* apObj, IUnknown* str, CTString PropertyName);
	datetime GetDateTime(IWbemClassObject* apObj, IUnknown* str, CTString PropertyName);
};
