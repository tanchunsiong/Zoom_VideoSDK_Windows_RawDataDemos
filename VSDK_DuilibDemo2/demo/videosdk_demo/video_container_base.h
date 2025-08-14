#ifndef _VIDEO_CONTAINER_BASE_H_
#define _VIDEO_CONTAINER_BASE_H_
//#include "zoom_video_sdk_include_interface.h"
#include "video_show_mgr_comm_def.h"

struct LowerThirdColorInfo;
class VideoLayoutItemBase;
class VideoContainerBase
{
public:
	VideoContainerBase(IZoomVideoSDKRenderer* renderer);
	virtual ~VideoContainerBase();

	virtual void UpdateLayoutItemPos() = 0;

	virtual void CreateAllItems(VideoRenderLayoutType layout_type) = 0;

	virtual void SubscribeAllUser() = 0;

	virtual void AddRendererVideoItem(IZoomVideoSDKUser* user) = 0;

	virtual void AddUserAvatarIcon(IZoomVideoSDKUser* user) = 0;

	virtual void Subscribe(IZoomVideoSDKUser* user, ZoomVideoSDKRawDataType dataType) = 0;

	virtual void Init() = 0;

	virtual void UnsubscribeAllItem();

	virtual void SetPos(RendererRect rc);

	virtual void SwitchRenderMode(RendererMode mode);

	void SetInset(RECT rect_inset);

	void SetRenderMode(RendererMode mode);

	void SetRowAndColumnGap(uint32_t row_gap, uint32_t column_gap);

	VideoLayoutItemBase* FindIdelItem();

	VideoLayoutItemBase* FindItemByUser(IZoomVideoSDKUser* user);

	VideoLayoutItemBase* FindSelectedItem();

	void RemoveAllItem();

	void SwitchVideoRenderLayoutType(VideoRenderLayoutType layout_type);

	bool IsPointOnRenderingItem(POINT pt,int margin = 0);

//Handle from UI Event
public:
	virtual void OnMouseMove(POINT pt);

	virtual void OnLButtonUp(POINT pt);

	virtual void OnSize(RendererRect rect);

	virtual void OnUserJoin(IVideoSDKVector<IZoomVideoSDKUser*>* userList);

	virtual void OnUserLeave(IVideoSDKVector<IZoomVideoSDKUser*>* userList);

	virtual void OnUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type);

	virtual void OnUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IZoomVideoSDKUser* user);



	virtual void OnUpdateRenderModeLowerThird(IZoomVideoSDKUser* pUser, wstring lower_third_name, wstring lower_third_description, LowerThirdColorInfo& color_info);

protected:
	void AdjustForInset(RendererRect& rc);

protected:

	IZoomVideoSDKRenderer* renderer_;

	SIZE size_item_;
	RECT rect_inset_;
	RendererRect rect_;
	RendererMode mode_;
	uint32_t row_gap_;
	uint32_t column_gap_;

	VideoRenderLayoutType cur_layout_type_;

	std::vector<VideoLayoutItemBase*> layout_item_list_;
};

#endif //_VIDEO_CONTAINER_BASE_H_