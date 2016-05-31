// stdafx.cpp : 표준 포함 파일을 포함하는 소스 파일입니다.
// GetURLDownload.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj는 미리 컴파일된 형식 정보를 포함합니다.

#include "stdafx.h"
#include "mmsystem.h"

HWND g_hMainFrame = NULL;
BOOL g_bExitProcess = FALSE;

#pragma comment(lib, "winmm.lib")
// ========================================================================================
// 
// ========================================================================================
void PlayFx(const char* szFileName)
{
	PlaySound(szFileName,NULL,SND_ASYNC );
}
// =================================================================
//
// =================================================================
void DEBUG_MESSAGE(char *fmt, ...)
{
	char buf[512];
	va_list argptr;	

	va_start(argptr, fmt);
	vsprintf(buf, fmt, argptr);
	va_end(argptr);

	if(g_hMainFrame)
		::SendMessage( g_hMainFrame, UM_DEBUG_MESSAGE, 0, (LPARAM)buf);
}

// =================================================================
//
// =================================================================
void THREAD_BEGIN(int nNum,char *fmt, ...)
{
	char buf[512];
	va_list argptr;	

	va_start(argptr, fmt);
	vsprintf(buf, fmt, argptr);
	va_end(argptr);

	if(g_hMainFrame)
		::SendMessage( g_hMainFrame, UM_THRED_BEGIN, nNum, (LPARAM)buf);
}
// =================================================================
//
// =================================================================
void THREAD_END(int nNum,int nFlag,char *fmt, ...)
{
	char buf[512];
	va_list argptr;	

	va_start(argptr, fmt);
	vsprintf(buf, fmt, argptr);
	va_end(argptr);

	if(g_hMainFrame)
		::SendMessage( g_hMainFrame, UM_THRED_END, MAKEWPARAM(nNum,nFlag), (LPARAM)buf);
}
// ==========================================================================
BOOL ParseStr(char *szSource,char *szDest,char nCode,int nNum)
{
	char *ptr = szSource;
	char *ptrPrev = NULL;
	int nCnt=0;
	ptrPrev = ptr;

	// 문장 전체를 검사..
	for(int i=0;i<strlen(szSource)+1;i++)
	{
		// 단어를 찾음..
		if (*ptr==nCode || strlen(szSource) == i)
		{
			// 해단 순번의 단어인지 확인..
			if(nCnt == nNum)
			{
				if(ptrPrev && ptr-ptrPrev>0)
				{
					if(ptr-ptrPrev>0 || strlen(szSource) == i)
					{
						strncpy(szDest,ptrPrev,ptr-ptrPrev);
						if(szDest[0] == nCode)
							szDest[0] = '\0';
						szDest[ptr-ptrPrev] = NULL;
					}
					else
						szDest[0] = NULL;
				}
				return TRUE;
			}
			// 문장의 끝이면 종료
			if(strlen(szSource) == i)
				return FALSE;
			ptrPrev = ptr;
			ptrPrev++;	// code값 다음부터 문장작으로 인식..
			nCnt++;
		}
		// 다음 포인터..
		ptr++;
	}
	return FALSE;
}