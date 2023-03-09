// ExcepHandler.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>

//�Ƚ���һ�·���ֵ����˼
//EXCEPTION_EXECUTE_HANDLER			//��ָ���쳣����������һ��ָ�����ִ��
//EXCEPTION_CONTINUE_SEARCH			//���ܴ�����쳣���ñ��˴�������
//EXCEPTION_CONTINUE_EXECUTION		//��ָ����ԡ��Ӹ�ָ�����ִ��


void ShowExceptionInfo(PEXCEPTION_POINTERS pExcepInfo )
{

}
LONG ShowSelectMessageBox( TCHAR* pTitle )
{
	int nRet = MessageBox( 0, _T("--------\n��Ҫ�����족���쳣?\n--------\nYES��������쳣��\n\nNO: �������˴���(return EXCEPTION_CONTINUE_SEARCH)"), pTitle, MB_YESNO );
	if ( nRet != IDYES )
	{//�ñ��˴���
		_tprintf( _T("[EH.Exe] [SELE] Select EXCEPTION_CONTINUE_SEARCH\n") );
		return EXCEPTION_CONTINUE_SEARCH;
	}
	nRet = MessageBox( 0, _T("--------\n�ǡ����ԡ����ǡ��������쳣?\n--------\nYES�����ԣ��Ӹ�ָ�����ִ��(return EXCEPTION_CONTINUE_EXECUTION)��\n\nNO: ��������һ��ָ�����ִ��(return EXCEPTION_EXECUTE_HANDLER)"), pTitle, MB_YESNO );
	if ( nRet != IDYES )
	{//����
		_tprintf( _T("[EH.Exe] [SELE] Select EXCEPTION_EXECUTE_HANDLER\n") );
		return EXCEPTION_EXECUTE_HANDLER;
	}

	//����
	_tprintf( _T("[EH.Exe] [SELE] Select EXCEPTION_CONTINUE_EXECUTION\n") );
	return EXCEPTION_CONTINUE_EXECUTION;
}
LONG NTAPI FirstVectExcepHandler( PEXCEPTION_POINTERS pExcepInfo )
{
	TCHAR* pTitle = _T("*�׸�* VEH�쳣������");
	_tprintf( _T("[EH.Exe] [VEH][1] in \n") );
	LONG nRet = ShowSelectMessageBox(pTitle);
	if ( nRet  == EXCEPTION_EXECUTE_HANDLER )
	{
		_tprintf( _T("[EH.Exe] [VEH][1] EXCEPTION_EXECUTE_HANDLER ��־��VEH����Ч��,���Եڶ���VEH�����á�\n") );
	}
	if ( nRet == EXCEPTION_CONTINUE_EXECUTION )
	{
		if ( MessageBox(0,_T("jmp int3��(����INT3ָ��,���򻹻ᱻ����)"), _T("�Ƿ���������һ��ָ��ִ��"), MB_YESNO) == IDYES )
		{
			pExcepInfo->ContextRecord->Eip += 1;//����int3
			_tprintf( _T("[EH.Exe] [VEH][1] ���쳣������ �ң�jmp int3\n") );
		}else
		{
			_tprintf( _T("[EH.Exe] [VEH][1] ���쳣������\n") );
		}
	}
	_tprintf( _T("[EH.Exe] [VEH][1] out\n") );
	return nRet;
}

LONG NTAPI LastVectExcepHandler( PEXCEPTION_POINTERS pExcepInfo )
{
	TCHAR* pTitle = _T("*���* VEH�쳣������");
	_tprintf( _T("[EH.Exe] [VEH][2] in \n") );
	LONG nRet = ShowSelectMessageBox(pTitle);
	if ( nRet  == EXCEPTION_EXECUTE_HANDLER )
	{
		_tprintf( _T("[EH.Exe] [VEH][2] EXCEPTION_EXECUTE_HANDLER ��־��VEH����Ч��,����SEH�����á�\n") );
	}
	if ( nRet == EXCEPTION_CONTINUE_EXECUTION )
	{
		if ( MessageBox(0,_T("jmp int3��(����INT3ָ��,���򻹻ᱻ����)"), _T("�Ƿ���������һ��ָ��ִ��"), MB_YESNO) == IDYES )
		{
			pExcepInfo->ContextRecord->Eip += 1;//����int3
			_tprintf( _T("[EH.Exe] [VEH][2] ���쳣������ �ң�jmp int3\n") );
		}else
		{
			_tprintf( _T("[EH.Exe] [VEH][2] ���쳣������\n") );
		}
	}
	_tprintf( _T("[EH.Exe] [VEH][2] out \n") );
	return nRet;
}

LONG NTAPI TopLevelExcepFilter( PEXCEPTION_POINTERS pExcepInfo )
{
	TCHAR* pTitle = _T("*����* �쳣������");
	_tprintf( _T("[EH.Exe] [TOP] in \n") );
	LONG nRet = ShowSelectMessageBox(pTitle);
	_tprintf( _T("[EH.Exe] [TOP] out \n") );;
	return nRet;
}

LONG FirstSEHer( PEXCEPTION_POINTERS pExcepInfo )
{
	TCHAR* pTitle = _T("��һ��SEH������");
	_tprintf( _T("[EH.Exe] [SEH][1] in \n") );
	LONG nRet = ShowSelectMessageBox(pTitle);
	_tprintf( _T("[EH.Exe] [SEH][1] out \n") );
	return nRet;
}
LONG SecondSEHer( PEXCEPTION_POINTERS pExcepInfo )
{
	TCHAR* pTitle = _T("�ڶ���SEH������");
	_tprintf( _T("[EH.Exe] [SEH][2] in \n") );
	LONG nRet = ShowSelectMessageBox(pTitle);
	_tprintf( _T("[EH.Exe] [SEH][2] out \n") );;
	return nRet;
}

LONG ThirdSEHer( PEXCEPTION_POINTERS pExcepInfo )
{
	TCHAR* pTitle = _T("������SEH������");
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
			_tprintf( _T("[EH.Exe] [SEH][1] ����������~(ֻ�з���EXCEPTION_EXECUTE_HANDLER�Ż��ߵ�����)\n"));
		}
	}
	__except( SecondSEHer(GetExceptionInformation()) )
	{
		_tprintf( _T("[EH.Exe] [SEH][2] ����������(ֻ�з���EXCEPTION_EXECUTE_HANDLER�Ż��ߵ�����)\n"));
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
		_tprintf( _T("[EH.Exe] [SEH][3] ����������(ֻ�з���EXCEPTION_EXECUTE_HANDLER�Ż��ߵ�����)\n"));
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
	_tprintf( _T("[EH.Exe] �����˳�\n") );
	return 0;
}

