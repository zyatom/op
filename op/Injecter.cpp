#include "stdafx.h"
#include "Injecter.h"


Injecter::Injecter()
{
}


Injecter::~Injecter()
{
}

BOOL Injecter::EnablePrivilege(BOOL enable)
{
	// �õ����ƾ��
	HANDLE hToken = NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY | TOKEN_READ, &hToken))
		return FALSE;

	// �õ���Ȩֵ
	LUID luid;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		return FALSE;

	// �������ƾ��Ȩ��
	TOKEN_PRIVILEGES tp = {};
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL))
		return FALSE;

	// �ر����ƾ��
	CloseHandle(hToken);

	return TRUE;
}


long Injecter::InjectDll(DWORD pid, LPCTSTR dllPath,long& error_code)
{
	
	auto jhandle=::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	/**pid = processInfo.dwProcessId;
	*process = processInfo.hProcess;*/
	if (!jhandle) {
		error_code = ::GetLastError();
		return -1;
	}
	DWORD dllPathSize = ((DWORD)_tcslen(dllPath) + 1) * sizeof(TCHAR);

	// �����ڴ��������DLL·��
	void* remoteMemory = VirtualAllocEx(jhandle, NULL, dllPathSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (remoteMemory == NULL)
	{
		//setlog(L"�����ڴ�ʧ�ܣ�������룺%u\n", GetLastError());
		error_code = ::GetLastError();
		return -2;
	}

	// д��DLL·��
	if (!WriteProcessMemory(jhandle, remoteMemory, dllPath, dllPathSize, NULL))
	{
		//setlog(L"д���ڴ�ʧ�ܣ�������룺%u\n", GetLastError());
		error_code = ::GetLastError();
		return -3;
	}

	// ����Զ�̵߳���LoadLibrary
	auto lpfn=GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryW");
	if (!lpfn) {
		error_code = ::GetLastError();
		return -4;
	}
	HANDLE remoteThread = CreateRemoteThread(jhandle, NULL, 0, (LPTHREAD_START_ROUTINE)lpfn, remoteMemory, 0, NULL);
	if (remoteThread == NULL)
	{
		//setlog(L"����Զ�߳�ʧ�ܣ�������룺%u\n", GetLastError());
		error_code = ::GetLastError();
		return -5;
	}
	// �ȴ�Զ�߳̽���
	WaitForSingleObject(remoteThread, INFINITE);
	// ȡDLL��Ŀ����̵ľ��
	DWORD remoteModule;
	GetExitCodeThread(remoteThread, &remoteModule);

	// �ָ��߳�
	//ResumeThread(processInfo.hThread);

	// �ͷ�
	CloseHandle(remoteThread);
	VirtualFreeEx(jhandle, remoteMemory, dllPathSize, MEM_DECOMMIT);
	CloseHandle(jhandle);
	error_code = 0;
	return 1;
}