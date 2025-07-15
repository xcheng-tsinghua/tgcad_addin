#pragma once

class IModelInfoUI
{
public:
	virtual bool IsRootExists(const CString &strRoot) = 0;
};

class IMethodUI
{
public:
	virtual int DoAddRoot() = 0;
	virtual int DoAddModelData() = 0;
	virtual int DoDeleteRoot() = 0;
	virtual int DoDeleteModelData() = 0;
	virtual int DoDeleteListItem(const DWORD dwData, const CString &strItemName) = 0;
	virtual int DoDblclkListItem(const DWORD dwData) = 0;
	virtual int DoTestModelAllCfg(const DWORD dwData) = 0;
};