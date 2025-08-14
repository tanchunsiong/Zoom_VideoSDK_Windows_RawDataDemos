#include "stdafx.h"
#include "gallary_mode_video_container.h"
#include "wall_mode_video_layout_item.h"
#include "active_video_layout_item.h"
#include "videosdk_demo_mgr.h"
#include "main_frame.h"

#define MORE_BTN_LEFT_MARGIN 34
#define MORE_BTN_TOP_MARGIN 10

GalleryModeVideoContainer::GalleryModeVideoContainer(IZoomVideoSDKRenderer* renderer) :VideoContainerBase(renderer), show_gallery_(false),
                                                                                       active_video_layout_item_(nullptr), per_page_max_count_(0),
																					   per_page_count_(0),current_page_(0)
{

}

GalleryModeVideoContainer::~GalleryModeVideoContainer()
{
	if (active_video_layout_item_)
	{
		delete active_video_layout_item_;
		active_video_layout_item_ = nullptr;
	}
}

void GalleryModeVideoContainer::CalculateActiveItemRect(RendererRect& rect_active)
{
	RendererRect rc = rect_;

	//adjust for inset
	AdjustForInset(rc);

	if (show_gallery_)
	{
		rect_active = { rc.left,rc.top,rc.width - ZOOM_LAYOUT_ITEM_WIDTH - (int32_t)column_gap_,rc.height };
	}
	else
	{
		rect_active = { rc.left,rc.top,rc.width,rc.height };
	}
}

void GalleryModeVideoContainer::UpdatePerPageCount()
{
	per_page_max_count_ = (rect_gallery_render_.height + row_gap_) / (ZOOM_LAYOUT_ITEM_HEIGHT + row_gap_); //max * ZOOM_LAYOUT_ITEM_HEIGHT + (max -1) * row_gap_ = rect_gallery_render_.height

	if (per_page_max_count_ > layout_item_list_.size())
		per_page_max_count_ = layout_item_list_.size();

	per_page_count_ = per_page_max_count_;

	if (per_page_count_ > ZoomVideoSDKMgr::GetInst().GetUserCountInSession())
		per_page_count_ = ZoomVideoSDKMgr::GetInst().GetUserCountInSession();
}

int GalleryModeVideoContainer::CalculateGalleryTopStartPos(int show_count)
{
	int top_start_pos = 0;

	top_start_pos = rect_gallery_render_.top + (rect_gallery_render_.height - show_count * ZOOM_LAYOUT_ITEM_HEIGHT - (show_count - 1) * row_gap_) / 2;

	return top_start_pos;
}

void GalleryModeVideoContainer::UpdateLayoutItemPos()
{
	RendererRect rc = rect_;

	//adjust for inset
	AdjustForInset(rc);

	RendererRect rect_active = { 0 };
	CalculateActiveItemRect(rect_active);

	//set active item pos
	if (active_video_layout_item_)
	{
		active_video_layout_item_->SetPos(rect_active);
	}

	if (!show_gallery_ || rendering_gallery_item_list_.empty())
		return;

	//set gallery item pos
	int show_count = rendering_gallery_item_list_.size();
	int top_start_pos = CalculateGalleryTopStartPos(show_count);

	if (show_count <= 0 || show_count > layout_item_list_.size())
	{
		return;
	}

	for (int i = 0; i < show_count; i++)
	{
		VideoLayoutItemBase* item = rendering_gallery_item_list_[i];
		if (item)
		{
			RendererRect rect_item = rect_gallery_render_;
			rect_item.top = rc.top + top_start_pos + i * (ZOOM_LAYOUT_ITEM_HEIGHT + row_gap_);
			rect_item.width = ZOOM_LAYOUT_ITEM_WIDTH;
			rect_item.height = ZOOM_LAYOUT_ITEM_HEIGHT;
			item->SetPos(rect_item);
		}
	}
}

