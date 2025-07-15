// SEOEMAddIn.h : main header file for the SEOEMAddIn DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "SampleAddin_i.h"

class SEAddin;


class CSampleAddinApp : public CWinApp
{
public:
	CSampleAddinApp();

// Overrides
public:
	virtual BOOL InitInstance();
	int ExitInstance();

public:
	static CSampleAddinApp* GetSEApp();
	SEAddin* GetSEAddin();
	void SetSEAddin(SEAddin* pAddin);

	static ApplicationPtr GetApplication();

private:
	SEAddin* m_pSEAddin;
};
