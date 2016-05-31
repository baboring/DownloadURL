#pragma once

#include <vector>

using namespace std;

const int IMAGECOUNT = 4;

class CWebInfo
{
public:
	CWebInfo()
	{
		m_URL = "";
		m_Title = "";
		m_Comment = "";
		m_image = 0;
		m_checked = 0;
		m_nStatus = 0;
	}

	CString m_URL;		// URL
	CString m_Title;	// Title Name
	CString m_Comment;
	int m_image;		// connect status
	bool m_checked;		// check status
	bool m_nStatus;		// 0 : ready, 1 : work , 2 : Finish
};

class CListCheck
{
public:
	//This is the database we using
	vector<CWebInfo> m_database;

protected:
	//Image list
	CImageList m_imagelist;

	CListCtrl	m_ctrList;

public:
	BOOL	IsCheckBoxesVisible();
	void	ToggleCheckBox(int item);
	void	ChangeStyle(int nStyle);
	void	Clear();
	BOOL	Initial();
	void	Redraw();

	int		GetCount()		{ return m_database.size();}

	void	Add(CWebInfo& info);

	BOOL	DeleteItem(int nNum);

	void OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult);
	void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	void OnOdfinditemList(NMHDR* pNMHDR, LRESULT* pResult);
	void OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult);

	CListCheck(void);
	~CListCheck(void);
};