void GalleryModeVideoContainer::CreateAllItems(VideoRenderLayoutType layout_type)
{
	if (layout_type != kVideoRenderLayoutType1x6)
		return;

	show_gallery_ = false;

	active_video_layout_item_ = new ActiveVideoLayoutItem(renderer_);

	active_video_layout_item_->SetRenderMode(mode_);

	for (int i = 0; i < 6; i++)
	{
		VideoLayoutItemBase* layout_item = new NormalVideoLayoutItem(renderer_);
		layout_item->SetRenderMode(mode_);
		if (layout_item)
		{
			layout_item_list_.push_back(layout_item);
		}
	}

	SetPos(rect_);
}

void GalleryModeVideoContainer::SubscribeAllUser()
{
	//this contatiner recreate, always only show active element
	vector<IZoomVideoSDKUser*> user_list = ZoomVideoSDKMgr::GetInst().GetAllUsers();

	if (user_list.size() == 0)
		return;

	IZoomVideoSDKUser* user = nullptr;
	for (size_t i = 0; i < user_list.size(); i++)
	{
		user = user_list[i];
		if (!user) continue;

		if (active_video_layout_item_)
		{
			if (active_video_layout_item_->IsIdle())
			{
				active_video_layout_item_->Subscribe(user, RAW_DATA_TYPE_VIDEO, ZoomVideoSDKResolution_1080P);
				break;
			}
		}
	}
}

void GalleryModeVideoContainer::UnsubscribeAllItem()
{
	ClearGalleryRenderingItem();

	if (active_video_layout_item_)
	{
		active_video_layout_item_->unSubscribe();
	}
}

void GalleryModeVideoContainer::UpdateGalleryRenderRect()
{
	rect_gallery_render_.left = rect_.left + rect_.width - ZOOM_LAYOUT_ITEM_WIDTH - column_gap_;
	rect_gallery_render_.top = rect_.top + ZOOM_LAYOUT_GALLERY_TOP_MARGIN;
	rect_gallery_render_.width = ZOOM_LAYOUT_ITEM_WIDTH;
	rect_gallery_render_.height = rect_.height - ZOOM_LAYOUT_GALLERY_TOP_MARGIN * 2;
}

void GalleryModeVideoContainer::SetPos(RendererRect rc)
{
	rect_ = rc;

	UpdateGalleryRenderRect();

	OnSizeChangeUpdateGalleryRenderingItemList();
}

void GalleryModeVideoContainer::SwitchRenderMode(RendererMode mode)
{
	mode_ = mode;

	if (active_video_layout_item_)
	{
		active_video_layout_item_->SwitchRenderMode(mode_);
	}

	for (auto iter = rendering_gallery_item_list_.begin(); iter != rendering_gallery_item_list_.end(); iter++)
	{
		VideoLayoutItemBase* item = *iter;
		if (item)
		{
			item->SwitchRenderMode(mode_);
		}
	}
}

void GalleryModeVideoContainer::AddRendererVideoItem(IZoomVideoSDKUser* user)
{
	if (active_video_layout_item_)
	{
		if (active_video_layout_item_->IsIdle())
		{
			active_video_layout_item_->AddRendererVideoItem(user);
		}
	}
}

void GalleryModeVideoContainer::AddUserAvatarIcon(IZoomVideoSDKUser* user)
{
	if (!user || !active_video_layout_item_)
		return;

	IZoomVideoSDKUser* active_user = active_video_layout_item_->GetUser();
	if (active_user == user)
	{
		active_video_layout_item_->AddUserAvatarIcon();
	}
}

void GalleryModeVideoContainer::Subscribe(IZoomVideoSDKUser* user,ZoomVideoSDKRawDataType dataType)
{
	if (active_video_layout_item_)
	{
		if (active_video_layout_item_->GetUser() == user)
		{
			if (!active_video_layout_item_->IsIdle() && !active_video_layout_item_->GetUserHasSubscribed())
			{
				active_video_layout_item_->Subscribe(user, dataType, ZoomVideoSDKResolution_1080P);
			}
		}
	}
}

void GalleryModeVideoContainer::Init()
{
	if (active_video_layout_item_)
	{
		active_video_layout_item_->unSubscribe();
	}

	for (size_t i = 0; i < layout_item_list_.size(); i++)
	{
		if (layout_item_list_[i])
		{
			layout_item_list_[i]->unSubscribe();
		}
	}

	rendering_gallery_item_list_.clear();
	map_page_user_list_.clear();

	rect_inset_ = { 0,0,0,0 };
	per_page_max_count_ = 0;
	per_page_count_ = 0;
	current_page_ = 0;
	show_gallery_ = false;

	UpdateLayoutItemPos();
}

