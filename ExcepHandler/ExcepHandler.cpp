// ExcepHandler.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>

//先介绍一下返回值的意思
//EXCEPTION_EXECUTE_HANDLER			//该指令异常被处理。从下一条指令继续执行
//EXCEPTION_CONTINUE_SEARCH			//不能处理该异常，让别人处理它吧
//EXCEPTION_CONTINUE_EXECUTION		//该指令被忽略。从该指令处继续执行


void ShowExceptionInfo(PEXCEPTION_POINTERS pExcepInfo )
{

}
LONG ShowSelectMessageBox( TCHAR* pTitle )
{
	int nRet = MessageBox( 0, _T("--------\n我要“认领”该异常?\n--------\nYES：认领该异常。\n\nNO: 交给别人处理(return EXCEPTION_CONTINUE_SEARCH)"), pTitle, MB_YESNO );
	if ( nRet != IDYES )
	{//让别人处理
		_tprintf( _T("[EH.Exe] [SELE] Select EXCEPTION_CONTINUE_SEARCH\n") );
		return EXCEPTION_CONTINUE_SEARCH;
	}
	nRet = MessageBox( 0, _T("--------\n是“忽略”还是“处理”该异常?\n--------\nYES：忽略，从该指令处继续执行(return EXCEPTION_CONTINUE_EXECUTION)。\n\nNO: 处理，从下一条指令继续执行(return EXCEPTION_EXECUTE_HANDLER)"), pTitle, MB_YESNO );
	if ( nRet != IDYES )
	{//处理
		_tprintf( _T("[EH.Exe] [SELE] Select EXCEPTION_EXECUTE_HANDLER\n") );
		return EXCEPTION_EXECUTE_HANDLER;
	}

	//忽略
	_tprintf( _T("[EH.Exe] [SELE] Select EXCEPTION_CONTINUE_EXECUTION\n") );
	return EXCEPTION_CONTINUE_EXECUTION;
}
LONG NTAPI FirstVectExcepHandler( PEXCEPTION_POINTERS pExcepInfo )
{
	TCHAR* pTitle = _T("*首个* VEH异常处理器");
	_tprintf( _T("[EH.Exe] [VEH][1] in \n") );
	LONG nRet = ShowSelectMessageBox(pTitle);
	if ( nRet  == EXCEPTION_EXECUTE_HANDLER )
	{
		_tprintf( _T("[EH.Exe] [VEH][1] EXCEPTION_EXECUTE_HANDLER 标志在VEH中无效！,所以第二个VEH被调用。\n") );
	}
	if ( nRet == EXCEPTION_CONTINUE_EXECUTION )
	{
		if ( MessageBox(0,_T("jmp int3？(跳过INT3指令,否则还会被断下)"), _T("是否修正到下一条指令执行"), MB_YESNO) == IDYES )
		{
			pExcepInfo->ContextRecord->Eip += 1;//跳过int3
			_tprintf( _T("[EH.Exe] [VEH][1] 该异常被处理。 且：jmp int3\n") );
		}else
		{
			_tprintf( _T("[EH.Exe] [VEH][1] 该异常被处理\n") );
		}
	}
	_tprintf( _T("[EH.Exe] [VEH][1] out\n") );
	return nRet;
}

LONG NTAPI LastVectExcepHandler( PEXCEPTION_POINTERS pExcepInfo )
{
	TCHAR* pTitle = _T("*最后* VEH异常处理器");
	_tprintf( _T("[EH.Exe] [VEH][2] in \n") );
	LONG nRet = ShowSelectMessageBox(pTitle);
	if ( nRet  == EXCEPTION_EXECUTE_HANDLER )
	{
		_tprintf( _T("[EH.Exe] [VEH][2] EXCEPTION_EXECUTE_HANDLER 标志在VEH中无效！,所以SEH被调用。\n") );
	}
	if ( nRet == EXCEPTION_CONTINUE_EXECUTION )
	{
		if ( MessageBox(0,_T("jmp int3？(跳过INT3指令,否则还会被断下)"), _T("是否修正到下一条指令执行"), MB_YESNO) == IDYES )
		{
			pExcepInfo->ContextRecord->Eip += 1;//跳过int3
			_tprintf( _T("[EH.Exe] [VEH][2] 该异常被处理。 且：jmp int3\n") );
		}else
		{
			_tprintf( _T("[EH.Exe] [VEH][2] 该异常被处理\n") );
		}
	}
	_tprintf( _T("[EH.Exe] [VEH][2] out \n") );
	return nRet;
}

