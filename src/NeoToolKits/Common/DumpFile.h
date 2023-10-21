#pragma once
#include <windows.h>
#include <Dbghelp.h>
#include <iostream>  
#include <vector>  
#include <stdlib.h>
using namespace std; 

#include <QTime>
#include <QString>
#include <QCoreApplication>

#pragma comment(lib, "Dbghelp.lib")


namespace NSDumpFile
{ 
	void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)  
	{  
		// 创建Dump文件  
		//  
		HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  


		// Dump信息  
		//  
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;  
		dumpInfo.ExceptionPointers = pException;  
		dumpInfo.ThreadId = GetCurrentThreadId();  
		dumpInfo.ClientPointers = TRUE;  


		// 写入Dump文件内容  
		//  
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);  


		CloseHandle(hDumpFile);  
	}  


	LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
	{
		Q_UNUSED(lpTopLevelExceptionFilter)
		return NULL;
	}

	BOOL PreventSetUnhandledExceptionFilter()
	{
		HMODULE hKernel32 = LoadLibrary(L"kernel32.dll");
		if (hKernel32 ==   NULL)
			return FALSE;


		void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
		if(pOrgEntry == NULL)
			return FALSE;


		unsigned char newJump[ 100 ];
		DWORD dwOrgEntryAddr = (DWORD) pOrgEntry;
		dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far


		void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
		DWORD dwNewEntryAddr = (DWORD) pNewFunc;
		DWORD dwRelativeAddr = dwNewEntryAddr -  dwOrgEntryAddr;

		newJump[ 0 ] = 0xE9;  // JMP absolute
		memcpy(&newJump[ 1 ], &dwRelativeAddr, sizeof(pNewFunc));
		SIZE_T bytesWritten;
		BOOL bRet = WriteProcessMemory(GetCurrentProcess(),	pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);
		return bRet;
	}


	LONG WINAPI UnhandledExceptionFilterEx(struct _EXCEPTION_POINTERS *pException)
	{
		QString strDumpFileName = QString("%1/%2_CrashDumpFile.dmp").arg(QCoreApplication::applicationDirPath()).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz"));
		CreateDumpFile(strDumpFileName.toStdWString().c_str(), pException);

		/*TCHAR strTime[MAX_PATH] = {0};
		wsprintf(strTime,L"%d-%d-%d-%d-%d-%d_",mTime.GetYear(),mTime.GetMonth(),mTime.GetDay(),mTime.GetHour(),mTime.GetMinute(),mTime.GetSecond());
		TCHAR szMbsFile[MAX_PATH] = { 0 };
		::GetModuleFileName(NULL, szMbsFile, MAX_PATH);
		TCHAR* pFind = _tcsrchr(szMbsFile, '\\');
		if(pFind)
		{
			*(pFind+1) = 0;
			_tcscat(szMbsFile,strTime);
			_tcscat(szMbsFile, L"CrashDumpFile.dmp");
			CreateDumpFile(szMbsFile, pException);
		}*/

		// TODO: MiniDumpWriteDump
		FatalAppExit(-1, L"The software crashed!!!   Please analyze the.dmp file.");
		return EXCEPTION_CONTINUE_SEARCH;
	}


	void RunCrashHandler()
	{
		SetUnhandledExceptionFilter(UnhandledExceptionFilterEx);
		PreventSetUnhandledExceptionFilter();
	}
};


#define DeclareDumpFile() NSDumpFile::RunCrashHandler();