void GalleryModeVideoContainer::UnsubsrcibeAllGalleryItem()
{
	ClearGalleryRenderingItem();
}

void GalleryModeVideoContainer::OnSizeChangeUpdateGalleryRenderingItemList()
{
	if (show_gallery_)
	{
		int old_per_page_count = per_page_count_;
		UpdateMapPageUserList();
		if (old_per_page_count != per_page_count_)
		{
			UpdateCurGalleryPage();
			SubscribeCurrentPageUser();
		}
	}

	UpdateLayoutItemPos();
}

void GalleryModeVideoContainer::OnUserJoinUpdateGalleryRenderingItemList(IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (!show_gallery_ || !userList)
		return;

	for (int i = 0; i < userList->GetCount(); i++)
	{
		IZoomVideoSDKUser* user = userList->GetItem(i);
		if (!user) continue;

		AddGalleryRenderingItem(user);
	}

	UpdateMapPageUserList();
	UpdateCurGalleryPage();

	UpdateLayoutItemPos();
}

void GalleryModeVideoContainer::OnUserLeaveUpdateGalleryRenderingItemList(IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (!show_gallery_ || !userList)
		return;

	for (int i = 0; i < userList->GetCount(); i++)
	{
		IZoomVideoSDKUser* user = userList->GetItem(i);
	
		if (FindRenderingItemByUser(user))
		{
			DeleteGalleryRenderingItem(user);
		}
	}

	UpdateMapPageUserList();

	int old_cur_page = current_page_;
	UpdateCurGalleryPage();

	if (old_cur_page != current_page_)
	{
		SubscribeCurrentPageUser();
	}
	else if(per_page_count_ > rendering_gallery_item_list_.size())
	{
		FillRenderingItem();
	}

}

void GalleryModeVideoContainer::OnTurnPageUpdateGalleryRenderingItemList(bool turn_up)
{
	if (!show_gallery_)
		return;


	if (turn_up)
	{
		current_page_--;
	}
	else
	{
		current_page_++;
	}
	
	if (current_page_ <= 0 || current_page_ > GetTotalPage())
		return;

	UpdateMapPageUserList();
	SubscribeCurrentPageUser();

	UpdateLayoutItemPos();
}

void GalleryModeVideoContainer::AddGalleryRenderingItem(IZoomVideoSDKUser* user)
{
	if (!user)
		return;

	if (per_page_max_count_ > rendering_gallery_item_list_.size())
	{
		VideoLayoutItemBase* layout_item = FindIdelItem();
		if (layout_item)
		{
			layout_item->Subscribe(user, RAW_DATA_TYPE_VIDEO, ZoomVideoSDKResolution_180P);
			rendering_gallery_item_list_.push_back(layout_item);
		}
	}
}

void GalleryModeVideoContainer::DeleteGalleryRenderingItem(IZoomVideoSDKUser* user)
{
	if (!user)
		return;

	for (auto iter = rendering_gallery_item_list_.begin(); iter != rendering_gallery_item_list_.end(); iter++)
	{
		VideoLayoutItemBase* item = *iter;
		if (item)
		{
			if (item->GetUser() == user)
			{
				item->unSubscribe();
				rendering_gallery_item_list_.erase(iter);
				break;
			}
		}
	}
}

void GalleryModeVideoContainer::ClearGalleryRenderingItem()
{
	for (auto iter = rendering_gallery_item_list_.begin(); iter != rendering_gallery_item_list_.end(); iter++)
	{
		VideoLayoutItemBase* item = *iter;
		if (item)
		{
			item->unSubscribe();
		}
	}
	rendering_gallery_item_list_.clear();
}

