#ifndef _SWITCH_CAMERA_POP_WND_UI_H_
#define _SWITCH_CAMERA_POP_WND_UI_H_

class CSwitchCameraPopWnd : public WindowImplBase
{
public:
	CSwitchCameraPopWnd();
	~CSwitchCameraPopWnd();
	void SetBtnText(int index, std::wstring deviceName, std::wstring deviceID);

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder() { return _T(""); }
	virtual CDuiString GetSkinFile() { return to_wstring(IDXML_SWITCH_CAMERA_POP_WND_UI).c_str(); }
	virtual UILIB_RESOURCETYPE GetResourceType() const { return UILIB_RESOURCE; };
	virtual LPCTSTR GetWindowClassName() const { return _T("SwitchCameraPopWnd"); }
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	
private:
	void InitControl();	

private:
	CButtonUI* btn_camera_1_ = nullptr;
	CButtonUI* btn_camera_2_ = nullptr;
	CButtonUI* btn_camera_3_ = nullptr;
	CButtonUI* btn_camera_4_ = nullptr;
	CButtonUI* btn_camera_5_ = nullptr;
	CButtonUI* btn_camera_6_ = nullptr;
	std::map<int, std::wstring> map_cameras_;
};

#endif