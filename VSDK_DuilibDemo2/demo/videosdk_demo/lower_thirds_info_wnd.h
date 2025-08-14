#ifndef _LOWER_THIRDS_INFO_WND_H_
#define _LOWER_THIRDS_INFO_WND_H_

#include "videosdk_demo_mgr.h"

class CLowerThirdsInfoWnd : public WindowImplBase, public IParseChannelCmdWnd
{
public:
	CLowerThirdsInfoWnd();
	~CLowerThirdsInfoWnd();

public:
	void	OnSessionJoin();
	virtual HWND GetHWND() { return m_hWnd; }	
	virtual void OnParseChannelCmd(IZoomVideoSDKUser* pSender, const std::vector<std::wstring >& cmdVector);	
	void	SetLowerThirdsInfo(CDuiString thirdsName, CDuiString thirdsDescription, CDuiString thirdsRgb);
	LowerThirdInfo	GetUserLowerThirdInfo(IZoomVideoSDKUser* pUser);
	LowerThirdColorInfo GetLowerThirdsColorInfo(const std::wstring& colorKey);
	void	OnUserLeave(IVideoSDKVector<IZoomVideoSDKUser*>* userList);	
protected:

	virtual void InitWindow();
	virtual CDuiString GetSkinFolder() { return _T(""); }
	virtual CDuiString GetSkinFile() { return to_wstring(IDXML_LOWERTHIRDS_INFO_WND_UI).c_str(); }
	virtual UILIB_RESOURCETYPE GetResourceType() const { return UILIB_RESOURCE; };
	virtual LPCTSTR GetWindowClassName() const { return _T("LowerThirdsInfoWnd"); }
	

private:
	void	InitColorInfo();
	void	InitControl();
	void	InitCurrentUserLowerThirdInfo();
	bool	CheckLowerThirdEnable();	
	void	CacheUserLowerThirdInfo(IZoomVideoSDKUser* pUser, const std::wstring& lower_third_name, const std::wstring& lower_third_description, const std::wstring& lower_third_color);
private:
	CLabelUI* label_thirds_name_ = nullptr;
	CLabelUI* label_thirds_title_ = nullptr;
	CControlUI* control_left_line_ = nullptr;
	CControlUI* control_right_corner_ = nullptr;
	map<std::wstring, LowerThirdColorInfo> lower_third_color_info_map;
	map<std::wstring, LowerThirdInfo> lower_third_info_map;
};

#endif