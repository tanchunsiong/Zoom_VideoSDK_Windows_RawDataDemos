#ifndef _CONTROL_CAMERA_POP_WND_UI_H_
#define _CONTROL_CAMERA_POP_WND_UI_H_

class CControlCameraPopWnd : public WindowImplBase
{
public:
	CControlCameraPopWnd();
	~CControlCameraPopWnd();
	void SetText(std::wstring text);
	int GetWndWidth();
protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder() { return _T(""); }
	virtual CDuiString GetSkinFile() { return to_wstring(IDXML_CONTROL_CAMERA_POP_WND_UI).c_str(); }
	virtual UILIB_RESOURCETYPE GetResourceType() const { return UILIB_RESOURCE; };
	virtual LPCTSTR GetWindowClassName() const { return _T("ControlCameraPopWnd"); }
	
private:
	void InitControl();	

private:
	CLabelUI* label_text_ = nullptr;
};

#endif