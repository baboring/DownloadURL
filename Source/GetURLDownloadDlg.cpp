// GetURLDownloadDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "GetURLDownload.h"
#include "GetURLDownloadDlg.h"
#include <afxsock.h>		// MFC socket extensions
#include <afxinet.h>	//ftp �� ���� �߰��ؾߵ�
#include <afxmt.h>
#include <fstream >
#include <iostream >
#include < string >
#include "html.h"

#include ".\geturldownloaddlg.h"

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� ������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGetURLDownloadDlg ��ȭ ����



CGetURLDownloadDlg::CGetURLDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetURLDownloadDlg::IDD, pParent)
	, m_bChkOpenURL(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nCountThread = 0;
}

void CGetURLDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_TEXT, m_reText);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IP1);
	DDX_Control(pDX, IDC_IPADDRESS2, m_IP2);
	DDX_Control(pDX, IDC_LST_LOG, m_lstLog);
	DDX_Check(pDX, IDC_CHK_OPEN_URL, m_bChkOpenURL);
	DDX_Control(pDX, IDC_PROGRESS_THREAD, m_ctrProgress);
	DDX_Control(pDX, IDC_LST_REPORT, m_ctrList);
}

BEGIN_MESSAGE_MAP(CGetURLDownloadDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BN_RUN, OnBnClickedBnRun)
	ON_BN_CLICKED(IDC_BN_BATCH, OnBnClickedBnBatch)
	ON_BN_CLICKED(IDC_CHK_OPEN_URL, OnBnClickedChkOpenUrl)
	ON_BN_CLICKED(IDC_BN_CLEAR_URL, OnBnClickedBnClearUrl)
	ON_BN_CLICKED(IDC_BN_STOP, OnBnClickedBnStop)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LST_REPORT, OnLvnGetdispinfoLstReport)
	ON_NOTIFY(NM_CLICK, IDC_LST_REPORT, OnNMClickLstReport)
	ON_NOTIFY(LVN_ODFINDITEM, IDC_LST_REPORT, OnLvnOdfinditemLstReport)
	ON_BN_CLICKED(IDC_BN_CLEAR_LIST, OnBnClickedBnClearList)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_REPORT, OnNMDblclkLstReport)
	ON_BN_CLICKED(IDC_BN_OPEN_CHECK_ALL, OnBnClickedBnOpenCheckAll)
	ON_BN_CLICKED(IDC_BN_UNCHECK_ALL, OnBnClickedBnUncheckAll)
	ON_BN_CLICKED(IDC_BN_CHECK_ALL, OnBnClickedBnCheckAll)
	ON_BN_CLICKED(IDC_BN_OPEN_NOTE, OnBnClickedBnOpenNote)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, OnIpnFieldchangedIpaddress1)
	ON_BN_CLICKED(IDC_BN_OPEN_SOURCE, OnBnClickedBnOpenSource)
	ON_BN_CLICKED(IDC_BN_RANGE, OnBnClickedBnRange)
END_MESSAGE_MAP()


// CGetURLDownloadDlg �޽��� ó����

BOOL CGetURLDownloadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.
	g_hMainFrame = m_hWnd;
	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	SetDlgItemText(IDC_EDT_URL,"http://127.0.0.1");
	SetDlgItemText(IDC_EDT_IP,"127.0.0.1");


	//�۲� �ʱ�ȭ

	CHARFORMAT cf;
	cf.cbSize = sizeof(cf);
	cf.dwEffects = CFE_PROTECTED;
//	cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_STRIKEOUT | CFM_UNDERLINE | CFM_FACE | CFM_SIZE | CFM_CHARSET | CFM_PROTECTED | CFM_COLOR;
	cf.dwMask = CFM_CHARSET | CFM_FACE | CFM_EFFECTS;
//	cf.yHeight = 200;
	cf.crTextColor = RGB(0, 0, 0);
	
//	cf.bCharSet = HANGUL_CHARSET;		// �ѱ��ϰ��
	cf.bCharSet = JOHAB_CHARSET;		// �ѱ��ϰ��
	strcpy(cf.szFaceName, _T("Tahoma"));
