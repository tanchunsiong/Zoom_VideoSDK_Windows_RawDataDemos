#include "stdafx.h"
//#include "zoom_video_sdk_include_interface.h"
#include "wall_mode_video_layout_item.h"
#include "zoom_videosdk_renderer_interface.h"

NormalVideoLayoutItem::NormalVideoLayoutItem(IZoomVideoSDKRenderer* renderer):VideoLayoutItemBase(renderer)
{

}

NormalVideoLayoutItem::~NormalVideoLayoutItem()
{

}

void NormalVideoLayoutItem::Subscribe(IZoomVideoSDKUser* pUser, ZoomVideoSDKRawDataType dataType, ZoomVideoSDKResolution eResolution)
{
	VideoLayoutItemBase::Subscribe(pUser, dataType, eResolution);

	UpdateRenderUserName();
	//UpdateAudioUserName();
	UpdateLowerThird();
	UpdateUserEmoji(user_emoji_resid_);
	UpdateMoreBtn();
}