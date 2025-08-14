#ifndef _BULLET_SCREEN_WND_H_
#define _BULLET_SCREEN_WND_H_

#include "videosdk_demo_mgr.h"

#define WM_ADD_ROLLCONTROL (WM_USER + 1024) //

class CRollControlUI;

class CBulletScreenWnd : public WindowImplBase, public IParseChannelCmdWnd
{
public:
	CBulletScreenWnd();
	~CBulletScreenWnd();

	void	RemoveAllRoolControl();
	virtual void OnParseChannelCmd(IZoomVideoSDKUser* pSender, const std::vector<std::wstring >& cmdVector);
	virtual HWND GetHWND() { return m_hWnd; }
protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder() { return _T(""); }
	virtual CDuiString GetSkinFile() { return to_wstring(IDXML_BULLET_SCREEN_WND_UI).c_str(); }
	virtual UILIB_RESOURCETYPE GetResourceType() const { return UILIB_RESOURCE; };
	virtual LPCTSTR GetWindowClassName() const { return _T("BulletScreenWnd"); }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

private:
	void		InitControl();
	void		InitEmojiImageMap();
	void		AddRoolControl(const std::wstring& buf);
	
	CDuiString	GetRandomPos();
private:
	CVerticalLayoutUI* m_pBulletScreenLayout;
	map<std::wstring, std::wstring> m_emojiImageMap;
};

#endif