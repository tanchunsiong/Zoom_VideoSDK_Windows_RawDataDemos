#include "stdafx.h"
#include "enum_apps_info_mgr.h"
#include "app_utils.h"
#include "utils.h"

BOOL CALLBACK EnumMonitorsCallback(HMONITOR handle, HDC hDC, LPRECT lpRect, LPARAM lParam)
{
	std::vector<MONITORINFOEX>* monitor_list = (std::vector<MONITORINFOEX>*)lParam;
	MONITORINFOEX s;
	ZeroMemory(&s, sizeof(s));    
	s.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(handle, &s);
	monitor_list->push_back(s);
	return TRUE;
}

EnumAppsInfoMgr::EnumAppsInfoMgr()
{
	desktop_dc_ = CreateCompatibleDC(nullptr);
}

EnumAppsInfoMgr::~EnumAppsInfoMgr()
{
}

EnumAppsInfoMgr& EnumAppsInfoMgr::Instance()
{
	static EnumAppsInfoMgr instance;
	return instance;
}

RECT GetScreenRect(AppId screen) {

	DISPLAY_DEVICEW device;
	device.cb = sizeof(device);
	BOOL result = EnumDisplayDevicesW(NULL, screen, &device, 0);


	DEVMODEW device_mode;
	device_mode.dmSize = sizeof(device_mode);
	device_mode.dmDriverExtra = 0;
	result = EnumDisplaySettingsExW(device.DeviceName, ENUM_CURRENT_SETTINGS, &device_mode, 0);

	int x = device_mode.dmPosition.x;
	int y = device_mode.dmPosition.y;
	int width = device_mode.dmPelsWidth;
	int height = device_mode.dmPelsHeight;

	return { x, y, x + width, y + height };
}

AppList EnumAppsInfoMgr::GetDisplayScreenList()
{
	AppList screen_list;
	std::vector<MONITORINFOEX> monitor_list;
	EnumDisplayMonitors(NULL, NULL, EnumMonitorsCallback, (LPARAM)&monitor_list);
	for (size_t i = 0; i < monitor_list.size(); ++i) {
		AppInfo screen_info;
		
		screen_info.share_type = kScreenShare;
		screen_info.device = monitor_list[i].szDevice;

		RECT rect = GetScreenRect(i);
		screen_info.id = i;
		screen_info.rect.left = rect.left;
		screen_info.rect.right = rect.right;
		screen_info.rect.top = rect.top;
		screen_info.rect.bottom = rect.bottom;
		screen_list.push_back(screen_info);
	}
	return screen_list;
}

Gdiplus::Bitmap* EnumAppsInfoMgr::ScaleBitmap(Gdiplus::Bitmap* pBitmap, UINT nWidth, UINT nHeight)
{
	Gdiplus::Bitmap* image = new Gdiplus::Bitmap(nWidth, nHeight, pBitmap->GetPixelFormat());
	if (image)
	{
		Gdiplus::Graphics* graphics = Gdiplus::Graphics::FromImage(image);
		if (graphics) 
		{
			// use the best interpolation mode
			graphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
			graphics->DrawImage(pBitmap, 0, 0, nWidth, nHeight);
			delete graphics;
		}
	}
	return image;
}

wstring EnumAppsInfoMgr::GetWindowPNGImage(HBITMAP hbitmap, int width, int height)
{
	ULONG_PTR m_token;
	Gdiplus::GdiplusStartupInput input;
	Gdiplus::GdiplusStartupOutput output;
	Gdiplus::Status status = GdiplusStartup(&m_token, &input, &output);

	Gdiplus::Bitmap* bitmap = NULL;
	bitmap = Gdiplus::Bitmap::FromHBITMAP(hbitmap, NULL);
	if (bitmap == nullptr) {
		DWORD error = GetLastError();
		std::cout << "bitmap is nullptr: " << error << std::endl;
		return _T("");
	}
	Gdiplus::Bitmap* scale_bitmap = ScaleBitmap(bitmap, width, height);
	if (bitmap) 
	{
		delete bitmap;
		bitmap = nullptr;
	}
	if (!scale_bitmap) 
	{
		return _T("");
	}
	IStream* stream = nullptr;
	HRESULT hRe = CreateStreamOnHGlobal(NULL, TRUE, &stream);
	CLSID clsidPNG;
	CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &clsidPNG);
	scale_bitmap->Save(stream, &clsidPNG);
	HGLOBAL hg = NULL;
	GetHGlobalFromStream(stream, &hg);
	std::vector<TCHAR> data;
	SIZE_T size = GlobalSize(hg);
	data.resize(size);
	LPVOID ps = GlobalLock(hg);
	memcpy(&data[0], ps, size);   
	GlobalUnlock(hg);
	wstring thumbnail_image(data.begin(), data.end());
	if (scale_bitmap) 
	{
		delete scale_bitmap;
		scale_bitmap = nullptr;
	}
	Gdiplus::GdiplusShutdown(m_token);
	return thumbnail_image;
}