//	strcpy(cf.szFaceName, _T("����"));

	m_reText.SetDefaultCharFormat(cf);

	// WrapNone (���左 ���ϱ�)
	m_reText.SetTargetDevice(NULL, 1);

	// WrapToWindow (���左 �ϱ�)
	//m_reText.SetTargetDevice(NULL, 0);

	m_reText.SetWindowText("�̰Ǹ� �̷�~");

	// Auto Language �ɼ� ����( �ֲ���? )
	//::SendMessage(m_reText.GetSafeHwnd(),EM_SETLANGOPTIONS,0,0);

	m_IP1.SetAddress(127,0,0,1);
	m_IP2.SetAddress(127,0,0,1);

	CListCheck::Initial();

	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

void CGetURLDownloadDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CGetURLDownloadDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
HCURSOR CGetURLDownloadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CGetURLDownloadDlg::DownloadURL_Session(LPCTSTR szImgPath,LPCTSTR szWriteFile)
{
	CInternetSession   is;
	CInternetFile*     pif    = NULL;
	HANDLE             hFile  = NULL;

	pif = (CInternetFile*)is.OpenURL(szImgPath, 1, INTERNET_FLAG_TRANSFER_BINARY);
	if ( pif == NULL )
		return FALSE;

	CHAR szLocalFile[255];
	CString szFileName;
	szFileName.Format("down\\%s.txt",pif->GetFileName());

	hFile = CreateFile(szLocalFile, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		pif->Close();
		is.Close();
		return FALSE;
	}

	while(1)
	{
		INT    nRead        = 0;
		DWORD  dwWritten    = 0;
		CHAR   szBuffer[255];

		nRead = pif->Read(szBuffer, 255);
		if ( nRead == 0 )
			break;

		WriteFile(hFile, szBuffer, nRead, &dwWritten, NULL);

		szBuffer[nRead] = '\0';
	}

	CloseHandle(hFile);
	pif->Close();
	is.Close();

	return TRUE;
}
/*********************************************************************************************************/
BOOL CGetURLDownloadDlg::DownloadWebPage(LPCTSTR szURL)    //1��
/*********************************************************************************************************/
{
	DWORD dwServiceType;
	INTERNET_PORT nPort;
	CString strServer, strObject, strData;
	if(!AfxParseURL(szURL, dwServiceType, strServer, strObject, nPort))
	{
		OutputDebugString("ERROR# URL Parsing\n");
		return FALSE;
	}
	DWORD dwRet;
	CString strHeader = "Content-Type: application/x-www-form-urlencoded";
	CInternetSession session("My Session");
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	try
	{
		pServer = session.GetHttpConnection(strServer, nPort);
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, 0, 1, 0, 0, INTERNET_FLAG_RELOAD);
		pFile->AddRequestHeaders(strHeader);
		pFile->SendRequest();
		pFile->QueryInfoStatusCode(dwRet);

		if(dwRet == HTTP_STATUS_OK)
		{
			char strTemp[512];
			CStdioFile stdFile;
			CString szFileName;
			szFileName.Format("down\\%s.txt",szURL);
			szFileName.Replace('/','_');
			szFileName.Replace(':','_');
			if(stdFile.Open(szFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
			{
				while(pFile->ReadString(strTemp,512))
				{
					stdFile.WriteString(strTemp);
				}
				stdFile.Close();
			}
		}
		else
		{
			OutputDebugString("ERROR# HTTP_STATUS\n");
		}

		delete pFile;
		delete pServer;
	}
	catch(CInternetException* pEx)
	{
		TCHAR szCause[255];
		CString strError;

		pEx->GetErrorMessage(szCause, 255);
		strError.Format("ERROR# Connect Failed[%d] :", pEx->m_dwError);
		strError += szCause;
		strError += _T("\n");
		OutputDebugString(strError);

		session.Close();
		return FALSE;
	}

	session.Close();

	return TRUE;
}

// ====================================================================================================
// ���� üũ
// ====================================================================================================
BOOL CGetURLDownloadDlg::OpenText(LPCTSTR szFIleName)
{
	// TODO: Add your control notification handler code here
	// ===========================================================

	int nCnt = 0;
	int nLen = 0;

	fstream			m_fs;

	m_fs.open( szFIleName ,fstream::in);
	BOOL bResult = !m_fs;
	if(bResult == FALSE)
	{
		LONG size = 0;
		m_fs.seekg (0, ios::end);
		size = m_fs.tellg();

		char* lpTemp = new char[size];
		memset(lpTemp,0,size);
		m_fs.seekg (0, ios::beg);
		m_fs.read (lpTemp, size);



		m_reText.SetWindowText(lpTemp);	// �����...
//		m_reText.ReplaceSel(lpTemp);	// �ڿ� ���̱�

		m_fs.close();
		delete [] lpTemp;

		return TRUE;
	}
	return FALSE;
}
/**********************************************************************************************************/
BOOL CGetURLDownloadDlg::DownloadWebPage2(LPCTSTR szURL)    //2��
/**********************************************************************************************************/
{
	DWORD dwFlags;

	if(!InternetGetConnectedState(&dwFlags, 0))   //���ͳ� ���Ӱ����ϸ�.      
	{
		OutputDebugString("ERROR# InternetGetConnectedState()\n");
		return FALSE;
	}
	CString szFileName;
	szFileName.Format("down\\%s.txt",szURL);
	szFileName.Replace('/','_');
	szFileName.Replace(':','_');
	if(URLDownloadToFile(NULL, szURL, szFileName, 0, NULL) != S_OK) //�������� �ٿ�ε�.
	{
		OutputDebugString("ERROR# URLDownloadToFile()\n");
		return FALSE;
	}

	return TRUE;
}
VOID CGetURLDownloadDlg::AddLog(const char* szLog)
{
	m_lstLog.AddString(szLog);
	m_lstLog.SetCurSel(m_lstLog.GetCount()-1);
}
// ===========================================================================
//
// ===========================================================================
void CGetURLDownloadDlg::OnBnClickedBnRun()
{
	CString szURL;
	GetDlgItemText(IDC_EDT_URL,szURL);
	if(szURL.Left(7).CompareNoCase("http://"))
	{
		szURL = "http://" + szURL;
		SetDlgItemText(IDC_EDT_URL,szURL);
	}

	CString szFileName;
	szFileName.Format("down\\%s.txt",szURL);
	szFileName.Replace('/','_');
	szFileName.Replace(':','_');

	DEBUG_MESSAGE("������ %s",szURL);

	if(DownloadURL(szURL) == FALSE)
		DEBUG_MESSAGE("������� %s",szURL);
	else
	{
		DEBUG_MESSAGE("���Ἲ�� %s",szURL);
		FILE* fp = fopen(szFileName,"rt");
		if(fp)
		{
			fclose(fp);
			OpenText(szFileName);
			HtmlParser parser;
			parser.LoadHtml(szFileName.GetBuffer());

		}
	}
}
// ===========================================================================
//
// ===========================================================================
BOOL CGetURLDownloadDlg::DownloadURL(const char* szURL)
{
	if(DownloadWebPage(szURL) == FALSE)
		return FALSE;
	return TRUE;
}
// ===========================================================
//
// ===========================================================
struct THREAMPARAM
{
	CGetURLDownloadDlg* lpThis;
	char	szURL[128];
	int		nNum;
};
// ===========================================================
//
// ===========================================================
DWORD WINAPI ThreadDownloadURL(LPVOID arg)
{
	//	DWORD tid1;
	THREAMPARAM* lParam = (THREAMPARAM*)arg;
	char szURL[128];
	int nNum = lParam->nNum;
	strcpy(szURL,lParam->szURL);

	if(lParam->lpThis->ProcessURL(szURL))
		THREAD_END(nNum,1,szURL);
	else
		THREAD_END(nNum,0,szURL);
	return 0;
}
// ===========================================================
//
// ===========================================================
DWORD WINAPI ThreadProcess(LPVOID arg)
{
	//	DWORD tid1;
	CGetURLDownloadDlg* lpThis = (CGetURLDownloadDlg*)arg;
	lpThis->BatchProcess();
	return 0;
}
// ===========================================================================
//
// ===========================================================================
BOOL CGetURLDownloadDlg::ProcessURL(CString szURL)
{
	CString szFileName;
	szFileName.Format("down\\%s.txt",szURL);
	szFileName.Replace('/','_');
	szFileName.Replace(':','_');

	//	if(DownloadURL(szURL,"") == FALSE)
	if(DownloadURL(szURL) == FALSE)
	{
		DEBUG_MESSAGE("������� %s",szURL);
	}
	else
	{
		DEBUG_MESSAGE("���Ἲ�� %s",szURL);
		FILE* fp = fopen(szFileName,"rt");
		if(fp)
		{
			fclose(fp);
			if(m_bChkOpenURL)
				ShellExecute(NULL, "open", szURL, NULL, NULL, SW_SHOW);
			return TRUE;
		}
	}
	return FALSE;
}
void CGetURLDownloadDlg::BatchProcess()
{
	CString szURL;
	char szServerIP[128] = "";
	DWORD dwIP = 0;
	DWORD dwIP2 = 0;
	m_IP1.GetAddress(dwIP);
	m_IP2.GetAddress(dwIP2);

	m_ctrProgress.SetRange(0,(dwIP2-dwIP)+1);
	m_ctrProgress.SetPos(0);

	g_bExitProcess = FALSE;

	CWnd* lpWnd = GetDlgItem(IDOK);
	if(lpWnd)
		lpWnd->EnableWindow(FALSE);

	DEBUG_MESSAGE("�۾� ���� --------------------------------");
	for(int nCnt=0;dwIP<=dwIP2;dwIP++,nCnt++)
	{
		int nClassA = (dwIP >> 24)&0xff;
		int nClassB = (dwIP >> 16)&0xff;
		int nClassC = (dwIP >> 8)&0xff;
		int nClassD = (dwIP >> 0)&0xff;

//		THREAD_BEGIN(nCnt,"http://%d.%d.%d.%d/",nClassA,nClassB,nClassC,nClassD);
		sprintf(szServerIP,"http://%d.%d.%d.%d/",nClassA,nClassB,nClassC,nClassD);
		// --------------------------
		CWebInfo info;
		info.m_URL = szServerIP;
		info.m_image = 0;
		info.m_checked = 0;
		info.m_nStatus = 0;
		info.m_Comment = "";
		int nNum = CListCheck::GetCount();
		CListCheck::Add(info);
		CListCheck::Redraw();

	}
	// Check Thread
	int i=0;
	while(1)
	{
		if(m_nCountThread<32)
		{
			int nWorks = 0;
			for(i=0;i<CListCheck::m_database.size();i++)
			{
				if(CListCheck::m_database[i].m_nStatus == 0)
				{
					if(g_bExitProcess)
						CListCheck::m_database[i].m_Comment = "����";
					nWorks++;
				}
			}
			if(nWorks<1 || g_bExitProcess)
				break;
			// üũ�Ұ͵��� �˻��ؼ� ó��
			for(i=0;i<CListCheck::m_database.size();i++)
			{
				if(CListCheck::m_database[i].m_nStatus == 0)
				{
					CListCheck::m_database[i].m_nStatus = 1;
					CListCheck::m_database[i].m_Comment = "������";
					CListCheck::Redraw();
					THREAMPARAM param;
					param.lpThis = this;
					param.nNum = i;
					strcpy(param.szURL,CListCheck::m_database[i].m_URL);
					// --------------------------
					m_nCountThread++;
					SetDlgItemInt(IDC_LBL_THRED_COUNT,m_nCountThread,TRUE);
					DWORD tid;
					CreateThread(0,0,&ThreadDownloadURL,&param,0,&tid);	
					DEBUG_MESSAGE("[%d] ������ %s",i+1,CListCheck::m_database[i].m_URL);
					Sleep(100);
					break;
				}
			}
		}
		Sleep(100);
	}
	// 
}

void CGetURLDownloadDlg::OnBnClickedBnBatch()
{
	if(m_nCountThread>0)
	{
		AfxMessageBox("�������� �۾��� �ֽ��ϴ�. �۾��� �Ϸ�Ǹ� �ٽ� �õ��ϼ���",MB_OK);
		return;
	}
	DWORD tid;
	CreateThread(0,0,&ThreadProcess,this,0,&tid);	

}

LRESULT CGetURLDownloadDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	switch(message)
	{
	case UM_DEBUG_MESSAGE:
		AddLog((char*)lParam);
		break;
	case UM_THRED_BEGIN:
		break;
	case UM_THRED_END:
		m_nCountThread--;
		{
			int nNum = LOWORD(wParam);
			int nFlag = HIWORD(wParam);

			if(nNum > -1 && nNum < CListCheck::GetCount())
			{
				int nWorks = 0;
				int nFound = 0;
				for(int i=0;i<CListCheck::m_database.size();i++)
				{
					if(CListCheck::m_database[i].m_nStatus == 0)
						nWorks++;
					if(CListCheck::m_database[i].m_image == 1)
						nFound++;
				}
				// ���� ���
				m_ctrProgress.SetPos(CListCheck::m_database.size()-nWorks -m_nCountThread);
				CString szStatus;
				szStatus.Format("%d/%d",nFound,CListCheck::m_database.size());
				SetDlgItemText(IDC_LBL_RESULT,szStatus);

				// �۾� ������ ó��
				CListCheck::m_database[nNum].m_nStatus = 2;
				if(nFlag)
				{
					CString szFileName;
					szFileName.Format("down\\%s.txt",m_database[nNum].m_URL);
					szFileName.Replace('/','_');
					szFileName.Replace(':','_');

					HtmlParser parser;
					parser.LoadHtml(szFileName.GetBuffer());
					
					m_database[nNum].m_Title = 	parser.m_szTitle;
					m_database[nNum].m_image = 1;
					m_database[nNum].m_Comment = "����";
				}
				else
				{
					m_database[nNum].m_image = 2;
					m_database[nNum].m_Comment = "����";
				}
				CListCheck::Redraw();
			}
		}
		SetDlgItemInt(IDC_LBL_THRED_COUNT,m_nCountThread,TRUE);
		if(m_nCountThread<1)
		{
			DEBUG_MESSAGE("�۾� �Ϸ� --------------------------------");
			OnBnClickedBnOpenNote();
			PlayFx("complete.wav");
		}

		break;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CGetURLDownloadDlg::OnBnClickedChkOpenUrl()
{
	UpdateData();
}

void CGetURLDownloadDlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//	CDialog::OnOK();
}

void CGetURLDownloadDlg::OnBnClickedBnClearUrl()
{
	m_reText.SetWindowText("");
}

void CGetURLDownloadDlg::OnBnClickedBnStop()
{
	g_bExitProcess = TRUE;
}

BOOL CGetURLDownloadDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return __super::DestroyWindow();
}

