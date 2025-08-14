#pragma once

class CCameraControlRequestReceivedWndUI : public WindowImplBase
{
public:
	CCameraControlRequestReceivedWndUI();
	~CCameraControlRequestReceivedWndUI();

	void SetCameraControlRequestHandler(IZoomVideoSDKCameraControlRequestHandler* handler);
	IZoomVideoSDKUser* GetRequester();
	void SetRequester(IZoomVideoSDKUser* pUser);
	void OnUserLeave(IVideoSDKVector<IZoomVideoSDKUser*>* userList);

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("VideoSDKDemoCameraControlRequestReceivedUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);
	

private:
	void InitControls();
	void DoDeclineBtnClick();
	void DoApproveBtnClick();
	

private:
	CButtonUI* btn_decline_ = nullptr;
	CButtonUI* btn_approve_ = nullptr;
	CLabelUI* label_text_ = nullptr;
	IZoomVideoSDKCameraControlRequestHandler* m_pCameraControlRequestHandler;
	IZoomVideoSDKUser* m_pRequester;

};