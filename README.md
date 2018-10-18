# ApiHookHelper
This project consists a set of functions to make API Hook easier on Windows Platform.

## Process Monitoring
One thing that is important for developing a hook is know what process user has started.

MonitorAllProcessCreation function gives developer the ability the receive a callback when a new process is created.


## DLL Injection
DLL injection is a must when developing a HOOK.
InjectDLL provide you a old school way (GetProcAddress -> VirtualAllocEx -> WriteProcessMemory -> CreateRemoteThread -> LoadLibrary) to inject a DLL into a process.
AdvInjectDLL provide you an experimental way (proposed by Pavel from Microsoft) to inject a DLL.

## Testing Environment
This library is developed and tested on Windows 10 64 bit.
Library can be called from both C++ and C#.