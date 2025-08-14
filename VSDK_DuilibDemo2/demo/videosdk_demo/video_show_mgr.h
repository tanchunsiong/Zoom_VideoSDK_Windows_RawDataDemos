#ifndef _ZOOM_VIDEO_CONTAINER_LAYOUT_MGR_H_
#define _ZOOM_VIDEO_CONTAINER_LAYOUT_MGR_H_

#include "video_show_mgr_comm_def.h"

struct LowerThirdColorInfo;
class VideoContainerBase;
class VideoLayoutItemBase;

class VideoShowMgr
{
public:
	VideoShowMgr(IZoomVideoSDKRenderer* renderer, RendererRect rect_renderer);
	~VideoShowMgr();

	void UnsubscribeAllItem();

	VideoRenderLayoutType GetCurrentLayoutType();

	int GetGalleryContainerGalleryWidth();

	int GetGalleryContainerTotalPage();

	int GetGalleryContainerCurPage();

	VideoLayoutItemBase* GetGalleryContainerSelectItem();

	void SubscribeAllUser();
	
	void Subscribe(IZoomVideoSDKUser* user, ZoomVideoSDKRawDataType dataType);

	void AddRendererVideoItem(IZoomVideoSDKUser* user);

	void AddUserAvatarIcon(IZoomVideoSDKUser* user);

	void ContainerInit();

	
public: //Handle from UI Event

	void OnUserJoin(IVideoSDKVector<IZoomVideoSDKUser*>* userList);

	void OnUserLeave(IVideoSDKVector<IZoomVideoSDKUser*>* userList);

	void OnSessionLeave();

	void OnUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type);

	void OnUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);



	void OnSize(RendererRect rect);

	void OnLButtonUp(POINT pt);

	void OnMouseMove(POINT pt);

	void SwitchVideoRenderLayoutType(const VideoContainerParam& param);

	void OnGalleryContainerShowGallery(bool show_gallery);

	void OnTurnUpPage();

	void OnTurnDownPage();

	void SwitchRenderMode(RendererMode mode);

	void OnUpdateRenderModeLowerThird(IZoomVideoSDKUser* pUser, wstring lower_third_name, wstring lower_third_description, LowerThirdColorInfo& color_info);

	void SwitchShareCamera();
	void ShareSelectedCamera(std::wstring deviceID);

private:

	IZoomVideoSDKRenderer* renderer_;

	VideoContainerBase* video_container_;

	RendererRect rect_renderer_;

	RECT rect_inset_;

	VideoRenderLayoutType cur_layout_type_;
};



#endif //_ZOOM_VIDEO_CONTAINER_LAYOUT_MGR_H_