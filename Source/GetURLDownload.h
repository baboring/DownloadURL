// GetURLDownload.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CGetURLDownloadApp:
// �� Ŭ������ ������ ���ؼ��� GetURLDownload.cpp�� �����Ͻʽÿ�.
//

class CGetURLDownloadApp : public CWinApp
{
public:
	CGetURLDownloadApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CGetURLDownloadApp theApp;
