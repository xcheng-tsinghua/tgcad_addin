#include "stdafx.h"
#include "Options.h"


IMPLEMENT_DYNAMIC(Options, CDialog)

Options::Options(CWnd* pParent /*=NULL*/)
	: CDialog(Options::IDD, pParent)
{

}

Options::~Options()
{
}

void Options::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Options, CDialog)
	ON_BN_CLICKED(IDOK, &Options::OnBnClickedOk)
END_MESSAGE_MAP()


// Options message handlers

void Options::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	if( pEdit )
	{
		pEdit->GetWindowText( m_strDoit );
	}
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	if( pEdit )
	{
		pEdit->GetWindowText( m_strOptions );
	}

	OnOK();
}
