#include "stdafx.h"
#include "video_container_base.h"
#include "video_layout_item_base.h"
#include "videosdk_demo_mgr.h"

#define MOUSE_HOVER_MARGIN	15

VideoContainerBase::VideoContainerBase(IZoomVideoSDKRenderer* renderer) :renderer_(renderer), row_gap_(0), column_gap_(0),
cur_layout_type_(kVideoRenderLayoutTypeNone), mode_(kVideoSDKRenderMode_AspectRatio)
{

}

VideoContainerBase::~VideoContainerBase()
{
	RemoveAllItem();
}

void VideoContainerBase::SetInset(RECT rect_inset)
{
	rect_inset_ = rect_inset;
}

void VideoContainerBase::SetRenderMode(RendererMode mode)
{
	mode_ = mode;
}

void VideoContainerBase::SetRowAndColumnGap(uint32_t row_gap, uint32_t column_gap)
{
	row_gap_ = row_gap;
	column_gap_ = column_gap;
}


void VideoContainerBase::SetPos(RendererRect rc)
{
	rect_ = rc;
	UpdateLayoutItemPos();
}

VideoLayoutItemBase* VideoContainerBase::FindIdelItem()
{
	for (auto iter = layout_item_list_.begin(); iter != layout_item_list_.end(); iter++)
	{
		VideoLayoutItemBase* item = *iter;
		if (item)
		{
			if (item->IsIdle())
			{
				return item;
			}
		}
	}

	return nullptr;
}

VideoLayoutItemBase* VideoContainerBase::FindItemByUser(IZoomVideoSDKUser* user)
{
	if (nullptr == user) return nullptr;

	for (auto iter = layout_item_list_.begin(); iter != layout_item_list_.end(); iter++)
	{
		VideoLayoutItemBase* item = *iter;
		if (item)
		{
			if (item->GetUser() == user)
			{
				return item;
			}
		}
	}

	return nullptr;
}

VideoLayoutItemBase* VideoContainerBase::FindSelectedItem()
{
	for (auto iter = layout_item_list_.begin(); iter != layout_item_list_.end(); iter++)
	{
		VideoLayoutItemBase* item = *iter;
		if (item)
		{
			if (item->get_is_selected())
			{
				return item;
			}
		}
	}

	return nullptr;
}

void VideoContainerBase::RemoveAllItem()
{
	for (auto iter = layout_item_list_.begin(); iter != layout_item_list_.end();)
	{
		VideoLayoutItemBase* item = *iter;
		if (item)
		{
			delete item;
		}
		layout_item_list_.erase(iter);
	}
	layout_item_list_.clear();
}

void VideoContainerBase::SwitchVideoRenderLayoutType(VideoRenderLayoutType layout_type)
{
	if (cur_layout_type_ == layout_type)
		return;
	cur_layout_type_ = layout_type;

	RemoveAllItem();

	CreateAllItems(layout_type);
}

void VideoContainerBase::SwitchRenderMode(RendererMode mode)
{
	mode_ = mode;
	for (auto iter = layout_item_list_.begin(); iter != layout_item_list_.end();iter++)
	{
		VideoLayoutItemBase* item = *iter;
		if (item)
		{
			item->SwitchRenderMode(mode_);
		}
	}
}

void VideoContainerBase::UnsubscribeAllItem()
{
	for (auto iter = layout_item_list_.begin(); iter != layout_item_list_.end();iter++)
	{
		VideoLayoutItemBase* item = *iter;
		if (item)
		{
			item->unSubscribe();
		}
	}
}

bool VideoContainerBase::IsPointOnRenderingItem(POINT pt, int margin)
{
	for (auto iter = layout_item_list_.begin(); iter != layout_item_list_.end(); iter++)
	{
		VideoLayoutItemBase* item = *iter;

		if (!item) continue;
		if (item->IsIdle()) continue;

		RendererRect rect_item = item->GetPos();
		CRect rect = { (int)rect_item.left - margin,(int)rect_item.top - margin,\
			(int)(rect_item.left + rect_item.width + margin),(int)(rect_item.top + rect_item.height + margin) };
		if (rect.PtInRect(pt))
		{
			return true;
		}
	}

	return false;
}