void CGetURLDownloadDlg::OnLvnGetdispinfoLstReport(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CListCheck::OnGetdispinfoList(pNMHDR,pResult);
	*pResult = 0;
}

void CGetURLDownloadDlg::OnNMClickLstReport(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CListCheck::OnClickList(pNMHDR,pResult);

	int nNum = CListCheck::m_ctrList.GetSelectionMark();
	if(nNum != -1)
	{
		SetDlgItemText(IDC_EDT_SELECT_URL,CListCheck::m_database[nNum].m_URL);

		CStdioFile stdFile;
		CString szFileName;
		szFileName.Format("down\\%s.txt",CListCheck::m_database[nNum].m_URL);
		szFileName.Replace('/','_');
		szFileName.Replace(':','_');

		OpenText(szFileName);
	}

	*pResult = 0;
}

void CGetURLDownloadDlg::OnLvnOdfinditemLstReport(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVFINDITEM pFindInfo = reinterpret_cast<LPNMLVFINDITEM>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CListCheck::OnOdfinditemList(pNMHDR,pResult);
	*pResult = 0;
}

void CGetURLDownloadDlg::OnBnClickedBnClearList()
{
	m_database.clear();
	CListCheck::Redraw();
}

void CGetURLDownloadDlg::OnNMDblclkLstReport(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nNum = CListCheck::m_ctrList.GetSelectionMark();
	if(nNum != -1)
	{
		ShellExecute(NULL, "open", CListCheck::m_database[nNum].m_URL, NULL, NULL, SW_SHOW);
	}
	*pResult = 0;
}

