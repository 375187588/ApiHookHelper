#pragma once
#include <string>

#ifdef APIHOOKHELPER_EXPORTS
#define APIHOOKHELPER_API __declspec(dllexport)
#else
#define APIHOOKHELPER_API __declspec(dllimport)
#endif

typedef APIHOOKHELPER_API std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > wstring;
typedef APIHOOKHELPER_API wstring WSTRING;

class APIHOOKHELPER_API CTString : public WSTRING
{
private:
	char* m_pANSIChar;
public:
	CTString();
	CTString(WCHAR * str);
	CTString(char* str);
	~CTString();

	static void SetLocale(int _Category, _In_opt_z_ char const* _Locale);
	const char* ToAString();
	const WCHAR* ToWString();
	const TCHAR* ToTString();
private:

};

