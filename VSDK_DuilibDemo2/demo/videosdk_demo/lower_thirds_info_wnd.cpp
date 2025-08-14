#include "stdafx.h"
#include "lower_thirds_info_wnd.h"
#include "main_frame.h"

#define LOWERTHIRDS_PADDING_LEFT 8
#define LOWERTHIRDS_PADDING_TOP 42

CLowerThirdsInfoWnd::CLowerThirdsInfoWnd()
{
}

CLowerThirdsInfoWnd::~CLowerThirdsInfoWnd()
{
}


void CLowerThirdsInfoWnd::InitWindow()
{
	InitControl();
	InitColorInfo();
}

void CLowerThirdsInfoWnd::InitControl()
{
	label_thirds_name_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_thirds_name")));
	label_thirds_title_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_thirds_title")));	
	control_left_line_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("control_left_line")));
	control_right_corner_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("control_right_corner")));
}

void CLowerThirdsInfoWnd::InitColorInfo()
{
	LowerThirdColorInfo colorInfo;

	//none
	colorInfo.color = LowerThirdColor::COLOR_NONE;
	colorInfo.bkcolor = _T("#444B53");
	colorInfo.textcolor = 0XF7F9FA;
	colorInfo.color_normal_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,0,2,18'");
	colorInfo.color_triangle_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,0,11,18'");
	colorInfo.gdi_font_color = Gdiplus::Color(245, 245, 245);
	colorInfo.gdi_font_bk_color = { 68, 75, 83};
	lower_third_color_info_map.insert(make_pair(_T("#444B53"), colorInfo));

	//bule
	colorInfo.color = LowerThirdColor::COLOR_BULE;
	colorInfo.bkcolor = _T("#1E71D6");
	colorInfo.textcolor = 0XF7F9FA;
	colorInfo.color_normal_image = _T("res='BUTTON_COLOR_COMMON' restype='ZPIMGRES' source='0,18,2,36'");
	colorInfo.color_triangle_image = _T("res = 'BUTTON_COLOR_COMMON' restype ='ZPIMGRES' source='0,18,11,36'");	
	colorInfo.gdi_font_color = Gdiplus::Color(245, 245, 245);
	colorInfo.gdi_font_bk_color = { 30, 113, 214};
	lower_third_color_info_map.insert(make_pair(_T("#1E71D6"), colorInfo));

	//red
	colorInfo.color = LowerThirdColor::COLOR_VIOLET;
	colorInfo.bkcolor = _T("#FD3D4A");
	colorInfo.textcolor = 0XF7F9FA;
	colorInfo.color_normal_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,72,2,90'");
	colorInfo.color_triangle_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,72,11,90'");
	colorInfo.gdi_font_color = Gdiplus::Color(245, 245, 245);
	colorInfo.gdi_font_bk_color = { 253, 61, 74};
	lower_third_color_info_map.insert(make_pair(_T("#FD3D4A"), colorInfo));

	//green
	colorInfo.color = LowerThirdColor::COLOR_GREEN;
	colorInfo.bkcolor = _T("#66CC84");
	colorInfo.textcolor = 0X131619;
	colorInfo.color_normal_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,54,2,72'");
	colorInfo.color_triangle_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,54,11,72'");
	colorInfo.gdi_font_color = Gdiplus::Color(245, 245, 245);
	colorInfo.gdi_font_bk_color = { 102, 204, 132 };
	lower_third_color_info_map.insert(make_pair(_T("#66CC84"), colorInfo));

	//orange
	colorInfo.color = LowerThirdColor::COLOR_ORANGE;
	colorInfo.bkcolor = _T("#FF8422");
	colorInfo.textcolor = 0X131619;
	colorInfo.color_normal_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,126,2,144'");
	colorInfo.color_triangle_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,126,11,144'");
	colorInfo.gdi_font_color = Gdiplus::Color(19, 22, 25);
	colorInfo.gdi_font_bk_color = { 255, 132, 34 };
	lower_third_color_info_map.insert(make_pair(_T("#FF8422"), colorInfo));

	//darkblue
	colorInfo.color = LowerThirdColor::COLOR_RED;
	colorInfo.bkcolor = _T("#493AB7");
	colorInfo.textcolor = 0XF7F9FA;
	colorInfo.color_normal_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,36,2,54'");
	colorInfo.color_triangle_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,36,11,54'");
	colorInfo.gdi_font_color = Gdiplus::Color(245, 245, 245);
	colorInfo.gdi_font_bk_color = { 73, 58, 183 };
	lower_third_color_info_map.insert(make_pair(_T("#493AB7"), colorInfo));

	//violet
	colorInfo.color = LowerThirdColor::COLOR_DARKBULE;
	colorInfo.bkcolor = _T("#A477FF");
	colorInfo.textcolor = 0XF7F9FA;
	colorInfo.color_normal_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,108,2,126'");
	colorInfo.color_triangle_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,108,11,126'");
	colorInfo.gdi_font_color = Gdiplus::Color(245, 245, 245);
	colorInfo.gdi_font_bk_color = { 164, 119, 255 };
	lower_third_color_info_map.insert(make_pair(_T("#A477FF"), colorInfo));

	//yellow
	colorInfo.color = LowerThirdColor::COLOR_YELLOW;
	colorInfo.bkcolor = _T("#FFBF39");
	colorInfo.textcolor = 0X131619;
	colorInfo.color_normal_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,90,2,108'");
	colorInfo.color_triangle_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,90,11,108'");
	colorInfo.gdi_font_color = Gdiplus::Color(9, 22, 25);
	colorInfo.gdi_font_bk_color = { 255, 191, 57 };
	lower_third_color_info_map.insert(make_pair(_T("#FFBF39"), colorInfo));
}

