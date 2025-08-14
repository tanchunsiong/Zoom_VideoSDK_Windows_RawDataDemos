#pragma once

#include <Windows.h>
#include <sstream>

enum WindowsMajorVersions 
{
	kWindows2000 = 5,
	kWindowsVista = 6,
	kWindows10 = 10,
};

inline bool GetOsVersion(int* major, int* minor, int* build)
{
	OSVERSIONINFO info = { 0 };
	info.dwOSVersionInfoSize = sizeof(info);
	if (GetVersionEx(&info)) {
		if (major)
			*major = info.dwMajorVersion;
		if (minor)
			*minor = info.dwMinorVersion;
		if (build)
			*build = info.dwBuildNumber;
		return true;
	}
	return false;
}

inline bool IsWindowsVistaOrLater() 
{
	int major;
	return (GetOsVersion(&major, nullptr, nullptr) && major >= kWindowsVista);
}

inline bool IsWindowsXpOrLater() 
{
	int major, minor;
	return (GetOsVersion(&major, &minor, nullptr) &&
		(major >= kWindowsVista || (major == kWindows2000 && minor >= 1)));
}

inline bool IsWindows8OrLater() 
{
	int major, minor;
	return (GetOsVersion(&major, &minor, nullptr) &&
		(major > kWindowsVista || (major == kWindowsVista && minor >= 2)));
}

inline bool IsWindows10OrLater() 
{
	int major;
	return (GetOsVersion(&major, nullptr, nullptr) && (major >= kWindows10));
}

inline void split(const std::wstring& s, TCHAR delim, std::vector<std::wstring>& elems) {
	std::wstringstream ss;
	ss.str(s);
	std::wstring item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}

inline std::vector<std::wstring> split(const std::wstring& s, TCHAR delim) {
	std::vector<std::wstring> elems;
	split(s, delim, elems);
	return elems;
}