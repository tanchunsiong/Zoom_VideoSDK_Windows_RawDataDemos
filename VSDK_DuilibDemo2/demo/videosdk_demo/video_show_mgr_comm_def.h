#ifndef _VIDEO_RENDER_MGR_COMM_DEF_H_
#define _VIDEO_RENDER_MGR_COMM_DEF_H_

#include "zoom_videosdk_renderer_def.h"
#include "zoom_videosdk_renderer_interface.h"

#define ZOOM_LAYOUT_ITEM_WIDTH 204
#define ZOOM_LAYOUT_ITEM_HEIGHT 117
#define ZOOM_LAYOUT_GALLERY_TOP_MARGIN 25

enum VideoRenderLayoutType
{
	kVideoRenderLayoutTypeNone = 0,
	kVideoRenderLayoutType1x6,
};

struct VideoContainerParam
{
	RECT rect_inset;
	uint32_t row_gap;
	uint32_t column_gap;
	VideoRenderLayoutType layout_type;
	RendererMode mode;
	VideoContainerParam()
	{
		rect_inset = { 20,20,20,20 };
		row_gap = 20;
		column_gap = 20;
		layout_type = kVideoRenderLayoutType1x6;
		mode = kVideoSDKRenderMode_AspectRatio;
	}
};

#endif //_VIDEO_RENDER_MGR_COMM_DEF_H_