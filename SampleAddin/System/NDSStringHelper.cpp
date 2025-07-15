#include "stdafx.h"
#include "NDSStringHelper.h"
#include <algorithm>
#include <afxpriv.h>
#include <atlenc.h>

string NDSStringHelper::ToUpper(const string& str)
{
	string trans(str);
	transform(trans.begin(), trans.end(), trans.begin(), ::toupper);

	return trans;
}

string NDSStringHelper::ToLower(const string& str)
{
	string trans(str);
	transform(trans.begin(), trans.end(), trans.begin(), ::tolower);

	return trans;
}

string NDSStringHelper::ToString(const wstring& wstr)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (nLen <= 0) return std::string("");
	char* pszDst = new char[nLen];
	if (nullptr == pszDst) return std::string("");
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pszDst, nLen, nullptr, nullptr);
	pszDst[nLen - 1] = 0;
	std::string strTemp(pszDst);
	delete[] pszDst;

	return strTemp;

	//std::locale sys_locale("");

	//const wchar_t* data_from = wstr.c_str();
	//const wchar_t* data_from_end = wstr.c_str() + wstr.size();
	//const wchar_t* data_from_next = 0;

	//int wchar_size = 4;
	//char* data_to = new char[(wstr.size() + 1) * wchar_size];
	//char* data_to_end = data_to + (wstr.size() + 1) * wchar_size;
	//char* data_to_next = 0;

	//memset(data_to, 0, (wstr.size() + 1) * wchar_size);

	//typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
	//mbstate_t out_state = 0;
	//auto result = std::use_facet<convert_facet>(sys_locale).out(
	//	out_state, data_from, data_from_end, data_from_next,
	//	data_to, data_to_end, data_to_next);
	//if (result == convert_facet::ok)
	//{
	//	std::string dst = data_to;
	//	delete[] data_to;
	//	return dst;
	//}
	//else
	//{
	//	printf("convert error!\n");
	//	delete[] data_to;
	//	return std::string("");
	//}
}

string NDSStringHelper::ToString(const CString& cstr)
{
	wstring wstr = cstr.GetString();
	return ToString(wstr);
}

string NDSStringHelper::ToString(const string& str)
{
	return ToUTF8(str);
}

string NDSStringHelper::ToUTF8(const string& str)
{
	wstring wstr = ToWString(str);
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(wstr);
}

string NDSStringHelper::ToUTF8(const wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(wstr);
}

string NDSStringHelper::ToUTF8(const CString& cstr)
{
	wstring wstr = ToWString(cstr);
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(wstr);
}

string NDSStringHelper::FromUTF8(const string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	wstring wstr = conv.from_bytes(str);

	return ToString(wstr);
}

wstring NDSStringHelper::ToWString(const string& str, bool isUTF8 /* = false */)
{
	if (isUTF8)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
		return conv.from_bytes(str);
	}

	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), NULL, 0);
	wchar_t *buffer = new wchar_t[len + 1];
	memset(buffer, 0, sizeof(wchar_t)*(len + 1));
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), buffer, len);
	wstring return_value;
	return_value.append(buffer);
	delete[]buffer;
	return return_value;

	//std::locale sys_locale("");

	//const char* data_from = str.c_str();
	//const char* data_from_end = str.c_str() + str.size();
	//const char* data_from_next = 0;

	//wchar_t* data_to = new wchar_t[str.size() + 1];
	//wchar_t* data_to_end = data_to + str.size() + 1;
	//wchar_t* data_to_next = 0;

	//wmemset(data_to, 0, str.size() + 1);

	//typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
	//mbstate_t in_state = 0;
	//auto result = std::use_facet<convert_facet>(sys_locale).in(
	//	in_state, data_from, data_from_end, data_from_next,
	//	data_to, data_to_end, data_to_next);
	//if (result == convert_facet::ok)
	//{
	//	std::wstring dst = data_to;
	//	delete[] data_to;
	//	return dst;
	//}
	//else
	//{
	//	printf("convert error!\n");
	//	delete[] data_to;
	//	return std::wstring(L"");
	//}
}

wstring NDSStringHelper::ToWString(const CString& cstr)
{
	return cstr.GetString();
}

CString NDSStringHelper::ToCString(const string& str)
{
	return CString(str.c_str());
}

CString NDSStringHelper::ToCString(const wstring& wstr)
{
	return CString(wstr.c_str());
}

void NDSStringHelper::LTrim(string& str)
{
	if (!str.empty())
	{
		str.erase(0, str.find_first_not_of(" "));
	}
}

void NDSStringHelper::RTrim(string& str)
{
	if (!str.empty())
	{
		str.erase(str.find_last_not_of(" ") + 1);
	}
}

void NDSStringHelper::Trim(string& str)
{
	if (!str.empty())
	{
		str.erase(0, str.find_first_not_of(" "));
		str.erase(str.find_last_not_of(" ") + 1);
	}
}

string NDSStringHelper::GetTableString(UINT id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	wchar_t buff[256];
	::AfxLoadString(id, buff);
	wstring wstr(buff);

	return ToUTF8(wstr);
}

string NDSStringHelper::Guid()
{
	char buf[64] = { 0 };
	GUID guid;
	::CoCreateGuid(&guid);

	_snprintf_s(
		buf,
		sizeof(buf),
		"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	return string(buf);
}

string NDSStringHelper::GetCurrentFolderPath()
{
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(AfxGetStaticModuleState()->m_hCurrentInstanceHandle, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	CString csPath = szFilePath;

	return ToString(csPath);
}


string NDSStringHelper::ToBase64(const string& str)
{
	std::string base64Password;
	if (str.empty())
		return base64Password;
	
	int iSrcLent = (int)str.length();

	int ilen = Base64EncodeGetRequiredLength(iSrcLent);
	char* pDest = new char[ilen + 1];
	memset(pDest, 0, ilen + 1);
	Base64Encode((const BYTE*)str.c_str(), iSrcLent, pDest, &ilen);
	base64Password = pDest;

	return base64Password;
}

string NDSStringHelper::FromBase64(const string& str)
{
	std::string base64Decode;
	if (str.empty())
		return base64Decode;

	int iSrcLent = (int)str.length();

	int ilen = Base64DecodeGetRequiredLength(iSrcLent);
	char* pDest = new char[ilen + 1];
	memset(pDest, 0, ilen + 1);
	Base64Decode(str.c_str(), iSrcLent, (BYTE*)pDest, &ilen);
	base64Decode = pDest;

	return base64Decode;
}

string NDSStringHelper::FindMaxSubString(string a, string b)
{
	string res = "";
	string shorter = a.length() > b.length() ? b : a;
	string longer = a.length() > b.length() ? a : b;

	int maxlen = 0;
	int maxpos = -1;
	for (int i = 0; i < shorter.length(); i++) {
		for (int j = 0; j < longer.length(); j++)
		{
			if (shorter[i] == longer[j])
			{
				int k = 1;
				for (; (shorter[i + k] == longer[j + k]) && i + k < shorter.length(); k++);
				if (k > maxlen) {
					maxpos = i;
					maxlen = k;
				}
			}
		}
	}

	if (maxpos == -1)
		return res;
	else
	{
		res = shorter.substr(maxpos, maxlen);
		return res;
	}

}