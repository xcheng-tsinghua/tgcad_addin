#pragma once

// CTreeCtrlEx window
class IMethodUI;
class CTreeCtrlEx : public CTreeCtrl
{
	DECLARE_DYNCREATE(CTreeCtrlEx)
// Construction
public:
	CTreeCtrlEx(IMethodUI *pMethod = NULL);
	virtual ~CTreeCtrlEx();

protected:
	int GetCurrentItemLevel(HTREEITEM hItem);

protected:
	HTREEITEM m_hCurrentItem;

	IMethodUI *m_pMethod;
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCtrlEx)
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnAddRoot();
	afx_msg void OnDeleteRoot();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};