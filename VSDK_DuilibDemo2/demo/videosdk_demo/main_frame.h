#pragma once

#include "zoom_video_sdk_delegate_interface.h"
#include "zoom_video_sdk_chat_message_interface.h"

#include "video_show_mgr_comm_def.h"

#define MAIN_FRAME_TITLE _T("demo_main_window")
#define APP_NAME _T("video sdk demo")
#define POPUP_EXIST_APP 1

enum ReactionType;
enum CmdChannelType;
struct LowerThirdColorInfo;
struct LowerThirdInfo;

class CJoinSessionWndUI;
class CTitleBarWndUI;
class CBottomBarWndUI;
class CLeaveSessionTipsWndUI;
class CSessionInfoWndUI;
class CSessionSettingsWndUI;
class VideoShowMgr;
class ShareSelectAppsWndUI;
class ShareToolBarWndUI;
class ChatContentWndUI;
class TurnPageWndUI;
class MessageTipWndUI;
class CReactionBarWnd;
class CLowerThirdsInfoWnd;
class CBulletScreenWnd;
class IParseChannelCmdWnd;
class CFeedBackLaunchWnd;
class CFeedBackSubmitWnd;
class CFeedBackTipsWnd;
class CLowerThirdsSettingWndUI;
class CMorePopWnd;
class StatisticsWndUI;
class CSwitchCameraWndUI;
class CRemoteControlCameraWndUI;
class CCameraControlRequestReceivedWndUI;
class CStopCameraControlWndUI;
class CGalleryMorePopWnd;
class CControlCameraTipWndUI;
class CControlCameraPopWnd;
class TopTipWndUI;
class CSwitchCameraPopWnd;

class CMainFrame : public WindowImplBase
	             , public IZoomVideoSDKDelegate
{
public:

	static CMainFrame& GetInstance();

	void CreateJoinSessionWnd();
	void CreateTitleBarWnd();
	void CreateSwitchCamera();
	void CreateRemoteControlCamera();
	void CreateCameraControlRequestReceivedWnd();
	void CreateStopCameraControlWnd();
	void CreateControlCameraTipWnd();
	void CreateControlCameraPopWnd();
	void CreateTopTipWnd();
	void CreateSwitchCameraPopWnd();
	void CreateBottomBarWnd();
	void CreateLeaveSessionTipsWnd();
	void CreateSessionInfoWnd();
	void CreateSessionSettingsWnd();
	void CreateLowerThirdsSettingWnd();
	void CreateBulletScreenWnd();
	void CreateChatContentWnd();
	void CreateTurnPageWnd();
	void CreateMessageTipWnd();
	void CreateLowerThirdsInfoWnd();	
	void InitVideoSDK();
	void UninitVideoSDK();

	void ShowBottomBarWnd(bool bShow);
	void SetBottomBarHideTimer();
	void KillBottomBarHideTimer();
	void ShowLeaveSessionTipsWnd(bool bShow);
	void ShowSessionInfoWnd(RECT rc);
	void ShowSessionSettingWnd(bool show,RECT rc = { 0 });
	void ShowLowerThirdsSettingWndUI(bool show, RECT rc = { 0 });

	void ShowBulletScreenWnd(bool show);
	void ShowLowerThirdsInfoWnd(bool show);
	void ShowChatContentWnd(bool show);
	void ShowTurnUpPageWnd(bool show);
	void ShowTurnDownPageWnd(bool show);
	void ShowMessageTipWnd(bool show,wstring error_text = _T(""));
	void StartScreenShare(wstring screen_id);
	void StartShareView(HWND hwnd);

	void SendChatToAll(const zchar_t* msgContent);
	void StartShare2ndCamera();
	void SwitchShareCamera();
	void ShareSelectedCamera(std::wstring deviceID);
	void ShowSwitchShareCameraButton(bool show);
	void ShowRemoteControlCameraWnd(bool show);
	void ShowCameraControlRequestReceivedWnd(bool bShow);
	void ShowStopCameraControlWnd(bool bShow);
	void ShowControlCameraTipButton(bool show);
	IZoomVideoSDKUser* GetCameraControlRequester();
	void ShowSwitchCameraPopWnd(bool show, POINT pt = { 0 });

	void UpdateRenderModeLowerThird(IZoomVideoSDKUser* pUser, wstring lower_third_name, wstring lower_third_description, LowerThirdColorInfo &color_info);
	void UpdateFeedBackNum(int feedback_num);
	void ShowParticipantGallery(bool bShow);
	bool IsParticipantGalleryShown() const;

	bool IsCommandChannelConnect();

	void JoinSession(ZoomVideoSDKSessionContext& session_context);
	void LeaveSession(bool bEnd);

	void HanldeRenderModeChanged(RendererMode mode);
	RendererMode GetRenderMode() { return render_mode_; }

	void SwitchVideoRenderLayoutType(VideoContainerParam& param);
	void OnGalleryContainerSelectUserChange();
	CSessionSettingsWndUI* GetSessionSettingsWndUI() const;
	IZoomVideoSDKUser* GetGalleryContainerSelectUser();
	void RemoveUserReaction(IZoomVideoSDKUser* pUser);
	LowerThirdColorInfo GetLowerThirdsColorInfo(const std::wstring& colorKey);
	LowerThirdInfo	GetUserLowerThirdInfo(IZoomVideoSDKUser* pUser);
	void SetLowerThirdsInfo(std::wstring username, std::wstring description, std::wstring color);
	void GetLowerThirdInfoFromIniFile(std::wstring& username, std::wstring& description, std::wstring& color, std::wstring& enable);
	std::wstring GetLowerThirdConfPath();
	int GetUserReactionResId(IZoomVideoSDKUser* pUser);
	void SetActivityVideoEmojiPos(POINT emoji_pos);

	void HandleTurnUpPageEvent();
	void HandleTurnDownPageEvent();

	void OnJoinSessionCloseBtnClick();
	void OnMinimizeBtnClick();
	void OnFullScreenBtnClick();

	RECT GetMainFrameCenterRect(HWND hwnd);
	VideoShowMgr* GetVideoShowMgr() { return video_show_mgr_; };
protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetResourceID() const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled); //pre handle message
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT	OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