LowerThirdColorInfo CLowerThirdsInfoWnd::GetLowerThirdsColorInfo(const std::wstring& colorKey)
{
	std::wstring key = colorKey;
	if (key.empty() || colorKey == _T("null"))
		key = _T("#444B53");
	
	LowerThirdColorInfo colorInfo;
	if (lower_third_color_info_map.count(key) > 0)
		return lower_third_color_info_map[colorKey];
	else
		return colorInfo;
}

void CLowerThirdsInfoWnd::SetLowerThirdsInfo(CDuiString thirdsName, CDuiString thirdsDescription, CDuiString thirdsRgb)
{
	if (!label_thirds_name_ || !label_thirds_title_ || !control_right_corner_ || !control_left_line_)
		return;

	if (thirdsName.IsEmpty())
		return;

	LowerThirdColorInfo colorInfo = GetLowerThirdsColorInfo(thirdsRgb.GetData());
	
	label_thirds_name_->SetText(thirdsName.GetData());
	control_left_line_->SetBkImage(colorInfo.color_normal_image.data());

	if (thirdsDescription.IsEmpty())
	{
		control_left_line_->SetMaxHeight(19);
		label_thirds_title_->SetVisible(false);
		control_right_corner_->SetVisible(false);
	}
	else
	{
		control_left_line_->SetMaxHeight(38);
		label_thirds_title_->SetVisible(true);
		control_right_corner_->SetVisible(true);		
		label_thirds_title_->SetBkImage(colorInfo.color_normal_image.data());
		label_thirds_title_->SetText(thirdsDescription.GetData());
		label_thirds_title_->SetTextColor(colorInfo.textcolor);
		control_right_corner_->SetBkImage(colorInfo.color_triangle_image.data());
	}
}

bool CLowerThirdsInfoWnd::CheckLowerThirdEnable()
{
	std::wstring file_name = CMainFrame::GetInstance().GetLowerThirdConfPath();

	TCHAR tempStr[MAX_PATH] = { 0 };
	GetPrivateProfileString(_T("LOWERTHIRD"), _T("Enable"), NULL, tempStr, sizeof(tempStr), file_name.data());
	std::wstring bEnable = tempStr;
	return (bEnable == _T("1"));
}

void CLowerThirdsInfoWnd::OnSessionJoin()
{
	InitCurrentUserLowerThirdInfo();
}

