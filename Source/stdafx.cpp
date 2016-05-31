// stdafx.cpp : ǥ�� ���� ������ �����ϴ� �ҽ� �����Դϴ�.
// GetURLDownload.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj�� �̸� �����ϵ� ���� ������ �����մϴ�.

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

	// ���� ��ü�� �˻�..
	for(int i=0;i<strlen(szSource)+1;i++)
	{
		// �ܾ ã��..
		if (*ptr==nCode || strlen(szSource) == i)
		{
			// �ش� ������ �ܾ����� Ȯ��..
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
			// ������ ���̸� ����
			if(strlen(szSource) == i)
				return FALSE;
			ptrPrev = ptr;
			ptrPrev++;	// code�� �������� ���������� �ν�..
			nCnt++;
		}
		// ���� ������..
		ptr++;
	}
	return FALSE;
}