//IZoomVideoSDKDelegate
public:
	virtual void onSessionJoin();
	virtual void onSessionLeave();
	virtual void onSessionLeave(ZoomVideoSDKSessionLeaveReason eReason) {}
	virtual void onError(ZoomVideoSDKErrors errorCode, int detailErrorCode);

	virtual void onUserJoin(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	virtual void onUserLeave(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);

	virtual void onUserVideoStatusChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	virtual void onUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	virtual void onUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction);
	virtual void onFailedToStartShare(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser) {}
	virtual void onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper* pLiveStreamHelper, ZoomVideoSDKLiveStreamStatus status);
	virtual void onChatNewMessageNotify(IZoomVideoSDKChatHelper* pChatHelper, IZoomVideoSDKChatMessage* messageItem);
	virtual void onUserHostChanged(IZoomVideoSDKUserHelper* pUserHelper, IZoomVideoSDKUser* pUser);
	virtual void onUserActiveAudioChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* list);

	virtual void onUserRecordingConsent(IZoomVideoSDKUser* pUser) {}

	virtual void onSessionNeedPassword(IZoomVideoSDKPasswordHandler* handler);
	virtual void onSessionPasswordWrong(IZoomVideoSDKPasswordHandler* handler);

	virtual void onMixedAudioRawDataReceived(AudioRawData* data_);
	virtual void onOneWayAudioRawDataReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser);
	virtual void onSharedAudioRawDataReceived(AudioRawData* data_);
	virtual void onUserManagerChanged(IZoomVideoSDKUser* pUser);
	virtual void onUserNameChanged(IZoomVideoSDKUser* pUser);
	virtual void onCameraControlRequestResult(IZoomVideoSDKUser* pUser, bool isApproved);
	virtual void onCameraControlRequestReceived(IZoomVideoSDKUser* pUser, ZoomVideoSDKCameraControlRequestType requestType, IZoomVideoSDKCameraControlRequestHandler* pCameraControlRequestHandler);
	virtual void onRemoteControlStatus(IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction, ZoomVideoSDKRemoteControlStatus status) {}
	virtual void onRemoteControlRequestReceived(IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction,
		IZoomVideoSDKRemoteControlRequestHandler* pRemoteControlRequestHandler) {}
	virtual void onRemoteControlServiceInstallResult(bool bSuccess) {}
	virtual void onCommandReceived(IZoomVideoSDKUser* sender, const zchar_t* strCmd);
	virtual void onCommandChannelConnectResult(bool isSuccess);
	virtual void onInviteByPhoneStatus(PhoneStatus status, PhoneFailedReason reason);
    virtual void onCalloutJoinSuccess(IZoomVideoSDKUser* pUser, const zchar_t* phoneNumber);
	virtual void onCloudRecordingStatus(RecordingStatus status, IZoomVideoSDKRecordingConsentHandler* pHandler);
	virtual void onHostAskUnmute();
	virtual void onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser* pUser, IZoomVideoSDKRawDataPipe* pVideoPipe);

	virtual void onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume);
	virtual void onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status);
	virtual void onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status);
	virtual void onSelectedAudioDeviceChanged();
    virtual void onCameraListChanged() {}

	virtual void onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status);
	virtual void onLiveTranscriptionMsgReceived(const zchar_t* ltMsg, IZoomVideoSDKUser* pUser, ZoomVideoSDKLiveTranscriptionOperationType type);
	virtual void onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage);
	virtual void onSpokenLanguageChanged(ILiveTranscriptionLanguage* spokenLanguage) {}
	virtual void onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo* messageInfo) {};
	virtual void onOriginalLanguageMsgReceived(ILiveTranscriptionMessageInfo* messageInfo) {};
	virtual void onChatMsgDeleteNotification(IZoomVideoSDKChatHelper* pChatHelper, const zchar_t* msgID, ZoomVideoSDKChatMessageDeleteType deleteBy) {}

	virtual void onProxyDetectComplete() {}
	virtual void onProxySettingNotification(IZoomVideoSDKProxySettingHandler* handler) {}
	virtual void onSSLCertVerifiedFailNotification(IZoomVideoSDKSSLCertificateInfo* handler) {}

	virtual void onUserVideoNetworkStatusChanged(ZoomVideoSDKNetworkStatus status, IZoomVideoSDKUser* pUser) {}
	virtual void onShareNetworkStatusChanged(ZoomVideoSDKNetworkStatus shareNetworkStatus, bool isSendingShare) {}
	virtual void onAnnotationHelperCleanUp(IZoomVideoSDKAnnotationHelper* helper) {}
	virtual void onAnnotationPrivilegeChange(IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction) {}
	virtual void onAnnotationHelperActived(void* handle) {}

	virtual void onCallCRCDeviceStatusChanged(ZoomVideoSDKCRCCallStatus status) {}