void GalleryModeVideoContainer::UpdateMapPageUserList()
{
	map_page_user_list_.clear();

	UpdatePerPageCount();

	if (per_page_count_ == 0)
		return;

	vector<IZoomVideoSDKUser*> user_list = ZoomVideoSDKMgr::GetInst().GetAllUsers();
	if (user_list.empty())
		return;

	int page = 1;
	for (size_t i = 0; i < user_list.size(); i++)
	{
		IZoomVideoSDKUser* user = user_list[i];
			
		if (!user) continue;

		map_page_user_list_[page].push_back(user);

		if ((i + 1) % per_page_count_ == 0)
			page++;
	}
}

void GalleryModeVideoContainer::ShowGallery(bool show_gallery)
{
	if (show_gallery_ == show_gallery)
		return;

	show_gallery_ = show_gallery;

	if (show_gallery)
	{
		rect_inset_ = { 0,0,(LONG)column_gap_,0 };

		UpdateMapPageUserList();
		current_page_ = 1;
		SubscribeCurrentPageUser();
	}
	else
	{
		rect_inset_ = { 0,0,0,0 };
		UnsubsrcibeAllGalleryItem();
	}

	UpdateLayoutItemPos();
}

void GalleryModeVideoContainer::SwitchShareCamera()
{
	wstring current_device_id;
	if (!active_video_layout_item_)
		return;
	active_video_layout_item_->unSubscribe();

	IVideoSDKVector<IZoomVideoSDKCameraDevice*>*  camere_list = ZoomVideoSDKMgr::GetInst().GetCameraList();
	if (!camere_list)
	{
		return;
	}
	for (size_t i = 0; i < camere_list->GetCount(); i++)
	{
		IZoomVideoSDKCameraDevice* camera_device = camere_list->GetItem(i);
		if (camera_device && camera_device->getDeviceId())
		{
			current_device_id = camera_device->getDeviceId();
		}
		
		if (!camera_device->isSelectedDevice() &&
			current_device_id != ZoomVideoSDKMgr::GetInst().GetSharingCameraID()
			)
		{
			ZoomVideoSDKMgr::GetInst().GetShareHelper()->startShare2ndCamera(current_device_id.c_str());
			ZoomVideoSDKMgr::GetInst().SetSharingCameraID(current_device_id);
			IZoomVideoSDKUser* myself = ZoomVideoSDKMgr::GetInst().GetMySelf();
			active_video_layout_item_->Subscribe(myself, RAW_DATA_TYPE_SHARE, ZoomVideoSDKResolution_1080P);
			break;
		}
	}
}
void GalleryModeVideoContainer::ShareSelectedCamera(std::wstring deviceID)
{
	wstring current_device_id;
	if (!active_video_layout_item_)
		return;
	active_video_layout_item_->unSubscribe();

	IVideoSDKVector<IZoomVideoSDKCameraDevice*>* camere_list = ZoomVideoSDKMgr::GetInst().GetCameraList();
	if (!camere_list)
	{
		return;
	}
	for (size_t i = 0; i < camere_list->GetCount(); i++)
	{
		IZoomVideoSDKCameraDevice* camera_device = camere_list->GetItem(i);
		if (camera_device && camera_device->getDeviceId())
		{
			current_device_id = camera_device->getDeviceId();
		}

		if (!camera_device->isSelectedDevice() &&
			current_device_id != ZoomVideoSDKMgr::GetInst().GetSharingCameraID() &&
			current_device_id == deviceID
			)
		{
			ZoomVideoSDKMgr::GetInst().GetShareHelper()->startShare2ndCamera(current_device_id.c_str());
			ZoomVideoSDKMgr::GetInst().SetSharingCameraID(current_device_id);
			IZoomVideoSDKUser* myself = ZoomVideoSDKMgr::GetInst().GetMySelf();
			active_video_layout_item_->Subscribe(myself, RAW_DATA_TYPE_SHARE, ZoomVideoSDKResolution_1080P);
			break;
		}
	}
}




int GalleryModeVideoContainer::GetGalleryWidth()
{
	return rect_inset_.right + ZOOM_LAYOUT_ITEM_WIDTH + column_gap_;
}

int GalleryModeVideoContainer::GetTotalPage()
{
	int current_user_count = ZoomVideoSDKMgr::GetInst().GetUserCountInSession();

	if (current_user_count == 0 || per_page_count_ == 0)
		return 0;

	float page_count = (float)current_user_count / per_page_count_;

	if (page_count > (int)page_count)
		return (int)page_count + 1;
	else
		return page_count;
}

