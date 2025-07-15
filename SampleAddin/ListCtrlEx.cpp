#include "stdafx.h"
#include "ListCtrlEx.h"
#include "EdgeBarDlg.h"
//#include "DataCollection.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE  
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx
IMPLEMENT_DYNCREATE(CListCtrlEx, CListCtrl)
CListCtrlEx::CListCtrlEx(IMethodUI *pMethod)
	: m_pMethod(pMethod)
{
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	//ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//ON_NOTIFY_REFLECT(NM_RCLICK, OnRClick)
	//ON_COMMAND(ID_LIST_OPENMODEL, OnOpenModel)
	//ON_COMMAND(ID_LIST_TESTALL, OnTestAllCfg)
	//ON_COMMAND(ID_LIST_DEL, OnDeleteItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers
void CListCtrlEx::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	DWORD dwData = 0;
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
 		int nItem = GetNextSelectedItem(pos);
		dwData = GetItemData(nItem);
	}

	if (m_pMethod != NULL && dwData != 0)
	{
		m_pMethod->DoDblclkListItem(dwData);
	}

	*pResult = 0;
}

void CListCtrlEx::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// TODO: Add your control notification handler code here
	m_nSelectedItem = pNMListView->iItem;
	//if (m_nSelectedItem > -1)
	//{
	//	CMenu menu;
	//	if (!menu.LoadMenu(IDR_MENU_TOOL))
	//	{
	//		return ;
	//	}

	//	CMenu *pPopMenu = menu.GetSubMenu(1);

	//	DWORD dwWord = GetItemData(m_nSelectedItem);
	//	if (GetPartID(dwWord) == 0)
	//	{
	//		pPopMenu->EnableMenuItem(ID_LIST_OPENMODEL, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//		pPopMenu->EnableMenuItem(ID_LIST_TESTALL, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//	}

	//	CPoint pt(pNMListView->ptAction.x, pNMListView->ptAction.y);
	//	ClientToScreen(&pt);
	//	pPopMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, pt.x, pt.y, this, NULL);
	//}

	*pResult = 1;
}

void CListCtrlEx::OnLargeIcon()
{
	ModifyStyle(LVS_REPORT, LVS_ICON);
}

void CListCtrlEx::OnSmallIcon()
{
	ModifyStyle(LVS_ICON, LVS_REPORT);
}

void CListCtrlEx::OnDeleteItem()
{
	DWORD dwData = 0;
	CString strItemName;
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = GetNextSelectedItem(pos);
		dwData = GetItemData(nItem);
		strItemName = GetItemText(nItem, 0);
	}

	if (m_pMethod != NULL && dwData != 0)
	{
		m_pMethod->DoDeleteListItem(dwData, strItemName);
	}
}

void CListCtrlEx::OnTestAllCfg()
{
	DWORD dwData = 0;
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = GetNextSelectedItem(pos);
		dwData = GetItemData(nItem);
	}

	if (m_pMethod != NULL && dwData != 0)
	{
		m_pMethod->DoTestModelAllCfg(dwData);
	}
}

void CListCtrlEx::OnOpenModel()
{
	DWORD dwData = 0;
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos != NULL) 
	{
		int nItem = GetNextSelectedItem(pos);
		dwData = GetItemData(nItem);
	}

	if (m_pMethod != NULL && dwData != 0)
	{
		m_pMethod->DoDblclkListItem(dwData);
	}
}