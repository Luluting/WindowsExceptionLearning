// MyDbg.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	printf( "[MyDbg] 我是调试器，只用来测试哦~\n" ); 
	STARTUPINFO startup = {0};
	startup.cb = sizeof(startup);
	PROCESS_INFORMATION procInfo = {0};
	BOOL bRet = CreateProcess( NULL, _T("ExcepHandler.exe"), NULL, NULL, FALSE, DEBUG_ONLY_THIS_PROCESS, NULL, NULL, &startup, &procInfo );
	if ( !bRet )
	{
		printf( "[MyDbg] ExcepHandler.exe 不存在，无法创建调试进程。\n" ); 
		getchar();
		return 0;
	}
	printf( "[MyDbg] 创建调试进程成功！\n");
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
					printf( "[MyDbg] 第一次被断下。 ErrCode:%08X, ERRAddr:%p\n", pExcpInfo->ExceptionRecord.ExceptionCode, pExcpInfo->ExceptionRecord.ExceptionAddress);

				}
				else
				{
					printf( "[MyDbg] 第二次被断下。 ErrCode:%08X, ERRAddr:%p\n", pExcpInfo->ExceptionRecord.ExceptionCode, pExcpInfo->ExceptionRecord.ExceptionAddress);
				}

				if ( MessageBox(0,_T("处理该异常?"), _T("我是调试器"),MB_YESNO)==IDYES )
				{
					dwContinueStatus = DBG_CONTINUE;
					if ( MessageBox(0,_T("用NOP指令覆盖int3指令?"), _T("我是调试器"),MB_YESNO)==IDYES )
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
				printf( "[MyDbg] 该异常的处理结果:%s\n", dwContinueStatus==DBG_CONTINUE?"忽略":"交给程序自己处理");
			}
			break;
		case EXIT_PROCESS_DEBUG_EVENT:
			{
				printf( "[MyDbg] 收到进程退出事件！\n");
				bExit = TRUE;
			}
			break;
		}
		ContinueDebugEvent(debugEvent.dwProcessId, 
			debugEvent.dwThreadId, dwContinueStatus); 

	}
	printf( "[MyDbg] 结束调试！\n");
	CloseHandle( procInfo.hProcess );
	CloseHandle( procInfo.hThread );
	return 0;
}

