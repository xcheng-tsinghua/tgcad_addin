#pragma once
#include <string>
#include <codecvt>
#include <atlstr.h>

using namespace std;

class NDSStringHelper
{

public:

	static string ToUpper(const string& str);
	static string ToLower(const string& str);

	static string ToString(const wstring& wstr);
	static string ToString(const CString& cstr);

	//Chinese string to UTF-8
	static string ToString(const string& str);

	//Chinese string to UTF-8
	static string ToUTF8(const string& str);
	static string ToUTF8(const wstring& wstr);
	static string ToUTF8(const CString& cstr);

	static string FromUTF8(const string& str);

	static wstring ToWString(const string& str, bool isUTF8 = false);
	static wstring ToWString(const CString& cstr);

	static CString ToCString(const string& str);
	static CString ToCString(const wstring& wstr);

	static void LTrim(string& str);
	static void RTrim(string& str);
	static void Trim(string& str);

	static string GetTableString(UINT id);
	static string Guid();
	static string GetCurrentFolderPath();

	static string ToBase64(const string& str);
	static string FromBase64(const string& str);

	static string FindMaxSubString(string a, string b);
};