void CGetURLDownloadDlg::OnBnClickedBnOpenCheckAll()
{
	UpdateData();
	for(int i=0;i<CListCheck::m_database.size();i++)
	{
		if(CListCheck::m_database[i].m_checked)
		{
			ShellExecute(NULL, "open", CListCheck::m_database[i].m_URL, NULL, NULL, SW_SHOW);
		}
	}	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
void CGetURLDownloadDlg::OnBnClickedBnUncheckAll()
{
	UpdateData();
	for(int i=0;i<CListCheck::m_database.size();i++)
	{
		CListCheck::m_database[i].m_checked = FALSE;
	}
	CListCheck::Redraw();
}

void CGetURLDownloadDlg::OnBnClickedBnCheckAll()
{
	UpdateData();
	for(int i=0;i<CListCheck::m_database.size();i++)
	{
		if(CListCheck::m_database[i].m_image == 1)
			CListCheck::m_database[i].m_checked = TRUE;
	}
	CListCheck::Redraw();
}

void CGetURLDownloadDlg::OnBnClickedBnOpenNote()
{
	UpdateData();
	DWORD dwIP = 0;
	DWORD dwIP2 = 0;
	m_IP1.GetAddress(dwIP);
	m_IP2.GetAddress(dwIP2);
	int nClassA = (dwIP >> 24)&0xff;
	int nClassB = (dwIP >> 16)&0xff;
	int nClassC = (dwIP >> 8)&0xff;
	int nClassD = (dwIP >> 0)&0xff;

	int nClassA2 = (dwIP2 >> 24)&0xff;
	int nClassB2 = (dwIP2 >> 16)&0xff;
	int nClassC2 = (dwIP2 >> 8)&0xff;
	int nClassD2 = (dwIP2 >> 0)&0xff;

	CString szTitle;
	CString szFileName;

	szTitle.Format("%d.%d.%d.%d - %d.%d.%d.%d"
					,nClassA,nClassB,nClassC,nClassD
					,nClassA2,nClassB2,nClassC2,nClassD2);

	int nTotal = 0;
	for(int i=0;i<CListCheck::m_database.size();i++)
	{
		// �����Ѱ͵鸸
		if(CListCheck::m_database[i].m_image == 1)
			nTotal++;
	}
	szFileName.Format("%s(%d).txt",szTitle,nTotal);

	FILE* fp = fopen(szFileName,"wt");
	if(fp)
	{
		fprintf(fp,"%s\n",szTitle);
//		char szTemp1[256];
		for( i=0;i<CListCheck::m_database.size();i++)
		{
			// �����Ѱ͵鸸
			if(CListCheck::m_database[i].m_image == 1)
			{
				CListCheck::m_database[i].m_Title.TrimRight();
				CListCheck::m_database[i].m_Title.Replace(-82,' ');
				CListCheck::m_database[i].m_Title.Replace('��',' ');
				
//				strcpy(szTemp1,CListCheck::m_database[i].m_Title);
				fprintf(fp,"%s\t%s\n",CListCheck::m_database[i].m_URL,CListCheck::m_database[i].m_Title);
			}
		}	
		fclose(fp);
		ShellExecute(NULL, "open", "notepad",szFileName, NULL, SW_SHOW);
	}
}

void CGetURLDownloadDlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	DWORD dwIP = 0;
	m_IP1.GetAddress(dwIP);
	int nClassA = (dwIP >> 24)&0xff;
	int nClassB = (dwIP >> 16)&0xff;
	int nClassC = (dwIP >> 8)&0xff;
	int nClassD = (dwIP >> 0)&0xff;
	m_IP2.SetAddress(nClassA,nClassB,nClassC,255);

	*pResult = 0;
}

