// GetURLDownloadDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "listcheck.h"



// CGetURLDownloadDlg ��ȭ ����
class CGetURLDownloadDlg : public CDialog,public CListCheck
{
// ����
public:
	CGetURLDownloadDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_GETURLDOWNLOAD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����

	BOOL DownloadWebPage(LPCTSTR szURL);    //2��
	BOOL DownloadWebPage2(LPCTSTR szURL);    //2��
	BOOL DownloadURL_Session(LPCTSTR szImgPath,LPCTSTR szWriteFile);
public:
	BOOL OpenText(LPCTSTR szFIleName);
	VOID AddLog(const char* szLog);
	BOOL ProcessURL(CString szURL);
	BOOL DownloadURL(const char* szURL);
	void BatchProcess();

	int	m_nCountThread;
// ����
protected:
	HICON m_hIcon;

	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBnRun();
	CRichEditCtrl m_reText;
	CIPAddressCtrl m_IP1;
	CIPAddressCtrl m_IP2;
	CListBox m_lstLog;
	afx_msg void OnBnClickedBnBatch();
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	BOOL m_bChkOpenURL;
	afx_msg void OnBnClickedChkOpenUrl();
	CProgressCtrl m_ctrProgress;
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedBnClearUrl();
	afx_msg void OnBnClickedBnStop();

	virtual BOOL DestroyWindow();
	afx_msg void OnLvnGetdispinfoLstReport(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLstReport(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnOdfinditemLstReport(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBnClearList();
	afx_msg void OnNMDblclkLstReport(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBnOpenCheckAll();
	afx_msg void OnBnClickedBnUncheckAll();
	afx_msg void OnBnClickedBnCheckAll();
	afx_msg void OnBnClickedBnOpenNote();
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBnOpenSource();
	afx_msg void OnBnClickedBnRange();
};
