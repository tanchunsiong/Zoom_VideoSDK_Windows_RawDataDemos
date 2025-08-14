#ifndef _ENUM_WINDOW_INFO_MGR_H_
#define _ENUM_WINDOW_INFO_MGR_H_

#include "enum_apps_info_mgr_comm_def.h"

class EnumAppsInfoMgr
{
public:
	static EnumAppsInfoMgr& Instance();

	AppList GetDisplayScreenList();
	wstring GetDisplayScreenThumnailImage(AppInfo app, uint32_t thumbnail_width, uint32_t thumbnail_height);

	AppList GetAllAppsList();
	wstring GetAppThumbnailImage(AppInfo app,uint32_t thumbnail_width,uint32_t thumbnail_height);

	HBITMAP LoadBitmapFromResource(DWORD ResourceID, bool transparent = true);
	Gdiplus::Bitmap* ScaleBitmap(Gdiplus::Bitmap* pBitmap, UINT nWidth, UINT nHeight);
	wstring GetWindowPNGImage(HBITMAP hbitmap, int width, int height);

private:
	EnumAppsInfoMgr();
	~EnumAppsInfoMgr();

	HDC desktop_dc_;
};

#endif //_ENUM_WINDOW_INFO_MGR_H_