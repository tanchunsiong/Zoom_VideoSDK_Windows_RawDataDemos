#ifndef _NORMAL_VIDEO_LAYOUT_ITEM_H_
#define _NORMAL_VIDEO_LAYOUT_ITEM_H_

#include "video_layout_item_base.h"

class NormalVideoLayoutItem : public VideoLayoutItemBase
{
public:
	NormalVideoLayoutItem(IZoomVideoSDKRenderer* renderer);
	~NormalVideoLayoutItem();

	virtual void Subscribe(IZoomVideoSDKUser* pUser, ZoomVideoSDKRawDataType dataType, ZoomVideoSDKResolution eResolution);
};

#endif //_NORMAL_VIDEO_LAYOUT_ITEM_H_