HBITMAP EnumAppsInfoMgr::LoadBitmapFromResource(DWORD ResourceID, bool transparent)
{
	HMODULE hModule = GetModuleHandle(0);
	HRSRC hRsrc = ::FindResource(hModule, MAKEINTRESOURCE(ResourceID), _T("PNG")); // type 
	if (!hRsrc)
		return nullptr;
	// load resource into memory 
	DWORD len = SizeofResource(hModule, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hModule, hRsrc);
	if (!lpRsrc)
		return nullptr;
	// Allocate global memory on which to create stream 
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, lpRsrc, len);           
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	// load from stream 
	Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromStream(pstm);
	// free/release stuff 
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);

	HBITMAP hBitmap = NULL;
	Color clr(0, 0, 0, 0);
	image->GetHBITMAP(clr, &hBitmap);
	return hBitmap;
}

wstring EnumAppsInfoMgr::GetDisplayScreenThumnailImage(AppInfo app, uint32_t thumbnail_width, uint32_t thumbnail_height)
{
	int32_t width = app.rect.right - app.rect.left;
	int32_t height = app.rect.bottom - app.rect.top;

	BITMAPINFO bi = { 0 };
	BITMAPINFOHEADER* bih = &bi.bmiHeader;
	bih->biSize = sizeof(BITMAPINFOHEADER);
	bih->biBitCount = 32;
	bih->biWidth = width;
	bih->biHeight = -height;
	bih->biPlanes = 1;

	// HDC dest_hdc = CreateCompatibleDC(NULL);
	BYTE* bits = nullptr;
	HBITMAP hbitmap = CreateDIBSection(desktop_dc_, &bi, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
	if (!hbitmap || bits == nullptr) {
		std::cout << "CreateDIBSection Failed" << std::endl;
		return _T("");
	}
	HGDIOBJ gdiobj = SelectObject(desktop_dc_, hbitmap);
	HDC source_hdc = GetDC(NULL);
	BOOL ret = BitBlt(desktop_dc_, 0, 0, width, height, source_hdc, app.rect.left, app.rect.top, SRCCOPY);
	ReleaseDC(NULL, source_hdc);
	wstring thumbnail_image = GetWindowPNGImage(hbitmap, thumbnail_width, thumbnail_height);
	DeleteObject(hbitmap);
	return thumbnail_image;
}

AppList EnumAppsInfoMgr::GetAllAppsList()
{
	return GetAppList();
}

wstring EnumAppsInfoMgr::GetAppThumbnailImage(AppInfo app,uint32_t thumbnail_width, uint32_t thumbnail_height)
{
	HWND window = (HWND)app.id;
	int32_t width = app.rect.right - app.rect.left;
	int32_t height = app.rect.bottom - app.rect.top;

	BITMAPINFO bi = { 0 };
	BITMAPINFOHEADER* bih = &bi.bmiHeader;
	bih->biSize = sizeof(BITMAPINFOHEADER);
	bih->biBitCount = 32;
	bih->biWidth = width;
	bih->biHeight = -height;
	bih->biPlanes = 1;

	HDC source_hdc = GetDC(window);

	BYTE* bits = nullptr;
	HBITMAP hbitmap = CreateDIBSection(desktop_dc_, &bi, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
	if (!hbitmap || bits == nullptr) 
	{
		return _T("");
	}
	HGDIOBJ gdiobj = SelectObject(desktop_dc_, hbitmap);

	bool result = false;
	if (IsWindows8OrLater()) {
		const UINT flags = PW_RENDERFULLCONTENT;
		result = PrintWindow(window, desktop_dc_, flags);
		if (!result) {
			result = PrintWindow(window, desktop_dc_, 0);
		}
	}
	if (!result) {
		result = BitBlt(desktop_dc_, 0, 0, width, height, source_hdc, app.rect.left, app.rect.top, SRCCOPY);
	}

	ReleaseDC(NULL, source_hdc);

	wstring thumbnail_image = GetWindowPNGImage(hbitmap, thumbnail_width, thumbnail_height);
	DeleteObject(hbitmap);
	return thumbnail_image;
}