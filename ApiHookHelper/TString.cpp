#include "stdafx.h"
#include "TString.h"
#include <clocale>
#include <cstdlib>

CTString::CTString(WCHAR *str)
	: WSTRING(str)
{
	m_pANSIChar = NULL;
}

CTString::CTString(char* str)
	: WSTRING()
{
	size_t szStr = strnlen_s(str, 65535);
	WCHAR* pTemp = new WCHAR[szStr + 1];
	size_t szConverted = 0;
	mbstowcs_s(&szConverted, pTemp, szStr + 1, str, _TRUNCATE);
	(*this) += pTemp;
	delete[]pTemp;
	m_pANSIChar = NULL;
}

CTString::CTString()
	: WSTRING()
{
	m_pANSIChar = NULL;
}
CTString::~CTString()
{
	if (m_pANSIChar != NULL)
	{
		delete m_pANSIChar;
	}
}

void CTString::SetLocale(int _Category, _In_opt_z_ char const* _Locale)
{
	std::setlocale(_Category, _Locale);
}
const char* CTString::ToAString()
{
	size_t szCurrentString = this->length();
	this->m_pANSIChar = new char[szCurrentString + 1];
	size_t szConverted;

	wcstombs_s(&szConverted, m_pANSIChar, szCurrentString + 1,
		this->c_str(), _TRUNCATE);

	return m_pANSIChar;
}

const WCHAR* CTString::ToWString()
{
	return this->c_str();
}

const TCHAR* CTString::ToTString()
{
#ifdef UNICODE
	return ToWString();
#else
	return ToASTring();
#endif
}