void VideoContainerBase::OnMouseMove(POINT pt)
{
	if (!IsPointOnRenderingItem(pt, MOUSE_HOVER_MARGIN))
		return;

	for (auto iter = layout_item_list_.begin(); iter != layout_item_list_.end(); iter++)
	{
		VideoLayoutItemBase* item = *iter;

		if (!item) continue;
		if (item->IsIdle()) continue;

		RendererRect rect_item = item->GetPos();
		CRect rect = { (int)rect_item.left,(int)rect_item.top,(int)(rect_item.left + rect_item.width),(int)(rect_item.top + rect_item.height) };

	}
}

void VideoContainerBase::OnLButtonUp(POINT pt)
{
	//if not clicked on video item,do nothing
	if (!IsPointOnRenderingItem(pt))
		return;

	for (auto iter = layout_item_list_.begin(); iter != layout_item_list_.end(); iter++)
	{
		VideoLayoutItemBase* item = *iter;

		if (!item) continue;
		if (item->IsIdle()) continue;

		RendererRect rect_item = item->GetPos();
		CRect rect = { (int)rect_item.left,(int)rect_item.top,(int)(rect_item.left + rect_item.width),(int)(rect_item.top + rect_item.height) };
		if (rect.PtInRect(pt))
		{
			item->set_is_selected(true);
		}
		else
		{
			item->set_is_selected(false);
		}
	}
}

void VideoContainerBase::OnSize(RendererRect rect)
{
	SetPos(rect);
}

void VideoContainerBase::OnUserJoin(IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (!userList) return;
	
	for (int i = 0; i < userList->GetCount(); i++)
	{
		IZoomVideoSDKUser* join_user = userList->GetItem(i);
		VideoLayoutItemBase* layout_item = FindIdelItem();
		if (layout_item)
		{
			layout_item->Subscribe(join_user, RAW_DATA_TYPE_VIDEO, ZoomVideoSDKResolution_180P);
		}
	}
}

void VideoContainerBase::OnUserLeave(IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (!userList) return;

	for (int i = 0; i < userList->GetCount(); i++)
	{
		IZoomVideoSDKUser* leave_user = userList->GetItem(i);
		if (!leave_user) continue;
		VideoLayoutItemBase* layout_item = FindItemByUser(leave_user);
		if (layout_item)
		{
			layout_item->unSubscribe();
		}
	}

	UpdateLayoutItemPos();
}

void VideoContainerBase::OnUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type)
{
	VideoLayoutItemBase* item = FindItemByUser(pUser);
	if (item)
	{
		item->OnShareStatusChange();
	}
}

void VideoContainerBase::OnUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IZoomVideoSDKUser* user)
{
	VideoLayoutItemBase* layout_item = FindItemByUser(user);
	if (layout_item)
	{
		layout_item->OnAudioStatusChange();
	}
}

void VideoContainerBase::OnUserEmojiStatusChanged(IZoomVideoSDKUser* user, int res_id)
{
	VideoLayoutItemBase* layout_item = FindItemByUser(user);
	if (!layout_item)
		return;

	layout_item->OnEmojiStatusChange(res_id);
}

void VideoContainerBase::OnUpdateRenderModeLowerThird(IZoomVideoSDKUser* pUser, wstring lower_third_name, wstring lower_third_description, LowerThirdColorInfo& color_info)
{
	VideoLayoutItemBase* layout_item = FindItemByUser(pUser);
	if (!layout_item)
		return;
		
	layout_item->SetLowerThirdInfo(lower_third_name,lower_third_description, color_info);
	layout_item->OnLowerThirdChange();
}

void VideoContainerBase::AdjustForInset(RendererRect& rc)
{
	rc.left += rect_inset_.left;
	rc.top += rect_inset_.top;
	rc.width -= rect_inset_.left + rect_inset_.right;
	rc.height -= rect_inset_.top + rect_inset_.bottom;
}