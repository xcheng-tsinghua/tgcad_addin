#pragma once

// CListCtrlEx window
class IMethodUI;
class CListCtrlEx : public CListCtrl
{
	DECLARE_DYNCREATE(CListCtrlEx)
// Construction
public:
	CListCtrlEx(IMethodUI *pMethod = NULL);
	virtual ~CListCtrlEx();

protected:
	int m_nSelectedItem;

	IMethodUI *m_pMethod;

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLargeIcon();
	afx_msg void OnSmallIcon();
	afx_msg void OnDeleteItem();
	afx_msg void OnTestAllCfg();
	afx_msg void OnOpenModel();
	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};