int GalleryModeVideoContainer::GetCurPage()
{
	return current_page_;
}

void GalleryModeVideoContainer::UpdateCurGalleryPage()
{
	if (map_page_user_list_.empty())
		current_page_ = 0;

	if (rendering_gallery_item_list_.empty())
	{
		auto iter = map_page_user_list_.rbegin();
		current_page_ = iter->first;
		return;
	}

	IZoomVideoSDKUser* based_user = nullptr;
	VideoLayoutItemBase* selected_item = FindSelectedGalleryRenderingItem();
	if (selected_item)
	{
		based_user = selected_item->GetUser();
	}

	const zchar_t* first_rendering_item_user = nullptr;
	if (nullptr == based_user)
	{
		VideoLayoutItemBase* first_rendering_item = rendering_gallery_item_list_[0];
		if (first_rendering_item)
		{
			based_user = first_rendering_item->GetUser();
		}
	}

	if (nullptr == based_user)
		return;

	for (auto iter = map_page_user_list_.begin(); iter != map_page_user_list_.end(); iter++)
	{
		for (int i = 0; i < iter->second.size(); i++)
		{
			if (nullptr == iter->second[i]) continue;

			if (based_user == iter->second[i])
			{
				current_page_ = iter->first;
				break;
			}
		}
	}
}

void GalleryModeVideoContainer::SubscribeCurrentPageUser()
{
	if (!active_video_layout_item_)
		return;

	IZoomVideoSDKUser* active_user = active_video_layout_item_->GetUser();



	auto iter = map_page_user_list_.find(current_page_);
	if (iter != map_page_user_list_.end())
	{
		ClearGalleryRenderingItem();
		vector<IZoomVideoSDKUser*> page_user_list = iter->second;
		for (int i = 0; i < page_user_list.size(); i++)
		{
			IZoomVideoSDKUser* user = page_user_list[i];
			if (!user) return;

			VideoLayoutItemBase* item = FindIdelItem();
			if (item)
			{
			
				item->Subscribe(user, RAW_DATA_TYPE_VIDEO, ZoomVideoSDKResolution_180P);
		

				LowerThirdInfo lower_third_info = CMainFrame::GetInstance().GetUserLowerThirdInfo(user);
				LowerThirdColorInfo color_info = CMainFrame::GetInstance().GetLowerThirdsColorInfo(lower_third_info.lower_third_color);
				item->SetLowerThirdInfo(lower_third_info.lower_third_name, lower_third_info.lower_third_description, color_info);
								
				if (!item->IsIdle())
				{
					if (active_user == user)
					{
						item->set_is_selected(true);
					}
					rendering_gallery_item_list_.push_back(item);
				}
			}
		}
	}
}

void GalleryModeVideoContainer::FillRenderingItem()
{
	if (!active_video_layout_item_)
		return;

	IZoomVideoSDKUser* active_user = active_video_layout_item_->GetUser();

	auto iter = map_page_user_list_.find(current_page_);
	if (iter != map_page_user_list_.end())
	{
		vector<IZoomVideoSDKUser*> page_user_list = iter->second;

		for (int i = 0; i < page_user_list.size(); i++)
		{
			IZoomVideoSDKUser* user = page_user_list[i];
			if (!user) return;

			VideoLayoutItemBase* item_rendering = FindRenderingItemByUser(user);
			if (!item_rendering && per_page_count_ > rendering_gallery_item_list_.size()) //not find
			{
				VideoLayoutItemBase* item = FindIdelItem();
				if (item)
				{
					item->Subscribe(user, RAW_DATA_TYPE_VIDEO, ZoomVideoSDKResolution_180P);
					if (!item->IsIdle())
					{
						if (active_user == user)
						{
							item->set_is_selected(true);
						}
						rendering_gallery_item_list_.push_back(item);
					}
				}
			}
		}
	}
}

