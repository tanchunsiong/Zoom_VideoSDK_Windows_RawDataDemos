#ifndef _ENUM_WINDOW_INFO_MGR_CMM_DEF_H_
#define _ENUM_WINDOW_INFO_MGR_CMM_DEF_H_

typedef int32_t AppId;

enum ShareType
{
	kShareTypeNone = 0,
	kScreenShare,
	kWindowShare,
	kSecondCameraShare,
};

struct AppInfo
{
	AppId id;
	RECT rect;
	wstring device;
	wstring title;
	ShareType share_type;
	AppInfo()
	{
		id = -1;
		rect = { 0 };
		share_type = kShareTypeNone;
	}
};

typedef std::vector<AppInfo> AppList;

#endif //_ENUM_WINDOW_INFO_MGR_CMM_DEF_H_