LONG NTAPI TopLevelExcepFilter( PEXCEPTION_POINTERS pExcepInfo )
{
	TCHAR* pTitle = _T("*顶级* 异常处理器");
	_tprintf( _T("[EH.Exe] [TOP] in \n") );
	LONG nRet = ShowSelectMessageBox(pTitle);
	_tprintf( _T("[EH.Exe] [TOP] out \n") );;
	return nRet;
}

LONG FirstSEHer( PEXCEPTION_POINTERS pExcepInfo )
{
	TCHAR* pTitle = _T("第一个SEH处理器");
	_tprintf( _T("[EH.Exe] [SEH][1] in \n") );
	LONG nRet = ShowSelectMessageBox(pTitle);
	_tprintf( _T("[EH.Exe] [SEH][1] out \n") );
	return nRet;
}
LONG SecondSEHer( PEXCEPTION_POINTERS pExcepInfo )
{
	TCHAR* pTitle = _T("第二个SEH处理器");
	_tprintf( _T("[EH.Exe] [SEH][2] in \n") );
	LONG nRet = ShowSelectMessageBox(pTitle);
	_tprintf( _T("[EH.Exe] [SEH][2] out \n") );;
	return nRet;
}

LONG ThirdSEHer( PEXCEPTION_POINTERS pExcepInfo )
{
	TCHAR* pTitle = _T("第三个SEH处理器");
	_tprintf( _T("[EH.Exe] [SEH][3] in \n") );
	LONG nRet = ShowSelectMessageBox(pTitle);
	_tprintf( _T("[EH.Exe] [SEH][3] out \n") );;
	return nRet;
}

void ExcepFunction()
{
	__try
	{
		__try
		{
			_tprintf( _T("[EH.Exe] *[CALL] int 3\n") );
			__asm int 3;
		}
		__except( FirstSEHer(GetExceptionInformation()) )
		{
			_tprintf( _T("[EH.Exe] [SEH][1] 被俺处理了~(只有返回EXCEPTION_EXECUTE_HANDLER才会走到这里)\n"));
		}
	}
	__except( SecondSEHer(GetExceptionInformation()) )
	{
		_tprintf( _T("[EH.Exe] [SEH][2] 被俺处理了(只有返回EXCEPTION_EXECUTE_HANDLER才会走到这里)\n"));
	}
}

DWORD __stdcall ExcepThread(  LPVOID lpThreadParameter)
{
	_tprintf( _T("[EH.Exe] [ExcepThread] in\n"));
	__try
	{
		ExcepFunction();
	}
	__except( ThirdSEHer(GetExceptionInformation()) )
	{
		_tprintf( _T("[EH.Exe] [SEH][3] 被俺处理了(只有返回EXCEPTION_EXECUTE_HANDLER才会走到这里)\n"));
	}
	_tprintf( _T("[EH.Exe] [ExcepThread] out\n"));
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tprintf( _T("[EH.Exe] Add VEH.\n") );
	AddVectoredExceptionHandler( 1, &FirstVectExcepHandler );
	_tprintf( _T("[EH.Exe] Add VEH.\n") );
	AddVectoredExceptionHandler( 0, &LastVectExcepHandler );
	_tprintf( _T("[EH.Exe] Add Top LEF.\n") );
	SetUnhandledExceptionFilter( &TopLevelExcepFilter );

	HANDLE hThread = CreateThread( NULL, 0, &ExcepThread, NULL, 0, NULL );
	if ( hThread )
	{
		WaitForSingleObject( hThread, INFINITE );
	}
	_tprintf( _T("[EH.Exe] 进程退出\n") );
	return 0;
}

