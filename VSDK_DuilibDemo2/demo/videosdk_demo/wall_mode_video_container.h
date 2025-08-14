#ifndef _VIDEO_CONTAINER_H_
#define _VIDEO_CONTAINER_H_

#include "video_container_base.h"

class VideoLayoutItem;
class NormalVideoContainer : public VideoContainerBase
{
public:
	NormalVideoContainer(IZoomVideoSDKRenderer* renderer);
	~NormalVideoContainer();

	//overwrite
	virtual void UpdateLayoutItemPos();
	virtual void CreateAllItems(VideoRenderLayoutType layout_type);
	virtual void SubscribeAllUser(ZoomVideoSDKResolution resolution);

public: //Handle from UI Event
	virtual void OnUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type);
private:
	void CalculateItemSize(uint32_t row_column_num);
	void CalculateItemPos(uint32_t row_column_num);
};

#endif //_VIDEO_CONTAINER_H_