#if !defined __LINUX__
	virtual void onVideoCanvasSubscribeFail(ZoomVideoSDKSubscribeFailReason fail_reason, IZoomVideoSDKUser* pUser, void* handle) {}
	virtual void onShareCanvasSubscribeFail(IZoomVideoSDKUser* pUser, void* handle, IZoomVideoSDKShareAction* pShareAction) {}
#endif
	virtual void onChatPrivilegeChanged(IZoomVideoSDKChatHelper* pChatHelper, ZoomVideoSDKChatPrivilegeType privilege) {}
	virtual void onSendFileStatus(IZoomVideoSDKSendFile* file, const FileTransferStatus& status) {}
	virtual void onReceiveFileStatus(IZoomVideoSDKReceiveFile* file, const FileTransferStatus& status) {}
	virtual void onVideoAlphaChannelStatusChanged(bool isAlphaModeOn) {}
	virtual void onSpotlightVideoChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) {}


	virtual void onBindIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID) {}
	virtual void onUnbindIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID) {}
	virtual void onIncomingLiveStreamStatusResponse(bool bSuccess, IVideoSDKVector<IncomingLiveStreamStatus>* pStreamsStatusList) {}
	virtual void onStartIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID) {}
	virtual void onStopIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID) {}
	virtual void onShareContentSizeChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction) {}
	virtual void onShareContentChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction) {}

	virtual void onSubSessionStatusChanged(ZoomVideoSDKSubSessionStatus status, IVideoSDKVector<ISubSessionKit*>* pSubSessionKitList) {}

	virtual void onSubSessionManagerHandle(IZoomVideoSDKSubSessionManager* pManager) {}

	virtual void onSubSessionParticipantHandle(IZoomVideoSDKSubSessionParticipant* pParticipant) {}

	virtual void onSubSessionUsersUpdate(ISubSessionKit* pSubSessionKit) {}

	virtual void onBroadcastMessageFromMainSession(const zchar_t* sMessage, const zchar_t* sUserName) {}

	virtual void onSubSessionUserHelpRequest(ISubSessionUserHelpRequestHandler* pHandler) {}

	virtual void onSubSessionUserHelpRequestResult(ZoomVideoSDKUserHelpRequestResult eResult) {}

	virtual void onShareSettingChanged(ZoomVideoSDKShareSetting setting) {}


