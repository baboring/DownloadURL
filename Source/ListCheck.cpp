#include "stdafx.h"
#include ".\listcheck.h"
#include "resource.h"

// =========================================
//
// =========================================
CListCheck::CListCheck(void)
{
}
// =========================================
//
// =========================================
CListCheck::~CListCheck(void)
{
}
// =========================================
//
// =========================================
BOOL CListCheck::IsCheckBoxesVisible()
{
	DWORD style = m_ctrList.GetStyle();

	if( ((style & LVS_TYPEMASK) == LVS_LIST) ||
		((style & LVS_TYPEMASK) == LVS_REPORT) )
		return TRUE;

	return FALSE;
}
// =========================================
//
// =========================================
void CListCheck::Clear()
{
	//Clear database and list.
	m_database.clear();

	m_ctrList.SetItemCount(0);

}
// =========================================
//
// =========================================
BOOL CListCheck::Initial()
{
	ListView_SetExtendedListViewStyle(m_ctrList.m_hWnd, LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES);

	//Create image list
	m_imagelist.Create(IDB_BITMAP_SMALL, 16, ILC_COLOR4, RGB(255, 255, 255));

	m_ctrList.SetImageList(&m_imagelist, LVSIL_SMALL);

	m_ctrList.InsertColumn(0, _T("URL"), LVCFMT_LEFT, 200);
	m_ctrList.InsertColumn(1, _T("Title"), LVCFMT_LEFT, 260);
	m_ctrList.InsertColumn(2, _T("Comment"), LVCFMT_LEFT, 70);

	return TRUE;
}
// =========================================
//
// =========================================
void CListCheck::Redraw()
{
//	m_ctrList.SetItemCount( m_database.size() );

	// 깜박임을 줄여준단다..
	m_ctrList.SetItemCountEx( m_database.size() );
}
// =========================================
//
// =========================================
void CListCheck::Add(CWebInfo& info)
{
	m_database.push_back(info);
}
// =========================================
//
// =========================================
void CListCheck::ToggleCheckBox(int item)
{
	//Change check box
	m_database[item].m_checked = !m_database[item].m_checked;

	//And redraw
	m_ctrList.RedrawItems(item, item);
}
// =========================================
//
// =========================================
BOOL CListCheck::DeleteItem(int nNum)
{
	if(nNum> -1 && nNum < m_database.size())
	{
		m_database.erase(m_database.begin()+nNum);
		m_ctrList.DeleteItem( nNum);
		return TRUE;
	}
	return FALSE;
}
// =========================================
//
// =========================================
void CListCheck::ChangeStyle(int nStyle)
{
	//Remove old style, and add new
	m_ctrList.ModifyStyle(LVS_TYPEMASK, nStyle);

	//Turn on/off check box
	if(nStyle == LVS_ICON || nStyle == LVS_SMALLICON)
		//Off
		ListView_SetExtendedListViewStyleEx(m_ctrList.m_hWnd, LVS_EX_CHECKBOXES, 0);
	else
		//On
		ListView_SetExtendedListViewStyleEx(m_ctrList.m_hWnd, LVS_EX_CHECKBOXES, LVS_EX_CHECKBOXES);
}
// =========================================
//This is called when a key i pressed down
// =========================================
void CListCheck::OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDown = (LV_KEYDOWN*)pNMHDR;

	//If user press space, toggle flag on selected item
	if( pLVKeyDown->wVKey == VK_SPACE )
	{
		//Check if check boxes are visible
		if( IsCheckBoxesVisible() )
		{
			//Toggle if some item is selected
			if(m_ctrList.GetSelectionMark() != -1)
				ToggleCheckBox( m_ctrList.GetSelectionMark() );
		}
	}

	*pResult = 0;
}
// =========================================
//This is called when user clicks on the list. 
//If you had check box you have to write code to make them possible to toggle.
// =========================================
void CListCheck::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LVHITTESTINFO hitinfo;
	//Copy click point
	hitinfo.pt = pNMListView->ptAction;

	//Make the hit test...
	int item = m_ctrList.HitTest(&hitinfo); 

	// 여기서는 안씀
	if(item != -1)
	{
		// 체크 버튼만 클릭했을때
		if( hitinfo.flags == LVHT_ONITEMSTATEICON)
		{
			ToggleCheckBox(item);
		}
	}


	*pResult = 0;
}
// =========================================
//This functions is called when the user "writes" in the list box to find an item.
// =========================================
void CListCheck::OnOdfinditemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// pNMHDR has information about the item we should find
	// In pResult we should save which item that should be selected
	NMLVFINDITEM* pFindInfo = (NMLVFINDITEM*)pNMHDR;

	/* pFindInfo->iStart is from which item we should search.
	We search to bottom, and then restart at top and will stop
	at pFindInfo->iStart, unless we find an item that match
	*/

	// Set the default return value to -1
	// That means we didn't find any match.
	*pResult = -1;

	//Is search NOT based on string?
	if( (pFindInfo->lvfi.flags & LVFI_STRING) == 0 )
	{
		//This will probably never happend...
		return;
	}

	/*
	Let's look on a sample list;

	Name
	Anders
	* Anna
	Annika
	Bob
	Emma
	Emmanuel

	Anna is selected. 
	If "A" is written, Annika should be selected.
	If "AND" is written, Anders should be selected. 
	If "ANNK" is written, the selection should stay on Anna.
	If "E" is written, Emma should be selected.

	*/

	//This is the string we search for
	CString searchstr = pFindInfo->lvfi.psz;

	//	TRACE(_T("Find: %s\n"), searchstr);

	int startPos = pFindInfo->iStart;
	//Is startPos outside the list (happens if last item is selected)
	if(startPos >= m_ctrList.GetItemCount())
		startPos = 0;

	int currentPos=startPos;

	//Let's search...
	do
	{		
		//Do this word begins with all characters in searchstr?
		if( _tcsnicmp(m_database[currentPos].m_URL, searchstr, searchstr.GetLength()) == 0)
		{
			//Select this item and stop search.
			*pResult = currentPos;
			break;
		}

		//Go to next item
		currentPos++;

		//Need to restart at top?
		if(currentPos >= m_ctrList.GetItemCount())
			currentPos = 0;

		//Stop if back to start
	}while(currentPos != startPos);		
}
// =========================================
//This function is called when the list needs data. This is the most
//critical function when working with virtual lists.
// =========================================
void CListCheck::OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	//Create a pointer to the item
	LV_ITEM* pItem= &(pDispInfo)->item;

	//Which item number?
	int itemid = pItem->iItem;

	//Do the list need text information?
	if (pItem->mask & LVIF_TEXT)
	{
		CString text;

		//Which column?
		if(pItem->iSubItem == 0)
		{
			//Text is name
			text = m_database[itemid].m_URL;
		}
		else if (pItem->iSubItem == 1)
		{
			//Text is slogan
			text = m_database[itemid].m_Title;
		}
		else if (pItem->iSubItem == 2)
		{
			//Text is slogan
			text = m_database[itemid].m_Comment;
		}

		//Copy the text to the LV_ITEM structure
		//Maximum number of characters is in pItem->cchTextMax
		lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
	}

	//Do the list need image information?
	if( pItem->mask & LVIF_IMAGE) 
	{
		//Set which image to use
		pItem->iImage=m_database[itemid].m_image;

		//Show check box?
		if(IsCheckBoxesVisible())
		{
			//To enable check box, we have to enable state mask...
			pItem->mask |= LVIF_STATE;
			pItem->stateMask = LVIS_STATEIMAGEMASK;

			if(m_database[itemid].m_checked)
			{
				//Turn check box on..
				pItem->state = INDEXTOSTATEIMAGEMASK(2);
			}
			else
			{
				//Turn check box off
				pItem->state = INDEXTOSTATEIMAGEMASK(1);
			}
		}
	}

	*pResult = 0;
}