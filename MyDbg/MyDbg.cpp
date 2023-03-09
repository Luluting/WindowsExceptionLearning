// MyDbg.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	printf( "[MyDbg] ���ǵ�������ֻ��������Ŷ~\n" ); 
	STARTUPINFO startup = {0};
	startup.cb = sizeof(startup);
	PROCESS_INFORMATION procInfo = {0};
	BOOL bRet = CreateProcess( NULL, _T("ExcepHandler.exe"), NULL, NULL, FALSE, DEBUG_ONLY_THIS_PROCESS, NULL, NULL, &startup, &procInfo );
	if ( !bRet )
	{
		printf( "[MyDbg] ExcepHandler.exe �����ڣ��޷��������Խ��̡�\n" ); 
		getchar();
		return 0;
	}
	printf( "[MyDbg] �������Խ��̳ɹ���\n");
	int bExit = FALSE;
	bool bFirstExcp = true;
	while(!bExit) 
	{
		DWORD dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
		DEBUG_EVENT debugEvent;
		WaitForDebugEvent(&debugEvent, INFINITE);
		switch ( debugEvent.dwDebugEventCode )
		{
		case EXCEPTION_DEBUG_EVENT:
			{
				if ( bFirstExcp == true )
				{
					dwContinueStatus = DBG_CONTINUE;
					bFirstExcp = false;
					break;
				}
				EXCEPTION_DEBUG_INFO* pExcpInfo = &debugEvent.u.Exception;
				if ( pExcpInfo->dwFirstChance )
				{
					printf( "[MyDbg] ��һ�α����¡� ErrCode:%08X, ERRAddr:%p\n", pExcpInfo->ExceptionRecord.ExceptionCode, pExcpInfo->ExceptionRecord.ExceptionAddress);

				}
				else
				{
					printf( "[MyDbg] �ڶ��α����¡� ErrCode:%08X, ERRAddr:%p\n", pExcpInfo->ExceptionRecord.ExceptionCode, pExcpInfo->ExceptionRecord.ExceptionAddress);
				}

				if ( MessageBox(0,_T("������쳣?"), _T("���ǵ�����"),MB_YESNO)==IDYES )
				{
					dwContinueStatus = DBG_CONTINUE;
					if ( MessageBox(0,_T("��NOPָ���int3ָ��?"), _T("���ǵ�����"),MB_YESNO)==IDYES )
					{
						void* pExcpAddr = debugEvent.u.Exception.ExceptionRecord.ExceptionAddress;
						if ( debugEvent.u.Exception.dwFirstChance )
						{
							pExcpAddr = debugEvent.u.Exception.ExceptionRecord.ExceptionAddress;
						}
						BYTE szBuffer[1] = {0x90};//NOP
						DWORD dwOldProt = 0;
						VirtualProtectEx( procInfo.hProcess, pExcpAddr, 1, PAGE_EXECUTE_READWRITE, &dwOldProt );
						WriteProcessMemory( procInfo.hProcess, pExcpAddr, szBuffer, 1, NULL );
						VirtualProtectEx( procInfo.hProcess, pExcpAddr, 1, dwOldProt, &dwOldProt );
					}
				}
				printf( "[MyDbg] ���쳣�Ĵ�����:%s\n", dwContinueStatus==DBG_CONTINUE?"����":"���������Լ�����");
			}
			break;
		case EXIT_PROCESS_DEBUG_EVENT:
			{
				printf( "[MyDbg] �յ������˳��¼���\n");
				bExit = TRUE;
			}
			break;
		}
		ContinueDebugEvent(debugEvent.dwProcessId, 
			debugEvent.dwThreadId, dwContinueStatus); 

	}
	printf( "[MyDbg] �������ԣ�\n");
	CloseHandle( procInfo.hProcess );
	CloseHandle( procInfo.hThread );
	return 0;
}