private:
	CMainFrame();
	~CMainFrame();
	void DestroyAllOwnedWnd();
	void InitControls();
	void OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnCopyData(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnIconNotify(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void PopupAppWindow();

	void StartPreview();

	void UpdateParticipantNum();
	void UpdateAllOwnedWndPos();
	void UpdateTtileBarWndPos();
	void UpdateBottomBarWndPos();
	void UpdateChatContentWndPos(bool show = false);
	void UpdateTurnUpPageWndPos();
	void UpdateTurnDownPageWndPos();
	void UpdateShareToolBarWndPos();
	void UpdateMessageTipWndPos();
	void UpdateLowerThirdsWndPos();
	void UpdateBulletScreenWndPos();
	void UpdateSwitchCameraWndPos();
	void UpdateRemoteControlCameraWndPos();
	void UpdateControlCameraTipWndPos();
	void UpdateCameraControlRequestReceivedWndPos();
	void UpdateTopTipWndPos();
	void HideOrShowTurnPageWnd();
	void HideTurnPageWnd();
	void HideAllOwnedWnd();

	void InsertContentToChatContentWnd(const zchar_t* user_name, const zchar_t* content);

	void OnLeaveSessionUIUpdate();
	void OnJoinMettingJoinFailed(int error_code);
	void OnInvalidParamterNotification();
	void OnMeetingDisconnecting();
	bool JudgeIsIncludeMySelf(IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	void SetCommandChannelConnect(bool is_connect);

	void SetNotifyIconData(NOTIFYICONDATA& nid);
	void AddTheTray();
	void DeleteTheTray();
	
private:
	CJoinSessionWndUI*		join_session_wnd_ = nullptr;
	CTitleBarWndUI*			title_bar_wnd_ = nullptr;
	CBottomBarWndUI*		bottom_bar_wnd_ = nullptr;
	CLeaveSessionTipsWndUI* leave_session_tips_wnd_ = nullptr;
	CSessionInfoWndUI*		session_infos_wnd_ = nullptr;
	CSessionSettingsWndUI*	session_setting_wnd_ = nullptr;
	ShareSelectAppsWndUI*	share_select_apps_wnd_ = nullptr;
	CReactionBarWnd*		reaction_bar_wnd_ = nullptr;
	CMorePopWnd*			more_pop_wnd_ = nullptr;
	ShareToolBarWndUI*		share_tool_bar_wnd_ = nullptr;
	ChatContentWndUI*		chat_content_wnd_ = nullptr;
	TurnPageWndUI*			turn_up_page_wnd_ = nullptr;
	TurnPageWndUI*			turn_down_page_wnd_ = nullptr;	
	MessageTipWndUI*		message_tip_wnd_ = nullptr;
	CBulletScreenWnd*		bullet_screen_wnd_ = nullptr;
	CLowerThirdsInfoWnd*	lower_thirds_info_wnd_ = nullptr;
	CFeedBackLaunchWnd*		feedback_launch_wnd_ = nullptr;
	CFeedBackSubmitWnd*		feedback_submit_wnd_ = nullptr;
	CFeedBackTipsWnd*		feedback_tips_wnd_ = nullptr;
	CLowerThirdsSettingWndUI* lower_thirds_setting_wnd_ = nullptr;
	StatisticsWndUI* statistics_wnd_ = nullptr;
	CSwitchCameraWndUI* switch_camera_wnd_ = nullptr;
	CRemoteControlCameraWndUI* remote_control_camera_wnd_ = nullptr;
	CCameraControlRequestReceivedWndUI* camera_control_request_wnd_ = nullptr;
	CStopCameraControlWndUI* stop_camera_control_wnd_ = nullptr;
	CGalleryMorePopWnd* gallery_more_pop_wnd_ = nullptr;
	CControlCameraTipWndUI* control_camera_tip_wnd_ = nullptr;
	CControlCameraPopWnd* control_camera_pop_wnd_ = nullptr;
	TopTipWndUI* top_tip_wnd_ = nullptr;
	CSwitchCameraPopWnd* switch_camera_pop_wnd_ = nullptr;

	map<CmdChannelType, IParseChannelCmdWnd*> cmd_channel_wnd_map_;
	IZoomVideoSDKRenderer*	renderer_ = nullptr;

	bool is_show_gallery_ = false;
	bool is_command_channel_connected_ = false;
	RendererMode render_mode_ = kVideoSDKRenderMode_AspectRatio;
	bool is_full_screen_ = false;
public:

	VideoShowMgr* video_show_mgr_;
};
