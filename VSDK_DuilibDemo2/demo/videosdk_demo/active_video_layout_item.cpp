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

	UpdateUserEmoji(-1);
}

void ActiveVideoLayoutItem::UpdateUserEmoji(int res_id)
{
	if (!renderer_) return;

	auto iter = map_layed_element_.find(_T("Emoji"));
	if (iter != map_layed_element_.end())
	{
		Gdiplus::Bitmap* pImage = LoadResImage(res_id);
		if (!pImage)
		{
			renderer_->RemoveLayeredElement(iter->second);
			map_layed_element_.erase(iter);
			return;
		}
		Gdiplus::BitmapData bitmap_data;
		GetImageBitmapData(pImage, bitmap_data);

		PixelsData pixels_data;
		pixels_data.width = bitmap_data.Width;
		pixels_data.height = bitmap_data.Height;
		pixels_data.pixels = (uint8_t*)bitmap_data.Scan0;
		pixels_data.pitch = bitmap_data.Stride;

		renderer_->ModifyLayeredImageElementPixelsData(iter->second, pixels_data);

		pImage->UnlockBits(&bitmap_data);
		delete pImage;
		pImage = nullptr;
	}
	else
	{
		Gdiplus::Bitmap* pImage = LoadResImage(res_id);
		if (!pImage)
			return;

		Gdiplus::BitmapData bitmap_data;
		GetImageBitmapData(pImage, bitmap_data);

		PixelsData pixels_data;
		pixels_data.width = bitmap_data.Width;
		pixels_data.height = bitmap_data.Height;
		pixels_data.pixels = (uint8_t*)bitmap_data.Scan0;
		pixels_data.pitch = bitmap_data.Stride;

		RendererRect rect;
		rect.width = bitmap_data.Width;
		rect.height = bitmap_data.Height;
		AddLayeredImageElement(_T("Emoji"), bitmap_data, rect, 2);

		renderer_->ModifyLayeredImageElementPixelsData(iter->second, pixels_data);

		pImage->UnlockBits(&bitmap_data);
		delete pImage;
		pImage = nullptr;
	}
}