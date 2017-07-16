#include <Windows.h>  
#include <TlHelp32.h>  
#include <stdio.h>  
#include <shlwapi.h>  
BOOL EnableDebugPrivilege();
DWORD GetTargetProcessID(const char *processExeName)  
{  
	if(!EnableDebugPrivilege()){
		printf("EnableDebugPrivilege faild\n");
	}
	if (processExeName == NULL) {  
		return 0;  
	}  

	HANDLE hSnapshot;  

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
	if (INVALID_HANDLE_VALUE == hSnapshot) {  
		printf("%d\n", GetLastError());  
		return 0;  
	}  

	PROCESSENTRY32 pe;  
	BOOL bRet = FALSE;  

	pe.dwSize = sizeof (PROCESSENTRY32);  
	bRet = Process32First(hSnapshot, &pe);  
	while (bRet) {  
		if (strstr(processExeName, pe.szExeFile)) {  
			return pe.th32ProcessID;  
		} else {  
			ZeroMemory(&pe, sizeof (PROCESSENTRY32));  
			pe.dwSize = sizeof (PROCESSENTRY32);  
			bRet = Process32Next(hSnapshot, &pe);  
		}  
	}  

	return 0;  
}  
BOOL  EnableDebugPrivilege()
{
	HANDLE hSnap;
	HANDLE hkernel32 = NULL;    //被注入进程的句柄
	PROCESSENTRY32 pe; 
	BOOL bNext;
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID Luid;
	LPVOID p;
	FARPROC pfn;
	if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS_P,&hToken))
	{
		printf("提升权限失败\n");
		system("pause");
		return 0;
	}

	if (!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&Luid))
	{
		printf("提升权限失败\n");
		system("pause");
		return 0;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = Luid;

	if (!AdjustTokenPrivileges(hToken,0,&tp,sizeof(TOKEN_PRIVILEGES),NULL,NULL))
	{
		printf("提升权限失败\n");
		system("pause");
		return 0;
	}
	return 1;
}
BOOL InjectDll(const char *DllFullPath, const DWORD dwRemoteProcessId)  
{  
	if (DllFullPath == NULL) {  
		return FALSE;  
	}  
	if (dwRemoteProcessId <= 0) {  
		return FALSE;  
	}  
	HANDLE hRemoteProcess;  

	hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwRemoteProcessId);  
	if (NULL == hRemoteProcess) {  
		printf("OpenProcess:%d\n", GetLastError());  
		return FALSE;  
	}  

	char *pRemoteAddr;  
	int len = strlen(DllFullPath) + 1;  
	pRemoteAddr = (char *)VirtualAllocEx(hRemoteProcess, NULL, len, MEM_COMMIT, PAGE_READWRITE);  
	if (NULL == pRemoteAddr) {  
		printf("VirtualAllocEx:%d\n", GetLastError());  
		goto error;  
	}  

	if (!WriteProcessMemory(hRemoteProcess, pRemoteAddr, DllFullPath, len, NULL)) {  
		printf("WriteProcessMemory:%d\n", GetLastError());  
		goto error;  
	}  

	HMODULE hModule;  
	char LoadLibraryA[] = "LoadLibraryA";  

	hModule = GetModuleHandle("kernel32.dll");  

	FARPROC LoadLibraryAAddr;  
	LoadLibraryAAddr = GetProcAddress(hModule, LoadLibraryA);  
	if (NULL == LoadLibraryAAddr) {  
		printf("GetProcAddress:%d\n", GetLastError());  
		goto error;  
	}  

	HANDLE remoteThread;  
	remoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryAAddr, pRemoteAddr, 0, NULL);  
	if (NULL == remoteThread) {  
		printf("CreateRemoteThread:%d\n", GetLastError());  
		goto error;  
	}  

	WaitForSingleObject(hRemoteProcess, INFINITE);  
	VirtualFreeEx(hRemoteProcess, pRemoteAddr, len, MEM_DECOMMIT);  

	CloseHandle(hRemoteProcess);  
	CloseHandle(remoteThread);  
	return TRUE;  

error:  
	CloseHandle(hRemoteProcess);  
	return FALSE;  
}  

int main(int argv,char * argc[])  
{  
	if(argv != 3){
		printf("param not right\n");
		return 0;
	}
	char * process = argc[2];
	char * dll = argc[1];
 
	DWORD pid;  

	pid = GetTargetProcessID(process); 
	InjectDll(dll, pid);  
	system("pause");
	return 0;  
}  