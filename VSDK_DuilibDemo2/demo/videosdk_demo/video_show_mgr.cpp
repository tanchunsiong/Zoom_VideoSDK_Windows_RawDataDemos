#include "stdafx.h"
#include "video_show_mgr.h"
#include "gallary_mode_video_container.h"
#include "video_layout_item_base.h"
#include "videosdk_demo_mgr.h"

VideoShowMgr::VideoShowMgr(IZoomVideoSDKRenderer* renderer, RendererRect rect_renderer):renderer_(renderer), 
                                                                             cur_layout_type_(kVideoRenderLayoutTypeNone), rect_renderer_(rect_renderer)
{

}

VideoShowMgr::~VideoShowMgr()
{
	if (video_container_)
	{
		delete video_container_;
		video_container_ = nullptr;
	}
}

void VideoShowMgr::UnsubscribeAllItem()
{
	if (video_container_)
	{
		video_container_->UnsubscribeAllItem();
	}
}

VideoRenderLayoutType VideoShowMgr::GetCurrentLayoutType()
{
	return cur_layout_type_;
}

int VideoShowMgr::GetGalleryContainerGalleryWidth()
{
	GalleryModeVideoContainer* video_container = dynamic_cast<GalleryModeVideoContainer*>(video_container_);
	if (video_container)
	{
		return video_container->GetGalleryWidth();
	}

	return 0;
}

int VideoShowMgr::GetGalleryContainerTotalPage()
{
	GalleryModeVideoContainer* video_container = dynamic_cast<GalleryModeVideoContainer*>(video_container_);
	if (video_container)
	{
		return video_container->GetTotalPage();
	}

	return 0;
}

int VideoShowMgr::GetGalleryContainerCurPage()
{
	GalleryModeVideoContainer* video_container = dynamic_cast<GalleryModeVideoContainer*>(video_container_);
	if (video_container)
	{
		return video_container->GetCurPage();
	}

	return 0;
}

VideoLayoutItemBase* VideoShowMgr::GetGalleryContainerSelectItem()
{
	GalleryModeVideoContainer* video_container = dynamic_cast<GalleryModeVideoContainer*>(video_container_);
	if (video_container)
	{
		return video_container->FindSelectedItem();
	}
	return NULL;
}

void VideoShowMgr::OnUserJoin(IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (video_container_)
	{
		video_container_->OnUserJoin(userList);
	}
}

void VideoShowMgr::OnUserLeave(IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (video_container_)
	{
		video_container_->OnUserLeave(userList);
	}
}

void VideoShowMgr::OnSessionLeave()
{
	if (video_container_)
	{
		delete video_container_;
		video_container_ = nullptr;
	}

	cur_layout_type_ = kVideoRenderLayoutTypeNone;
}

void VideoShowMgr::OnUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type)
{
	if (video_container_)
	{
		video_container_->OnUserShareStatusChanged(pShareHelper, pUser, status, type);
	}
}

void VideoShowMgr::OnUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (video_container_)
	{
		IZoomVideoSDKUser* user = nullptr;
		int count = userList->GetCount();
		for (int i = 0; i < count; i++)
		{
			user = userList->GetItem(i);
			video_container_->OnUserAudioStatusChanged(pAudioHelper,user);
		}
	}
}



void VideoShowMgr::OnUpdateRenderModeLowerThird(IZoomVideoSDKUser* pUser, wstring lower_third_name, wstring lower_third_description, LowerThirdColorInfo& color_info)
{
	if (video_container_)
	{
		video_container_->OnUpdateRenderModeLowerThird(pUser, lower_third_name, lower_third_description, color_info);
	}
}

void VideoShowMgr::OnSize(RendererRect rect)
{
	rect_renderer_ = rect;

	if (video_container_)
	{
		video_container_->OnSize(rect);
	}
}

void VideoShowMgr::OnLButtonUp(POINT pt)
{
	if (video_container_)
	{
		video_container_->OnLButtonUp(pt);
	}
}

void VideoShowMgr::OnMouseMove(POINT pt)
{
	if (video_container_)
	{
		video_container_->OnMouseMove(pt);
	}
}

void VideoShowMgr::SwitchVideoRenderLayoutType(const VideoContainerParam& param)
{
	if (cur_layout_type_ == param.layout_type)
		return;
	cur_layout_type_ = param.layout_type;

	if (video_container_)
	{
		delete video_container_;
		video_container_ = nullptr;
	}

	if (nullptr == video_container_)
	{
		if (param.layout_type == kVideoRenderLayoutType1x6)
		{
			video_container_ = new GalleryModeVideoContainer(renderer_);
		}
	}

	if (video_container_)
	{
		video_container_->SetInset(param.rect_inset);
		video_container_->SetRowAndColumnGap(param.row_gap, param.column_gap);
		video_container_->SetPos(rect_renderer_);
		video_container_->SetRenderMode(param.mode);
		video_container_->SwitchVideoRenderLayoutType(param.layout_type);
	}
}

void VideoShowMgr::SubscribeAllUser()
{
	if (video_container_)
	{
		video_container_->SubscribeAllUser();
	}
}

void VideoShowMgr::Subscribe(IZoomVideoSDKUser* user, ZoomVideoSDKRawDataType dataType)
{
	if (video_container_)
	{
		video_container_->Subscribe(user,dataType);
	}
}

void VideoShowMgr::AddRendererVideoItem(IZoomVideoSDKUser* user)
{
	if (video_container_)
	{
		video_container_->AddRendererVideoItem(user);
	}
}

void VideoShowMgr::AddUserAvatarIcon(IZoomVideoSDKUser* user)
{
	if (video_container_)
	{
		video_container_->AddUserAvatarIcon(user);
	}
}

void VideoShowMgr::ContainerInit()
{
	if (video_container_)
	{
		video_container_->Init();
	}
}

void VideoShowMgr::SwitchRenderMode(RendererMode mode)
{
	if (video_container_)
	{
		video_container_->SwitchRenderMode(mode);
	}
}

void VideoShowMgr::SwitchShareCamera()
{
	GalleryModeVideoContainer* video_container = dynamic_cast<GalleryModeVideoContainer*>(video_container_);
	if (video_container)
	{
		video_container->SwitchShareCamera();
	}
}
void VideoShowMgr::ShareSelectedCamera(std::wstring deviceID)
{
	GalleryModeVideoContainer* video_container = dynamic_cast<GalleryModeVideoContainer*>(video_container_);
	if (video_container)
	{
		video_container->ShareSelectedCamera(deviceID);
	}
}

void VideoShowMgr::OnGalleryContainerShowGallery(bool show_gallery)
{
	GalleryModeVideoContainer* video_container = dynamic_cast<GalleryModeVideoContainer*>(video_container_);
	if (video_container)
	{
		video_container->ShowGallery(show_gallery);
	}
}

void VideoShowMgr::OnTurnUpPage()
{
	GalleryModeVideoContainer* video_container = dynamic_cast<GalleryModeVideoContainer*>(video_container_);
	if (video_container)
	{
		video_container->TurnUpPage();
	}
}

void VideoShowMgr::OnTurnDownPage()
{
	GalleryModeVideoContainer* video_container = dynamic_cast<GalleryModeVideoContainer*>(video_container_);
	if (video_container)
	{
		video_container->TurnDownPage();
	}
}