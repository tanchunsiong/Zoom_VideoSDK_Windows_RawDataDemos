#include "stdafx.h"
#include "app_utils.h"

#include <tchar.h>
#include <shlobj.h>
#include <windows.h>
#include <wrl/client.h>
#include <vector>
#include <fstream>
// Just for the DWMWINDOWATTRIBUTE enums (DWMWA_CLOAKED).
#include <dwmapi.h>
#include <iostream>
#include <WinGDI.h>
#include <GdiPlus.h>
#include <Shlwapi.h> 
#include <combaseapi.h>
#include <WinBase.h>

#include "utils.h"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "Magnification.lib")

BOOL CALLBACK GetAppListHandler(HWND hwnd, LPARAM param) {
	AppList* app_list = reinterpret_cast<AppList*>(param);
	if (!IsWindowVisible(hwnd) || IsIconic(hwnd)) 
	{
		return TRUE;
	}
	HWND owner = GetWindow(hwnd, GW_OWNER);
	LONG exstyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	if (owner && !(exstyle & WS_EX_APPWINDOW)) 
	{
		return TRUE;
	}
	if (!IsWindowResponding(hwnd)) 
	{
		return TRUE;
	}
	AppInfo app;
	app.share_type = kWindowShare;
	app.id = reinterpret_cast<AppId>(hwnd);
	if (CanSafelyMakeBlockingCalls(hwnd)) 
	{
		const size_t kTitleLength = 500;
		WCHAR window_title[kTitleLength] = L"";
		if (GetWindowTextLength(hwnd) != 0 &&
			GetWindowTextW(hwnd, window_title, kTitleLength) > 0) 
		{
			app.title = window_title;
		}
	}
	if (app.title.empty()) {
		return TRUE;
	}

	const size_t kMaxClassNameLength = 256;
	WCHAR class_name[kMaxClassNameLength] = L"";
	const int class_name_length =
		GetClassNameW(hwnd, class_name, kMaxClassNameLength);
	if (class_name_length < 1)
		return TRUE;

	// Skip Program Manager window.
	if (wcscmp(class_name, L"Progman") == 0) {
		return TRUE;
	}

	if (wcscmp(class_name, L"Button") == 0) {
		return TRUE;
	}
	app_list->push_back(app);
	return TRUE;
}

AppList GetAppList()
{
	AppList app_list;
	EnumWindows(&GetAppListHandler, reinterpret_cast<LPARAM>(&app_list));
	for (auto iter = app_list.begin(); iter != app_list.end();)
	{
		RECT rect;
		GetWindowRect(reinterpret_cast<HWND>(iter->id), &rect);
		iter->rect.left = rect.left;
		iter->rect.right = rect.right;
		iter->rect.top = rect.top;
		iter->rect.bottom = rect.bottom;
		if (!IsWindowVisibleOnCurrentDesktop(reinterpret_cast<HWND>(iter->id)))
		{
			iter = app_list.erase(iter);
		}
		else 
		{
			++iter;
		}
	}
	return app_list;
}

bool IsWindowValidAndVisible(HWND window) 
{
	return IsWindow(window) && IsWindowVisible(window) && !IsIconic(window);
}

bool IsWindowResponding(HWND window) 
{
	const UINT uTimeoutMs = 50;
	return SendMessageTimeout(window, WM_NULL, 0, 0, SMTO_ABORTIFHUNG, uTimeoutMs, nullptr);
}

bool CanSafelyMakeBlockingCalls(HWND hwnd) 
{
	DWORD process_id;
	GetWindowThreadProcessId(hwnd, &process_id);
	if (process_id != GetCurrentProcessId() || IsWindowResponding(hwnd)) 
	{
		return true;
	}
	return false;
}

bool IsWindowVisibleOnCurrentDesktop(HWND hwnd) 
{
	return IsWindowValidAndVisible(hwnd) && IsWindowOnCurrentDesktop(hwnd) && !IsWindowCloaked(hwnd);
}

bool IsWindowOnCurrentDesktop(HWND hwnd) 
{
	BOOL on_current_desktop = TRUE;
	Microsoft::WRL::ComPtr<IVirtualDesktopManager> virtual_desktop_manager;
	if (IsWindows10OrLater()) 
	{
		HRESULT hr = ::CoCreateInstance(__uuidof(VirtualDesktopManager), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&virtual_desktop_manager));
		if (FAILED(hr)) {
			return on_current_desktop;
		}
		virtual_desktop_manager->IsWindowOnCurrentVirtualDesktop(hwnd, &on_current_desktop);
	}
	return on_current_desktop;
}

bool IsWindowCloaked(HWND hwnd) 
{
	int res = 0;
	DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &res, sizeof(res));
	return res != 0;
}