VideoLayoutItemBase* GalleryModeVideoContainer::FindRenderingItemByUser(IZoomVideoSDKUser* user)
{
	if (!user) return nullptr;

	for (auto iter = rendering_gallery_item_list_.begin(); iter != rendering_gallery_item_list_.end(); iter++)
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

bool GalleryModeVideoContainer::IsClickedOnGalleryRenderingItem(POINT pt)
{
	for (auto iter = rendering_gallery_item_list_.begin(); iter != rendering_gallery_item_list_.end(); iter++)
	{
		VideoLayoutItemBase* item = *iter;

		if (!item) continue;

		RendererRect rect_item = item->GetPos();
		CRect rect = { (int)rect_item.left,(int)rect_item.top,(int)(rect_item.left + rect_item.width),(int)(rect_item.top + rect_item.height) };
		if (rect.PtInRect(pt))
		{
			return true;
		}
	}

	return false;
}

VideoLayoutItemBase* GalleryModeVideoContainer::FindSelectedGalleryRenderingItem()
{
	for (auto iter = rendering_gallery_item_list_.begin(); iter != rendering_gallery_item_list_.end(); iter++)
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

void GalleryModeVideoContainer::TurnUpPage()
{
	OnTurnPageUpdateGalleryRenderingItemList(true);
}

void GalleryModeVideoContainer::TurnDownPage()
{
	OnTurnPageUpdateGalleryRenderingItemList(false);
}

void GalleryModeVideoContainer::OnLButtonUp(POINT pt)
{
	//if not clicked on rendering gallery item,do nothing
	if (!IsClickedOnGalleryRenderingItem(pt))
		return;

	for (auto iter = rendering_gallery_item_list_.begin(); iter != rendering_gallery_item_list_.end(); iter++)
	{
		VideoLayoutItemBase* item = *iter;

		if (!item) continue;

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

	if (!active_video_layout_item_) return;

	IZoomVideoSDKUser* active_user = nullptr;
	IZoomVideoSDKUser* selected_user = nullptr;

	active_user = active_video_layout_item_->GetUser();

	VideoLayoutItemBase* selected_item = FindSelectedGalleryRenderingItem();
	if (selected_item)
	{
		selected_user = selected_item->GetUser();
	}

	//click more button logic
	ZoomVideoSDKErrors err = ZoomVideoSDKErrors_Unknown;
	IZoomVideoSDKUser* myself = ZoomVideoSDKMgr::GetInst().GetMySelf();
	if (selected_user != myself)
	{
		RendererRect rect_selected_item = selected_item->GetPos();
		CRect rect_selected = { (int)(rect_selected_item.left + rect_selected_item.width - MORE_BTN_LEFT_MARGIN),(int)rect_selected_item.top + MORE_BTN_TOP_MARGIN,(int)(rect_selected_item.left + rect_selected_item.width - MORE_BTN_TOP_MARGIN),(int)(rect_selected_item.top + MORE_BTN_LEFT_MARGIN) };
		if (rect_selected.PtInRect(pt))
		{
			POINT pt_temp = { rect_selected.left, rect_selected.top };
			ClientToScreen(CMainFrame::GetInstance().GetHWND(), &pt_temp);

			ZoomVideoSDKMgr::GetInst().SetRemoteControlCameraUser(selected_user);
		
		}
	}

	if (selected_user && active_user != selected_user)
	{
		active_video_layout_item_->unSubscribe();

		ZoomVideoSDKRawDataType data_type = RAW_DATA_TYPE_VIDEO;

		if (myself && selected_user == myself)
		{
			ZoomVideoSDKShareStatus status = ZoomVideoSDKShareStatus_None;

			IVideoSDKVector<IZoomVideoSDKShareAction*>* pShareActionList = myself->getShareActionList();
			if (pShareActionList && pShareActionList->GetCount() > 0)
			{
				IZoomVideoSDKShareAction* pShareAction = pShareActionList->GetItem(0);
				if (pShareAction && pShareAction->getSharePipe())
					status = pShareAction->getSharePipe()->getShareStatus();
			}

			if (status == ZoomVideoSDKShareStatus_Start || status == ZoomVideoSDKShareStatus_Resume)
			{
	
				return;
			}
			
		}

		if (myself && selected_user != myself) //when sharing, if not myself active video show share,otherwise always show video.
		{
			ZoomVideoSDKShareStatus status = ZoomVideoSDKShareStatus_None;
			IVideoSDKVector<IZoomVideoSDKShareAction*>* pShareActionList = selected_user->getShareActionList();
			if (pShareActionList && pShareActionList->GetCount() > 0)
			{
				IZoomVideoSDKShareAction* pShareAction = pShareActionList->GetItem(0);
				if (pShareAction && pShareAction->getSharePipe())
					status = pShareAction->getSharePipe()->getShareStatus();
			}
			
			if (status == ZoomVideoSDKShareStatus_Start || status == ZoomVideoSDKShareStatus_Resume)
			{
				data_type = RAW_DATA_TYPE_SHARE;
			}
		}

		active_video_layout_item_->Subscribe(selected_user, data_type, ZoomVideoSDKResolution_1080P);
		

		/*CMainFrame::GetInstance().OnGalleryContainerSelectUserChange();*/
	}
}

void GalleryModeVideoContainer::OnUserJoin(IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (!userList) return;

	if (show_gallery_)
	{
		OnUserJoinUpdateGalleryRenderingItemList(userList);
	}
	else
	{
		if (userList->GetCount() > 0)
		{
			IZoomVideoSDKUser* join_user = userList->GetItem(0);
			if (active_video_layout_item_ && active_video_layout_item_->IsIdle())
			{
				active_video_layout_item_->Subscribe(join_user, RAW_DATA_TYPE_VIDEO, ZoomVideoSDKResolution_1080P);
			}
		}
	}

	UpdateLayoutItemPos();
}

void GalleryModeVideoContainer::OnUserLeave(IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (!userList) return;

	if (!active_video_layout_item_) return;

	if (show_gallery_)
	{
		OnUserLeaveUpdateGalleryRenderingItemList(userList);
	}

	for (int i = 0; i < userList->GetCount(); i++)
	{
		IZoomVideoSDKUser* leave_user = userList->GetItem(i);
		IZoomVideoSDKUser* active_user = active_video_layout_item_->GetUser();
		bool active_user_leave = false;
		if (active_user && leave_user)
		{
			if (active_user == leave_user) //active user leave
			{
				active_user_leave = true;
			}
		}

		if (show_gallery_)
		{
			if (active_user_leave && !rendering_gallery_item_list_.empty())
			{
				auto iter = rendering_gallery_item_list_.begin(); //subcribe the remain first rendering item.
				VideoLayoutItemBase* item = *iter;
				if (item)
				{
					item->set_is_selected(true);
					active_video_layout_item_->unSubscribe();

					IZoomVideoSDKUser* user = item->GetUser();
					if (user)
					{
						active_video_layout_item_->Subscribe(user, RAW_DATA_TYPE_VIDEO, ZoomVideoSDKResolution_1080P);
					}
				}
			}
		}
		else
		{
			if (active_user_leave) //active user leave
			{
				vector<IZoomVideoSDKUser*> user_list = ZoomVideoSDKMgr::GetInst().GetAllUsers();

				if (user_list.size() > 0)
				{
					IZoomVideoSDKUser* user = user_list[0];
					active_video_layout_item_->unSubscribe();
					active_video_layout_item_->Subscribe(user, RAW_DATA_TYPE_VIDEO, ZoomVideoSDKResolution_1080P);
				}
			}
		}
	}

	UpdateLayoutItemPos();
}

void GalleryModeVideoContainer::OnUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type)
{
	VideoContainerBase::OnUserShareStatusChanged(pShareHelper,pUser,status, type);

	if (!active_video_layout_item_)
		return;

	if (!pUser)
		return;

	IZoomVideoSDKUser* myself = ZoomVideoSDKMgr::GetInst().GetMySelf();
		
	if (pUser == myself)
	{
		active_video_layout_item_->unSubscribe();

		if (status == ZoomVideoSDKShareStatus_Start || status == ZoomVideoSDKShareStatus_Resume)
		{
			active_video_layout_item_->Subscribe(pUser, RAW_DATA_TYPE_SHARE, ZoomVideoSDKResolution_1080P);
		}
		else
		{
			active_video_layout_item_->Subscribe(pUser, RAW_DATA_TYPE_VIDEO, ZoomVideoSDKResolution_1080P);
		}
	}
}