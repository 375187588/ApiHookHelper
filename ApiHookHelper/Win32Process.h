#pragma once
#include <stdint.h>
#include "TString.h"

typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef CTString datetime;

#define PROCESS_EVENT_CREATE 0
#define PROCESS_EVENT_TERMINATE 1

typedef struct win32_Process
{
	CTString   CreationClassName;
	CTString   Caption;
	CTString   CommandLine;
	datetime	CreationDate;
	CTString   CSCreationClassName;
	CTString   CSName;
	CTString   Description;
	CTString   ExecutablePath;
	uint16   ExecutionState;
	CTString   Handle;
	uint32   HandleCount;
	datetime InstallDate;
	uint64   KernelModeTime;
	uint32   MaximumWorkingSetSize;
	uint32   MinimumWorkingSetSize;
	CTString   Name;
	CTString   OSCreationClassName;
	CTString   OSName;
	uint64   OtherOperationCount;
	uint64   OtherTransferCount;
	uint32   PageFaults;
	uint32   PageFileUsage;
	uint32   ParentProcessId;
	uint32   PeakPageFileUsage;
	uint64   PeakVirtualSize;
	uint32   PeakWorkingSetSize;
	uint32   Priority;
	uint64   PrivatePageCount;
	uint32   ProcessId;
	uint32   QuotaNonPagedPoolUsage;
	uint32   QuotaPagedPoolUsage;
	uint32   QuotaPeakNonPagedPoolUsage;
	uint32   QuotaPeakPagedPoolUsage;
	uint64   ReadOperationCount;
	uint64   ReadTransferCount;
	uint32   SessionId;
	CTString   Status;
	datetime TerminationDate;
	uint32   ThreadCount;
	uint64   UserModeTime;
	uint64   VirtualSize;
	CTString   WindowsVersion;
	uint64   WorkingSetSize;
	uint64   WriteOperationCount;
	uint64   WriteTransferCount;
}Win32_Process;
