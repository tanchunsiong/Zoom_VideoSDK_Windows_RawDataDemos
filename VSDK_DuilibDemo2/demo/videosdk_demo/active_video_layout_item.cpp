#include "stdafx.h"
#include "active_video_layout_item.h"
#include "zoom_videosdk_renderer_interface.h"

ActiveVideoLayoutItem::ActiveVideoLayoutItem(IZoomVideoSDKRenderer* renderer):VideoLayoutItemBase(renderer)
{

}

ActiveVideoLayoutItem::~ActiveVideoLayoutItem()
{

}

void ActiveVideoLayoutItem::Subscribe(IZoomVideoSDKUser* pUser, ZoomVideoSDKRawDataType dataType, ZoomVideoSDKResolution eResolution)
{
	VideoLayoutItemBase::Subscribe(pUser, dataType, eResolution);


}

