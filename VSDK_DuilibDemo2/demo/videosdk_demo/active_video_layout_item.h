#ifndef _ACTIVE_VIDEO_LAYOUT_ITEM_H_
#define _ACTIVE_VIDEO_LAYOUT_ITEM_H_

#include "video_layout_item_base.h"

class ActiveVideoLayoutItem : public VideoLayoutItemBase
{
public:
	ActiveVideoLayoutItem(IZoomVideoSDKRenderer* renderer);
	~ActiveVideoLayoutItem();

	virtual void Subscribe(IZoomVideoSDKUser* pUser, ZoomVideoSDKRawDataType dataType, ZoomVideoSDKResolution eResolution);

	void UpdateUserEmoji(int res_id);
};

#endif //_ACTIVE_VIDEO_LAYOUT_ITEM_H_