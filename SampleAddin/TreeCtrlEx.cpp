#include "stdafx.h"
#include "TreeCtrlEx.h"
//#include "MsgDlg.h"
#include "EdgeBarDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx
IMPLEMENT_DYNCREATE(CTreeCtrlEx, CTreeCtrl)
CTreeCtrlEx::CTreeCtrlEx(IMethodUI *pMethod)
	: m_pMethod(pMethod)
{
	m_hCurrentItem = NULL;

	//CMsgDlg dlg(enMsg_Error, _T("可以在产品区域点击“删除”菜单项删除一个产品"), _T("提示"));
	//dlg.DoModal();
}

CTreeCtrlEx::~CTreeCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CTreeCtrlEx, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeCtrlEx)
	//ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	//ON_COMMAND(IDM_ADD, OnAdd)
	//ON_COMMAND(IDM_DELETE, OnDelete)
	//ON_COMMAND(IDM_ADDROOT, OnAddRoot)
	//ON_COMMAND(IDM_DELROOT, OnDeleteRoot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx message handlers
void CTreeCtrlEx::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_TREEVIEW* pNMListView = (NM_TREEVIEW*)pNMHDR;

	//CMenu menu;
	//if (!menu.LoadMenu(IDR_MENU_TOOL))
	//{
	//	return ;
	//}

	//CMenu *pPopMenu = menu.GetSubMenu(0);

	//m_hCurrentItem = pNMListView->itemNew.hItem;

	//DWORD pos = GetMessagePos();
	//POINTS pts = MAKEPOINTS(pos);
	//CPoint point(pts.x, pts.y);
	//ScreenToClient(&point);
	//UINT nFlags = 0;
	//HTREEITEM hItem = HitTest(point, &nFlags);
	//if (hItem == NULL)
	//{
	//	pPopMenu->EnableMenuItem(IDM_ADD, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//	pPopMenu->EnableMenuItem(IDM_DELETE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//	pPopMenu->EnableMenuItem(IDM_DELROOT, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//}
	//else
	//{
	//	SelectItem(hItem);
	//	if ((nFlags & (TVHT_ONITEM|TVHT_ONITEMINDENT|TVHT_ONITEMRIGHT)) != 0)
	//	{
	//		if (GetCurrentItemLevel(hItem) == 0)
	//		{
	//			pPopMenu->EnableMenuItem(IDM_DELETE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//		}
	//		else
	//		{
	//			pPopMenu->EnableMenuItem(IDM_ADDROOT, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//			pPopMenu->EnableMenuItem(IDM_DELROOT, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//		}
	//	}
	//	else
	//	{
	//		pPopMenu->EnableMenuItem(IDM_ADD, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//		pPopMenu->EnableMenuItem(IDM_DELETE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//		pPopMenu->EnableMenuItem(IDM_DELROOT, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	//	}
	//}

	//ClientToScreen(&point);
	//pPopMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this, NULL);

	*pResult = 1;
}

void CTreeCtrlEx::OnAdd() 
{
	OutputDebugString(_T("OnAdd"));
	if (m_pMethod != NULL)
	{
		m_pMethod->DoAddModelData();
	}
}

void CTreeCtrlEx::OnDelete()
{
	OutputDebugString(_T("OnDelete"));
	if (m_pMethod != NULL)
	{
		m_pMethod->DoDeleteModelData();
	}
}

void CTreeCtrlEx::OnAddRoot()
{
	OutputDebugString(_T("OnAddRoot"));
	if (m_pMethod != NULL)
	{
		m_pMethod->DoAddRoot();
	}
}

void CTreeCtrlEx::OnDeleteRoot()
{
	OutputDebugString(_T("OnDeleteRoot"));
	if (m_pMethod != NULL)
	{
		m_pMethod->DoDeleteRoot();
	}
}

int CTreeCtrlEx::GetCurrentItemLevel( HTREEITEM hItem )
{
	int nLevel = 0;
	HTREEITEM hParentItem = GetParentItem(hItem);
	while(hParentItem != NULL)
	{
		++nLevel;
		hParentItem = GetParentItem(hParentItem);
	}

	return nLevel;
}