void CLowerThirdsInfoWnd::InitCurrentUserLowerThirdInfo()
{
	std::wstring username, description, color, isenable;
	CMainFrame::GetInstance().GetLowerThirdInfoFromIniFile(username, description, color, isenable);

	if (username.empty())
	{
		POINT emoji_middle_point = { LOWERTHIRDS_PADDING_LEFT, LOWERTHIRDS_PADDING_TOP };
		CMainFrame::GetInstance().SetActivityVideoEmojiPos(emoji_middle_point);
		return;
	}
	
	bool isshow = (isenable == _T("1"));
	if (isshow)
	{
		CMainFrame::GetInstance().ShowLowerThirdsInfoWnd(true);
		SetLowerThirdsInfo(username.data(), description.data(), color.data());
	}
	else
	{
		POINT emoji_middle_point = { LOWERTHIRDS_PADDING_LEFT, LOWERTHIRDS_PADDING_TOP};
		CMainFrame::GetInstance().SetActivityVideoEmojiPos(emoji_middle_point);
	}
}

LowerThirdInfo CLowerThirdsInfoWnd::GetUserLowerThirdInfo(IZoomVideoSDKUser* pUser)
{
	LowerThirdInfo lower_thirs_info;

	if (!pUser)
		return lower_thirs_info;

	if (pUser == ZoomVideoSDKMgr::GetInst().GetMySelf())
	{
		std::wstring username, description, color, isenable;
		CMainFrame::GetInstance().GetLowerThirdInfoFromIniFile(username, description, color, isenable);
		lower_thirs_info.lower_third_name = username;
		lower_thirs_info.lower_third_description = description;
		lower_thirs_info.lower_third_color = color;
	}
	else
	{
		std::wstring userId = pUser->getUserID();
		if (lower_third_info_map.count(userId) > 0)
			lower_thirs_info = lower_third_info_map[userId];
	}
	
	return lower_thirs_info;	
}

void CLowerThirdsInfoWnd::OnUserLeave(IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (!userList) return;

	std::wstring curSelectUserId = _T("");
	IZoomVideoSDKUser* pUser = CMainFrame::GetInstance().GetGalleryContainerSelectUser();
	if (pUser)
		curSelectUserId = pUser->getUserID();
	
	for (int i = 0; i < userList->GetCount(); i++)
	{
		IZoomVideoSDKUser* leave_user = userList->GetItem(i);
		if (!leave_user) continue;

		std::wstring userId = leave_user->getUserID();
		if (curSelectUserId == userId)
			InitCurrentUserLowerThirdInfo();
		
		auto iter = lower_third_info_map.find(userId);
		if (iter != lower_third_info_map.end())
			lower_third_info_map.erase(iter);
	}
}

void CLowerThirdsInfoWnd::CacheUserLowerThirdInfo(IZoomVideoSDKUser* pUser, const std::wstring& lower_third_name, const std::wstring& lower_third_description, const std::wstring& lower_third_color)
{
	if (!pUser)
		return;

	std::wstring userId = pUser->getUserID();
	LowerThirdInfo lower_thirs_info;
	lower_thirs_info.lower_third_name = lower_third_name;
	lower_thirs_info.lower_third_description = lower_third_description;
	lower_thirs_info.lower_third_color = lower_third_color;

	lower_third_info_map[userId] = lower_thirs_info;		
}

void CLowerThirdsInfoWnd::OnParseChannelCmd(IZoomVideoSDKUser* pSender, const std::vector<std::wstring >& cmdVector)
{
	if (cmdVector.size() < 4)
		return;

	std::wstring strName = cmdVector[1];
	std::wstring strCompanyName = cmdVector[2];
	std::wstring strRgb = cmdVector[3];
	
	LowerThirdColorInfo colorInfo = GetLowerThirdsColorInfo(strRgb);
	
	CacheUserLowerThirdInfo(pSender, strName, strCompanyName, strRgb);

	if (CheckLowerThirdEnable() && ZoomVideoSDKMgr::GetInst().IsInSession()) {
		CMainFrame::GetInstance().ShowLowerThirdsInfoWnd(true);
	}

	CMainFrame::GetInstance().UpdateRenderModeLowerThird(pSender, strName, strCompanyName, colorInfo);

	IZoomVideoSDKUser* pCurrentSelectUser = CMainFrame::GetInstance().GetGalleryContainerSelectUser();
	if (!pCurrentSelectUser || !pSender)
		return;

	if (pSender->getUserID() == pCurrentSelectUser->getUserID())		
		CMainFrame::GetInstance().SetLowerThirdsInfo(strName.data(), strCompanyName.data(), strRgb.data());
}