void CGetURLDownloadDlg::OnBnClickedBnOpenSource()
{
	int nNum = CListCheck::m_ctrList.GetSelectionMark();
	if(nNum != -1)
	{
		CStdioFile stdFile;
		CString szFileName;
		szFileName.Format("down\\%s.txt",CListCheck::m_database[nNum].m_URL);
		szFileName.Replace('/','_');
		szFileName.Replace(':','_');

		if(OpenText(szFileName))
			ShellExecute(NULL, "open", "notepad",szFileName,  NULL, SW_SHOW);
		else
			AfxMessageBox("������ �������� �ʽ��ϴ�.");
	}
}

void CGetURLDownloadDlg::OnBnClickedBnRange()
{
	char szTemp[128];
	GetDlgItemText(IDC_EDT_IP,szTemp,sizeof(szTemp));

	char szIP[4][128] = {"",};
	ParseStr(szTemp,szIP[0],'.',0);
	ParseStr(szTemp,szIP[1],'.',1);
	ParseStr(szTemp,szIP[2],'.',2);
	ParseStr(szTemp,szIP[3],'.',3);

	m_IP1.SetAddress(atoi(szIP[0]),atoi(szIP[1]),atoi(szIP[2]),atoi(szIP[3]));
	m_IP2.SetAddress(atoi(szIP[0]),atoi(szIP[1]),atoi(szIP[2]),atoi(szIP[3]));

	UpdateData(FALSE);

}
