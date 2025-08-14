#ifndef _ZOOM_VIDEO_CONTAINER_H_
#define _ZOOM_VIDEO_CONTAINER_H_

#include "video_container_base.h"

class VideoLayoutItem;
class GalleryModeVideoContainer : public VideoContainerBase
{
public:
	GalleryModeVideoContainer(IZoomVideoSDKRenderer* renderer);
	~GalleryModeVideoContainer();

	//overwrite
	virtual void UpdateLayoutItemPos();
	virtual void CreateAllItems(VideoRenderLayoutType layout_type);
	virtual void SubscribeAllUser();
	virtual void UnsubscribeAllItem();
	virtual void SetPos(RendererRect rc);
	virtual void SwitchRenderMode(RendererMode mode);
	virtual void AddRendererVideoItem(IZoomVideoSDKUser* user);
	virtual void AddUserAvatarIcon(IZoomVideoSDKUser* user);
	virtual void Subscribe(IZoomVideoSDKUser* user, ZoomVideoSDKRawDataType dataType);
	virtual void Init();
public: //Handle from UI Event
	virtual void OnLButtonUp(POINT pt);
	virtual void OnUserJoin(IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	virtual void OnUserLeave(IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	virtual void OnUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type);
public:

	void ShowGallery(bool show_gallery);

	int GetGalleryWidth();

	int GetTotalPage();

	int GetCurPage();

	void TurnUpPage();

	void TurnDownPage();

	void SwitchShareCamera();
	void ShareSelectedCamera(std::wstring deviceID);

private:
	void CalculateActiveItemRect(RendererRect& rect_active);
	void UpdatePerPageCount();
	int CalculateGalleryTopStartPos(int show_count);
	void UnsubsrcibeAllGalleryItem();
	void OnSizeChangeUpdateGalleryRenderingItemList();
	void OnUserJoinUpdateGalleryRenderingItemList(IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	void OnUserLeaveUpdateGalleryRenderingItemList(IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	void OnTurnPageUpdateGalleryRenderingItemList(bool turn_up);
	void AddGalleryRenderingItem(IZoomVideoSDKUser* user);
	void DeleteGalleryRenderingItem(IZoomVideoSDKUser* user);
	void ClearGalleryRenderingItem();
	void UpdateMapPageUserList();
	void UpdateCurGalleryPage();
	void SubscribeCurrentPageUser();
	void FillRenderingItem();
	VideoLayoutItemBase* FindRenderingItemByUser(IZoomVideoSDKUser* user);
	bool IsClickedOnGalleryRenderingItem(POINT pt);
	VideoLayoutItemBase* FindSelectedGalleryRenderingItem();
	void UpdateGalleryRenderRect();
private:

	bool show_gallery_;

	RendererRect rect_gallery_render_;

	VideoLayoutItemBase* active_video_layout_item_;
	std::vector<VideoLayoutItemBase*> rendering_gallery_item_list_;
	map<int, vector<IZoomVideoSDKUser*>> map_page_user_list_;

	int per_page_max_count_;
	int per_page_count_;
	int current_page_;
};

#endif //_ZOOM_VIDEO_CONTAINER_H_