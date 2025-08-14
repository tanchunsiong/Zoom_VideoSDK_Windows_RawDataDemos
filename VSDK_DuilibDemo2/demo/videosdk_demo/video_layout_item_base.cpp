#include "stdafx.h"
#include "video_layout_item_base.h"
#include "zoom_videosdk_renderer_interface.h"
#include "videosdk_demo_mgr.h"
#include "active_video_layout_item.h"
#include "wall_mode_video_layout_item.h"
#include "main_frame.h"

#define USER_NAME_LEFT_MARGIN 10
#define USER_NAME_BOTTOM_MARGIN 5
#define TEXT_LEFT_MARGIN 5
#define TEXT_TOP_MARGIN 2
#define REACTION_LEFT_MARGIN 0
#define REACTION_TOP_MARGIN 0
#define LOWER_THIRD_LEFT_MARGIN 0
#define LOWER_THIRD_TOP_MARGIN 0

VideoLayoutItemBase::VideoLayoutItemBase(IZoomVideoSDKRenderer* renderer) :renderer_(renderer), renderer_item_handle_(0), user_(nullptr),
raw_data_type_(RAW_DATA_TYPE_VIDEO), mode_(kVideoSDKRenderMode_AspectRatio),
user_has_subscribed_(false), user_has_hand_up_(false), lower_third_name_(_T("")), lower_third_description_(_T(""))
{
	lower_third_font_color_ = Gdiplus::Color(255, 255, 255);
	lower_third_font_bk_color_ = Gdiplus::Color(68, 75, 83, 1);
	user_emoji_resid_ = 0;
	emoji_middle_point_ = { 0, 0 };
}

VideoLayoutItemBase::~VideoLayoutItemBase()
{
	Clean();
	renderer_ = nullptr;
}

void VideoLayoutItemBase::Clean()
{
	RemoveRendererVideoItem();
	user_ = nullptr;
	user_has_subscribed_ = false;
}

void VideoLayoutItemBase::SetPos(RendererRect rect)
{
	rect_ = rect;

	if (renderer_)
	{
		renderer_->ModifyLayoutItemRect(renderer_item_handle_, rect);
		UpdateLayeredElementRect();

	}
}

RendererRect VideoLayoutItemBase::GetPos() const
{
	return rect_;
}

void VideoLayoutItemBase::SwitchRenderMode(RendererMode mode)
{
	mode_ = mode;
	if (renderer_item_handle_ && renderer_)
	{
		renderer_->ModifyLayoutItemRenderMode(renderer_item_handle_, mode_);
	}
}

RendererMode VideoLayoutItemBase::GetRenderMode() const
{
	return mode_;
}

void VideoLayoutItemBase::AddRendererVideoItem(IZoomVideoSDKUser* user)
{
	user_ = user;

	if (renderer_item_handle_ == 0)
	{
		if (renderer_)
		{
			renderer_item_handle_ = renderer_->AddLayoutItem(rect_, mode_);
		}
	}
}

void VideoLayoutItemBase::RemoveRendererVideoItem()
{
	if (renderer_item_handle_)
	{
		if (renderer_)
		{
			renderer_->RemoveLayoutItem(renderer_item_handle_);
		}
		renderer_item_handle_ = 0;
		map_layed_element_.clear();
	}
}

void VideoLayoutItemBase::Subscribe(IZoomVideoSDKUser* pUser, ZoomVideoSDKRawDataType dataType, ZoomVideoSDKResolution eResolution)
{
	if (!pUser)
		return;

	ZoomVideoSDKErrors err = ZoomVideoSDKErrors_Unknown;
	IZoomVideoSDKRawDataPipe* pPipe = nullptr;

	if (dataType == RAW_DATA_TYPE_VIDEO)
	{
		pPipe = pUser->GetVideoPipe();
	}
	else if (dataType == RAW_DATA_TYPE_SHARE)
	{
		IVideoSDKVector<IZoomVideoSDKShareAction*>* pShareActionList = pUser->getShareActionList();
		if (pShareActionList && pShareActionList->GetCount() > 0)
		{
			IZoomVideoSDKShareAction* pShareAction = pShareActionList->GetItem(0);
			if (pShareAction)
				pPipe = pShareAction->getSharePipe();
		}
	}

	if (pPipe)
	{
		err = pPipe->subscribe(eResolution, this);
	}

	if (err == ZoomVideoSDKErrors_Success)
	{
		AddRendererVideoItem(pUser);
		raw_data_type_ = dataType;
		user_has_subscribed_ = true;
	}
	else if (err == ZoomVideoSDKErrors_RAWDATA_VIDEO_DEVICE_ERROR)
	{
		AddRendererVideoItem(pUser);
		AddUserAvatarIcon();
	}
}

void VideoLayoutItemBase::unSubscribe()
{
	if (!user_)
	{
		return;
	}

	ZoomVideoSDKErrors err = ZoomVideoSDKErrors_Unknown;

	switch (raw_data_type_)
	{
	case RAW_DATA_TYPE_VIDEO:
	{
		if (user_->GetVideoPipe())
		{
			err = user_->GetVideoPipe()->unSubscribe(this);
		}
	}
	break;
	case RAW_DATA_TYPE_SHARE:
	{
		IVideoSDKVector<IZoomVideoSDKShareAction*>* pShareActionList = user_->getShareActionList();
		if (pShareActionList && pShareActionList->GetCount() > 0)
		{
			IZoomVideoSDKShareAction* pShareAction = pShareActionList->GetItem(0);
			if (pShareAction)
			{
				IZoomVideoSDKRawDataPipe* pPipe = pShareAction->getSharePipe();

				if (pPipe)
				{
					err = pPipe->unSubscribe(this);
				}
			}
		}
		
	}
	break;
	default:
		break;
	}

	Clean();
}


void VideoLayoutItemBase::onRawDataFrameReceived(YUVRawDataI420* data)
{
	if (!renderer_ || !renderer_item_handle_ || !data) return;

	renderer_->Draw(renderer_item_handle_, data);
}

bool VideoLayoutItemBase::GetUserHasSubscribed()
{
	return user_has_subscribed_;
}

bool VideoLayoutItemBase::GetUserHasHandUp()
{
	return user_has_hand_up_;
}

void VideoLayoutItemBase::SetUserHasHandUp(bool is_raise_hand)
{
	user_has_hand_up_ = is_raise_hand;
}



void VideoLayoutItemBase::SetLowerThirdInfo(wstring lower_third_name, wstring lower_third_description, LowerThirdColorInfo& color_info)
{
	if (lower_third_name.empty() && user_)
		lower_third_name_ = user_->getUserName();
	else
		lower_third_name_ = lower_third_name;

	lower_third_description_ = lower_third_description;
	lower_third_font_color_ = color_info.gdi_font_color;
	lower_third_font_bk_color_ = color_info.gdi_font_bk_color;
	lower_third_color_ = color_info.bkcolor;

	UpdateLowerThird();
}

void VideoLayoutItemBase::Trim(wstring& str) //Remove space at both ends
{
	if (str.empty())
		return;

	str.erase(0, str.find_first_not_of(_T(" ")));
	str.erase(str.find_last_not_of(_T(" ")) + 1);
}

wstring VideoLayoutItemBase::CalUserAvatar(const wstring& user_name)
{
	if (user_name.empty())
		return _T("");
	wstring user_avatar;
	wstring str_temp = user_name;
	Trim(str_temp);
	user_avatar += str_temp[0];

	int npos = str_temp.find_first_of(_T(" "));
	if (npos != -1)
	{
		str_temp = str_temp.substr(npos + 1, str_temp.length());
		Trim(str_temp);
		if (!str_temp.empty())
			user_avatar += str_temp[0];
	}
	else
	{
		if (str_temp.size() >= 2)
			user_avatar += str_temp[1];
	}
	return user_avatar;
}

void VideoLayoutItemBase::AddUserAvatarIcon()
{
	if (!renderer_ || !renderer_item_handle_)
		return;

	if (!user_)
		return;

	auto iter = map_layed_element_.find(_T("UserAvatar"));
	if (iter != map_layed_element_.end())
	{
		return;
	}

	wstring user_name = user_->getUserName();

	wstring user_avatar = CalUserAvatar(user_name);

	Gdiplus::RectF font_bk_rect = { 0, 0, 0, 0 };
	uint32_t font_size = 20;
	if (dynamic_cast<ActiveVideoLayoutItem*>(this))
	{
		font_bk_rect = { 0,0,150,150 };
		font_size = 40;
	}
	else
	{
		font_bk_rect = { 0,0,60,60 };
		font_size = 18;
	}

	StringBMPParam string_param;
	string_param.text = user_avatar;
	string_param.font_size = font_size;
	string_param.font_type = _T("Microsoft YaHei");
	string_param.font_color = Gdiplus::Color(255, 255, 255);
	string_param.font_bk_color = { 255,76,77,77 };
	string_param.font_bk_rect = font_bk_rect;
	Gdiplus::Bitmap* bitmap = CreateBMPFromString(string_param, rect_.width);
	if (!bitmap)
		return;

	Gdiplus::BitmapData bitmap_data;
	GetImageBitmapData(bitmap, bitmap_data);

	PixelsData pixels_data;
	pixels_data.width = bitmap_data.Width;
	pixels_data.height = bitmap_data.Height;
	pixels_data.pixels = (uint8_t*)bitmap_data.Scan0;
	pixels_data.pitch = bitmap_data.Stride;

	RendererRect rect;
	rect.width = bitmap_data.Width;
	rect.height = bitmap_data.Height;
	AddLayeredImageElement(_T("UserAvatar"), bitmap_data, rect, 2);

	bitmap->UnlockBits(&bitmap_data);
	delete bitmap;
	bitmap = nullptr;
}

void VideoLayoutItemBase::DeleteUserAvatarIcon()
{
	if (!renderer_)
		return;

	auto iter = map_layed_element_.find(_T("UserAvatar"));
	if (iter != map_layed_element_.end())
	{
		renderer_->RemoveLayeredElement(iter->second);
		map_layed_element_.erase(iter);
	}
}

void VideoLayoutItemBase::onRawDataStatusChanged(RawDataStatus status)
{
	if (!renderer_ || !renderer_item_handle_)
		return;


	if (status == RawData_On)
	{
		DeleteUserAvatarIcon();
	}
	else
	{
		renderer_->ClearLyaoutItemVideoData(renderer_item_handle_);
		AddUserAvatarIcon();
	}
}

bool VideoLayoutItemBase::IsIdle()
{
	if (renderer_item_handle_)
	{
		return false;
	}

	return true;
}



void VideoLayoutItemBase::set_is_selected(bool is_selected)
{
	if (renderer_ && renderer_item_handle_)
	{
		renderer_->ModifyLayoutItmeSelected(renderer_item_handle_, is_selected);
	}

	CMainFrame::GetInstance().OnGalleryContainerSelectUserChange();
}

bool VideoLayoutItemBase::get_is_selected() const
{
	if (renderer_ && renderer_item_handle_)
	{
		return renderer_->IsLayoutItemSelected(renderer_item_handle_);
	}

	return false;
}

void VideoLayoutItemBase::SetRenderMode(RendererMode mode)
{
	mode_ = mode;
}

IZoomVideoSDKUser* VideoLayoutItemBase::GetUser() const
{
	return user_;
}

void VideoLayoutItemBase::OnShareStatusChange()
{
	UpdateRenderUserName();
}

void VideoLayoutItemBase::OnAudioStatusChange()
{
	//UpdateAudioUserName();
	UpdateLowerThird();
}



void VideoLayoutItemBase::OnLowerThirdChange()
{
	UpdateLowerThird();
}



Gdiplus::Bitmap* VideoLayoutItemBase::CreateBMPFromString(const StringBMPParam& string_param, int max_width, int muted_width)
{
	if (rect_.width == 0 || rect_.height == 0)
		return nullptr;

	Gdiplus::FontFamily fontFamily(string_param.font_type.c_str());
	Gdiplus::Font font(&fontFamily, string_param.font_size, Gdiplus::FontStyleRegular);
	Gdiplus::SolidBrush solidBrushFont(string_param.font_color);
	HDC hDisplayDC = CreateICW(_T("DISPLAY"), NULL, NULL, NULL);
	HDC hDC = ::CreateCompatibleDC(hDisplayDC);

	//calculte bitmap width and height
	Gdiplus::Graphics graphics(hDC);
	//Smoothing font edges
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAlias);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	graphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
	graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

	//Calculates the size of the string
	Gdiplus::RectF boundingBox(0, 0, 0, 0);
	graphics.MeasureString(string_param.text.c_str(), -1, &font, Gdiplus::PointF(0, 0), &boundingBox);

	int bitmap_width = 0, bitmap_height = 0;
	//create bitmap
	Gdiplus::SizeF font_sizef;
	boundingBox.GetSize(&font_sizef);

	if (!string_param.font_bk_rect.IsEmptyArea())
	{
		bitmap_width = string_param.font_bk_rect.Width;
		bitmap_height = string_param.font_bk_rect.Height;
	}
	else
	{
		//bitmap_width = muted_width + (font_sizef.Width + 1) + TEXT_LEFT_MARGIN * 2;
		bitmap_width = rect_.width;
		bitmap_height = (font_sizef.Height + 1) + TEXT_TOP_MARGIN * 2;
	}

	//if (bitmap_width > max_width - 2 * USER_NAME_LEFT_MARGIN)
	//	bitmap_width = max_width - 2 * USER_NAME_LEFT_MARGIN;
	if (bitmap_width > max_width)
		bitmap_width = max_width;

	graphics.ReleaseHDC(hDC);
	DeleteDC(hDisplayDC);
	DeleteDC(hDC);

	//create bitmap and draw string to bitmap
	Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(bitmap_width, bitmap_height, PixelFormat32bppARGB);
	Gdiplus::Graphics g(bitmap);

	Gdiplus::StringFormat string_format;

	if (!string_param.font_bk_rect.IsEmptyArea())
	{
		string_format.SetAlignment(Gdiplus::StringAlignmentCenter);
		string_format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	}
	else
	{
		string_format.SetAlignment(Gdiplus::StringAlignmentNear);
		string_format.SetLineAlignment(Gdiplus::StringAlignmentNear);
	}

	string_format.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);

	//Smoothing font edges
	g.SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAlias);
	g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	g.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
	g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

	Gdiplus::RectF font_bk_rect = { 0,0,(REAL)rect_.width,(REAL)bitmap->GetHeight() + TEXT_LEFT_MARGIN * 2 };
	if (!string_param.font_bk_rect.IsEmptyArea())
	{
		font_bk_rect = { 0,0,(REAL)bitmap->GetWidth(),(REAL)bitmap->GetHeight() + TEXT_LEFT_MARGIN };
	}
	//Gdiplus::RectF font_bk_rect = { 0,0,(REAL)bitmap->GetWidth(),(REAL)bitmap->GetHeight() + TEXT_LEFT_MARGIN };
	
	Gdiplus::SolidBrush solidBrush(string_param.font_bk_color);
	g.FillRectangle(&solidBrush, font_bk_rect);

	//draw text to bitmap
	Gdiplus::PointF pt;
	Gdiplus::RectF font_rect;
	if (!string_param.font_bk_rect.IsEmptyArea())
	{
		pt.X = string_param.font_bk_rect.GetLeft();
		pt.Y = string_param.font_bk_rect.GetTop();
		font_rect = { pt.X ,pt.Y,font_bk_rect.Width,font_bk_rect.Height };
	}
	else
	{
		pt.X = muted_width + font_bk_rect.GetLeft() + TEXT_LEFT_MARGIN;		
		pt.Y = font_bk_rect.GetTop() + TEXT_TOP_MARGIN;
		//font_rect = { pt.X ,pt.Y,font_bk_rect.Width - TEXT_LEFT_MARGIN * 2,font_bk_rect.Height - TEXT_TOP_MARGIN * 2 };
		font_rect = { pt.X ,pt.Y,font_bk_rect.Width,font_bk_rect.Height - TEXT_TOP_MARGIN * 2 };
	}
	g.DrawString(string_param.text.c_str(), -1, &font, font_rect,
		&string_format, &solidBrushFont);

	return bitmap;
}

Gdiplus::Bitmap* VideoLayoutItemBase::CreateBMPFromLowerThirdString(const StringBMPParam& string_param, int max_width)
{
	if (rect_.width == 0 || rect_.height == 0)
		return nullptr;

	Gdiplus::FontFamily fontFamily(string_param.font_type.c_str());
	Gdiplus::Font font(&fontFamily, string_param.font_size, Gdiplus::FontStyleRegular);
	Gdiplus::Font font_name(&fontFamily, string_param.font_lower_third_size, Gdiplus::FontStyleBold);
	Gdiplus::SolidBrush solidBrushFont( string_param.font_color);
	HDC hDisplayDC = CreateICW(_T("DISPLAY"), NULL, NULL, NULL);
	HDC hDC = ::CreateCompatibleDC(hDisplayDC);

	//calculte bitmap width and height
	Gdiplus::Graphics graphics(hDC);
	//Smoothing font edges
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAlias);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	graphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
	graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

	//Calculates the size of the string
	Gdiplus::RectF boundingBox(0, 0, 0, 0);
	//graphics.MeasureString(string_param.text.c_str(), -1, &font_name, Gdiplus::PointF(0, 0), &boundingBox);
	graphics.MeasureString(string_param.text.c_str(), -1, &font, Gdiplus::PointF(0, 0), &boundingBox);
	
	int bitmap_width = 0, bitmap_height = 0;
	//create bitmap
	Gdiplus::SizeF font_sizef;
	boundingBox.GetSize(&font_sizef);

	if (!string_param.font_bk_rect.IsEmptyArea())
	{
		bitmap_width = string_param.font_bk_rect.Width;
		bitmap_height = string_param.font_bk_rect.Height;
	}
	else
	{
		bitmap_width = max_width;
		int height_factor = 1;
		if (!lower_third_description_.empty())
			height_factor++;

		if (user_)
		{
			ZoomVideoSDKAudioStatus audio_status = user_->getAudioStatus();
			if (audio_status.isMuted)
				height_factor++;
		}
		
		bitmap_height = (font_sizef.Height * height_factor + 1) + TEXT_TOP_MARGIN * 2;
	}

	if (bitmap_width > max_width)
		bitmap_width = max_width;

	graphics.ReleaseHDC(hDC);

	//create bitmap and draw string to bitmap
	Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(bitmap_width, bitmap_height, PixelFormat32bppARGB);
	Gdiplus::Graphics g(bitmap);

	Gdiplus::StringFormat string_format;
	string_format.SetAlignment(Gdiplus::StringAlignmentNear);
	string_format.SetLineAlignment(Gdiplus::StringAlignmentNear);
	string_format.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);

	//Smoothing font edges
	g.SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAlias);
	g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	g.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
	g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

	Gdiplus::RectF font_bk_rect = { 0,0,(REAL)bitmap->GetWidth(),(REAL)bitmap->GetHeight() };
	Gdiplus::SolidBrush solidBrush(lower_third_font_bk_color_);
	g.FillRectangle(&solidBrush, font_bk_rect);

	//draw text to bitmap
	Gdiplus::PointF pt;
	Gdiplus::RectF font_rect;
	if (!string_param.font_bk_rect.IsEmptyArea())
	{
		pt.X = string_param.font_bk_rect.GetLeft();
		pt.Y = string_param.font_bk_rect.GetTop();
		font_rect = { pt.X ,pt.Y,font_bk_rect.Width,font_bk_rect.Height };
	}
	else
	{
		pt.X = font_bk_rect.GetLeft() + TEXT_LEFT_MARGIN - 1;
		pt.Y = font_bk_rect.GetTop();
		font_rect = { pt.X ,pt.Y,font_bk_rect.Width - TEXT_LEFT_MARGIN * 2,font_bk_rect.Height - TEXT_TOP_MARGIN * 2 };
	}
	
	wstring third_name = lower_third_name_;

	if (third_name.empty())
	{
		if (!user_)
			third_name = string_param.text;
		else
			third_name = user_->getUserName();
	}

	g.DrawString(third_name.c_str(), -1, &font_name, font_rect,
		&string_format, &solidBrushFont);

	if (!lower_third_description_.empty())
	{
		pt.X = font_bk_rect.GetLeft() + TEXT_LEFT_MARGIN;
		font_rect = { pt.X ,font_sizef.Height ,font_bk_rect.Width - TEXT_LEFT_MARGIN * 2,font_bk_rect.Height - TEXT_TOP_MARGIN * 2 };

		g.DrawString(lower_third_description_.c_str(), -1, &font, font_rect,
			&string_format, &solidBrushFont);
	}
	
	return bitmap;
}

void VideoLayoutItemBase::GetImageBitmapData(Gdiplus::Bitmap* image, Gdiplus::BitmapData& bitmap_data)
{
	if (!image) return;

	Gdiplus::Rect bmpRect(0, 0, image->GetWidth(), image->GetHeight());
	image->LockBits(&bmpRect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmap_data);
}

void VideoLayoutItemBase::AddLayeredImageElement(wstring element_name, const Gdiplus::BitmapData& bitmap_data, RendererRect& rect, uint32_t zorder)
{
	if (nullptr == bitmap_data.Scan0 || !renderer_)
		return;

	int x_pos = 0, y_pos = 0;
	CalLayeredImageElementPos(element_name, rect);

	RendererLayeredImageElementData layeredEleData;

	layeredEleData.rect = rect;
	layeredEleData.pixels_data.pixel_format_type = kPixelFormatTypeARGB8888;
	layeredEleData.pixels_data.width = bitmap_data.Width;
	layeredEleData.pixels_data.height = bitmap_data.Height;
	layeredEleData.pixels_data.pixels = (uint8_t*)bitmap_data.Scan0;
	layeredEleData.pixels_data.pitch = bitmap_data.Stride;
	layeredEleData.zorder = zorder;
	LayeredElementHandle layered_element_handle = renderer_->AddLayeredImageElement(renderer_item_handle_, layeredEleData);
	map_layed_element_[element_name] = layered_element_handle;
}

void VideoLayoutItemBase::CalLayeredImageElementPos(wstring element_name, RendererRect& rect)
{
	if (element_name == _T("UserName"))
	{
		rect.left = 0;
		rect.top = rect_.height - rect.height;
	}
	else if (element_name == _T("Unmute"))
	{
		rect.left = TEXT_LEFT_MARGIN;
		rect.top = rect_.height - rect.height - TEXT_TOP_MARGIN / 2;
	}
	else if (element_name == _T("UserAvatar"))
	{
		rect.left = (rect_.width - rect.width) / 2;
		rect.top = (rect_.height - rect.width) / 2;
	}
	else if (element_name == _T("Emoji"))
	{
		rect.left = REACTION_LEFT_MARGIN + emoji_middle_point_.x;
		rect.top = REACTION_TOP_MARGIN + emoji_middle_point_.y;
	}
	else if (element_name == _T("More"))
	{
		rect.left = rect_.width - rect.width - TEXT_LEFT_MARGIN / 2;
		rect.top = TEXT_TOP_MARGIN;
	}
}

Gdiplus::Bitmap* VideoLayoutItemBase::GetUserNameBitmap(int max_width, int muted_width)
{
	if (!user_)
		return NULL;

	wstring user_name = lower_third_name_;
	if (user_name.empty())
		user_name = user_->getUserName();

	ZoomVideoSDKShareStatus share_status = ZoomVideoSDKShareStatus_None;

	IVideoSDKVector<IZoomVideoSDKShareAction*>* pShareActionList = user_->getShareActionList();
	if (pShareActionList && pShareActionList->GetCount() == 0)
	{
		IZoomVideoSDKShareAction* pShareAction = pShareActionList->GetItem(0);
		if (pShareAction && pShareAction->getSharePipe())
			share_status = pShareAction->getSharePipe()->getShareStatus();
	}

	if (share_status == ZoomVideoSDKShareStatus_Start || share_status == ZoomVideoSDKShareStatus_Resume)	
		user_name += _T(" is sharing");
	
	if (!lower_third_description_.empty())
		user_name = user_name + _T("|") + lower_third_description_;

	StringBMPParam string_param;
	string_param.text = user_name;
	string_param.font_size = 10;
	string_param.font_type = _T("Microsoft YaHei");
	string_param.font_color = lower_third_font_color_;
	string_param.font_bk_color = lower_third_font_bk_color_;

	Gdiplus::Bitmap* bitmap = CreateBMPFromString(string_param, max_width, muted_width);
	return bitmap;
}

Gdiplus::Bitmap* VideoLayoutItemBase::GetLowerThirdBitmap(int max_width, int muted_width, bool is_hover)
{
	if (!user_)
		return NULL;

	wstring user_name = lower_third_name_;
	if (user_name.empty())
		user_name = user_->getUserName();

	ZoomVideoSDKShareStatus share_status = ZoomVideoSDKShareStatus_None;
	IVideoSDKVector<IZoomVideoSDKShareAction*>* pShareActionList = user_->getShareActionList();
	if (pShareActionList && pShareActionList->GetCount() > 0)
	{
		IZoomVideoSDKShareAction* pShareAction = pShareActionList->GetItem(0);
		if (pShareAction && pShareAction->getSharePipe())
			share_status = pShareAction->getSharePipe()->getShareStatus();
	}

	if (share_status == ZoomVideoSDKShareStatus_Start || share_status == ZoomVideoSDKShareStatus_Resume)
		user_name += _T(" is sharing");

	if (!lower_third_description_.empty())
		user_name = user_name + _T("|") + lower_third_description_;

	StringBMPParam string_param;
	string_param.text = user_name;
	string_param.font_size = 10;
	string_param.font_type = _T("Microsoft YaHei");
	string_param.font_color = lower_third_font_color_;
	string_param.font_bk_color = lower_third_font_bk_color_;

	Gdiplus::Bitmap* bitmap = is_hover ? CreateBMPFromLowerThirdString(string_param, max_width) : CreateBMPFromString(string_param, max_width, muted_width);
	return bitmap;
}

void VideoLayoutItemBase::UpdateRenderUserName()
{
	if (!renderer_) return;

	Gdiplus::Bitmap* bitmap = GetUserNameBitmap(rect_.width);
	if (!bitmap)
		return;

	Gdiplus::BitmapData bitmap_data;
	GetImageBitmapData(bitmap, bitmap_data);

	PixelsData pixels_data;
	pixels_data.width = bitmap_data.Width;
	pixels_data.height = bitmap_data.Height;
	pixels_data.pixels = (uint8_t*)bitmap_data.Scan0;
	pixels_data.pitch = bitmap_data.Stride;

	auto iter = map_layed_element_.find(_T("UserName"));
	if (iter != map_layed_element_.end())
	{
		renderer_->ModifyLayeredImageElementPixelsData(iter->second, pixels_data);

		RendererRect rect_before;
		renderer_->GetLayeredElementRect(iter->second, rect_before);

		RendererRect rect_new = { rect_before.left,rect_before.top,(int32_t)pixels_data.width,(int32_t)pixels_data.height };
		renderer_->ModifyLayeredElementRect(iter->second, rect_new);
	}
	else
	{
		RendererRect rect;
		rect.width = bitmap_data.Width;
		rect.height = bitmap_data.Height;
		AddLayeredImageElement(_T("UserName"), bitmap_data, rect, 2);
	}

	bitmap->UnlockBits(&bitmap_data);
	delete bitmap;
	bitmap = nullptr;
}

void VideoLayoutItemBase::UpdateAudioUserName()
{
	IZoomVideoSDKUser* pUser = GetUser();
	if (!pUser) return;

	ZoomVideoSDKAudioStatus audio_status = pUser->getAudioStatus();

	auto iter = map_layed_element_.find(_T("UserName"));
	if (iter != map_layed_element_.end())
	{
		if (audio_status.isMuted)
		{
			auto iter_element = map_layed_element_.find(_T("Unmute"));
			if (iter_element != map_layed_element_.end())
			{
				return;
			}

			uint32_t x_margin = 0, y_margin = 0;
			RendererRect rect_name_before;
			renderer_->GetLayeredElementRect(iter->second, rect_name_before);

			Gdiplus::Bitmap* pImage = LoadResImage(IDB_PNG_MUTED_SHADE);
			if (!pImage)
				return;

			Gdiplus::BitmapData data;
			GetImageBitmapData(pImage, data);

			int32_t width = 0, height = 0;
			
			height = pImage->GetHeight();
			width = pImage->GetWidth();
			RendererRect rect;
			rect.width = width;
			rect.height = height;
			AddLayeredImageElement(_T("Unmute"), data, rect, 3);

			//user name max width change, update user name image data.
			Gdiplus::Bitmap* user_name_bitmap = GetUserNameBitmap(rect_.width, width);
			if (!user_name_bitmap)
				return;

			Gdiplus::BitmapData user_name_bitmap_data;
			GetImageBitmapData(user_name_bitmap, user_name_bitmap_data);

			PixelsData user_name_pixels_data;
			user_name_pixels_data.width = user_name_bitmap_data.Width;
			user_name_pixels_data.height = user_name_bitmap_data.Height;
			user_name_pixels_data.pixels = (uint8_t*)user_name_bitmap_data.Scan0;
			user_name_pixels_data.pitch = user_name_bitmap_data.Stride;
			renderer_->ModifyLayeredImageElementPixelsData(iter->second, user_name_pixels_data);

			RendererRect rect_name_new = { 0,rect.top,(int32_t)user_name_bitmap_data.Width,(int32_t)user_name_pixels_data.height };
			renderer_->ModifyLayeredElementRect(iter->second, rect_name_new);

			pImage->UnlockBits(&data);
			user_name_bitmap->UnlockBits(&user_name_bitmap_data);
			delete user_name_bitmap;
			delete pImage;
			user_name_bitmap = nullptr;
			pImage = nullptr;
		}
		else
		{
			auto iter_element = map_layed_element_.find(_T("Unmute"));
			if (iter_element != map_layed_element_.end())
			{
				RendererRect rect_unmute_before;
				renderer_->GetLayeredElementRect(iter_element->second, rect_unmute_before);
				renderer_->RemoveLayeredElement(iter_element->second);
				map_layed_element_.erase(iter_element);

				//user name max width change, update user name image data.
				Gdiplus::Bitmap* user_name_bitmap = GetUserNameBitmap(rect_.width);
				if (!user_name_bitmap)
					return;

				Gdiplus::BitmapData user_name_bitmap_data;
				GetImageBitmapData(user_name_bitmap, user_name_bitmap_data);

				PixelsData user_name_pixels_data;
				user_name_pixels_data.width = user_name_bitmap_data.Width;
				user_name_pixels_data.height = user_name_bitmap_data.Height;
				user_name_pixels_data.pixels = (uint8_t*)user_name_bitmap_data.Scan0;
				user_name_pixels_data.pitch = user_name_bitmap_data.Stride;
				renderer_->ModifyLayeredImageElementPixelsData(iter->second, user_name_pixels_data);

				RendererRect rect_name_new;
				rect_name_new.width = user_name_bitmap_data.Width;
				rect_name_new.height = user_name_bitmap_data.Height;
				CalLayeredImageElementPos(_T("UserName"), rect_name_new);
				renderer_->ModifyLayeredElementRect(iter->second, rect_name_new);

				user_name_bitmap->UnlockBits(&user_name_bitmap_data);
				delete user_name_bitmap;
				user_name_bitmap = nullptr;
			}
		}
	}
}


void VideoLayoutItemBase::UpdateMoreBtn()
{
	IZoomVideoSDKUser* pUser = GetUser();
	IZoomVideoSDKUser* pMyself = ZoomVideoSDKMgr::GetInst().GetMySelf();
	if (!pUser || !pMyself) 
		return;
	if (pUser == pMyself)
	{
		return;
	}

	auto iter_element = map_layed_element_.find(_T("More"));
	if (iter_element != map_layed_element_.end())
	{
		renderer_->RemoveLayeredElement(iter_element->second);
	}

	Gdiplus::Bitmap* pImage = LoadResImage(IDB_PNG_MORE_NORMAL);
	if (!pImage)
		return;

	Gdiplus::BitmapData data;
	GetImageBitmapData(pImage, data);

	RendererRect rect;
	rect.width = pImage->GetWidth();
	rect.height = pImage->GetHeight();
	AddLayeredImageElement(_T("More"), data, rect, 2);

	pImage->UnlockBits(&data);
	delete pImage;
	pImage = nullptr;
}

void VideoLayoutItemBase::UpdateLowerThird()
{
	IZoomVideoSDKUser* pUser = GetUser();
	if (!pUser) return;

	ZoomVideoSDKAudioStatus audio_status = pUser->getAudioStatus();

	auto iter = map_layed_element_.find(_T("UserName"));
	if (iter != map_layed_element_.end())
	{
		if (audio_status.isMuted)
		{			
			auto iter_element = map_layed_element_.find(_T("Unmute"));
			if (iter_element != map_layed_element_.end())
			{
				renderer_->RemoveLayeredElement(iter_element->second);				
			}

			Gdiplus::Bitmap* pImage = LoadResImage(IDB_PNG_MUTED_SHADE);
			if (!pImage)
				return;

			Gdiplus::BitmapData data;
			GetImageBitmapData(pImage, data);

			RendererRect rect;
			rect.width = pImage->GetWidth();
			rect.height = pImage->GetHeight();
			AddLayeredImageElement(_T("Unmute"), data, rect, 3);

			//user name max width change, update user name image data.
			Gdiplus::Bitmap* lower_third_bitmap = GetLowerThirdBitmap(rect_.width, pImage->GetWidth());
			if (!lower_third_bitmap)
				return;

			Gdiplus::BitmapData lower_third_bitmap_data;
			GetImageBitmapData(lower_third_bitmap, lower_third_bitmap_data);

			PixelsData lower_third_pixels_data;
			lower_third_pixels_data.width = lower_third_bitmap_data.Width;
			lower_third_pixels_data.height = lower_third_bitmap_data.Height;
			lower_third_pixels_data.pixels = (uint8_t*)lower_third_bitmap_data.Scan0;
			lower_third_pixels_data.pitch = lower_third_bitmap_data.Stride;
			renderer_->ModifyLayeredImageElementPixelsData(iter->second, lower_third_pixels_data);

			//RendererRect rect_name_new = { 0 ,rect.top,(int32_t)(lower_third_bitmap_data.Width + pImage->GetWidth()),(int32_t)lower_third_pixels_data.height };
			RendererRect rect_name_new = { 0 ,rect.top - TEXT_LEFT_MARGIN / 2 ,rect_.width,(int32_t)lower_third_pixels_data.height };
			renderer_->ModifyLayeredElementRect(iter->second, rect_name_new);

			pImage->UnlockBits(&data);
			lower_third_bitmap->UnlockBits(&lower_third_bitmap_data);
			delete lower_third_bitmap;
			delete pImage;
			lower_third_bitmap = nullptr;
			pImage = nullptr;
		}
		else
		{
			auto iter_element = map_layed_element_.find(_T("Unmute"));
			if (iter_element != map_layed_element_.end())
			{
				renderer_->RemoveLayeredElement(iter_element->second);
			}

			//user name max width change, update user name image data.
			Gdiplus::Bitmap* lower_third_bitmap = GetLowerThirdBitmap(rect_.width,0);
			if (!lower_third_bitmap)
				return;

			Gdiplus::BitmapData lower_third_bitmap_data;
			GetImageBitmapData(lower_third_bitmap, lower_third_bitmap_data);

			PixelsData lower_third_pixels_data;
			lower_third_pixels_data.width = lower_third_bitmap_data.Width;
			lower_third_pixels_data.height = lower_third_bitmap_data.Height;
			lower_third_pixels_data.pixels = (uint8_t*)lower_third_bitmap_data.Scan0;
			lower_third_pixels_data.pitch = lower_third_bitmap_data.Stride;
			renderer_->ModifyLayeredImageElementPixelsData(iter->second, lower_third_pixels_data);

			RendererRect rect_name_new;
			rect_name_new.width = lower_third_bitmap_data.Width;
			rect_name_new.height = lower_third_bitmap_data.Height;
			CalLayeredImageElementPos(_T("UserName"), rect_name_new);
			renderer_->ModifyLayeredElementRect(iter->second, rect_name_new);

			lower_third_bitmap->UnlockBits(&lower_third_bitmap_data);
			delete lower_third_bitmap;
			lower_third_bitmap = nullptr;

		}
	}
}

void VideoLayoutItemBase::UpdateMouseHoverLowerThird()
{
	IZoomVideoSDKUser* pUser = GetUser();
	if (!pUser) return;

	ZoomVideoSDKAudioStatus audio_status = pUser->getAudioStatus();

	auto iter = map_layed_element_.find(_T("UserName"));
	if (iter != map_layed_element_.end())
	{
		//user name max width change, update user name image data.
		Gdiplus::Bitmap* lower_third_bitmap = GetLowerThirdBitmap(rect_.width,0,true);
		if (!lower_third_bitmap)
			return;

		Gdiplus::BitmapData lower_third_bitmap_data;
		GetImageBitmapData(lower_third_bitmap, lower_third_bitmap_data);

		PixelsData lower_third_pixels_data;
		lower_third_pixels_data.width = lower_third_bitmap_data.Width;
		lower_third_pixels_data.height = lower_third_bitmap_data.Height;
		lower_third_pixels_data.pixels = (uint8_t*)lower_third_bitmap_data.Scan0;
		lower_third_pixels_data.pitch = lower_third_bitmap_data.Stride;
		renderer_->ModifyLayeredImageElementPixelsData(iter->second, lower_third_pixels_data);

		RendererRect rect_lower_third_new;
		rect_lower_third_new.width = lower_third_bitmap_data.Width;
		rect_lower_third_new.height = lower_third_bitmap_data.Height;
		rect_lower_third_new.left = LOWER_THIRD_LEFT_MARGIN;
		rect_lower_third_new.top = rect_.height - lower_third_bitmap_data.Height - LOWER_THIRD_TOP_MARGIN;
		renderer_->ModifyLayeredElementRect(iter->second, rect_lower_third_new);

		lower_third_bitmap->UnlockBits(&lower_third_bitmap_data);
		delete lower_third_bitmap;
		lower_third_bitmap = nullptr;
	}
}

void VideoLayoutItemBase::UpdateLayeredElementRect()
{
	if (!renderer_)
		return;

	auto iter = map_layed_element_.find(_T("Unmute"));
	if (iter != map_layed_element_.end())
	{
		RendererRect rect_unmute_before;
		renderer_->GetLayeredElementRect(iter->second, rect_unmute_before);
		CalLayeredImageElementPos(iter->first, rect_unmute_before);
		renderer_->ModifyLayeredElementRect(iter->second, rect_unmute_before);

		auto iter_user_name = map_layed_element_.find(_T("UserName"));
		if (iter_user_name != map_layed_element_.end())
		{
			RendererRect rect_name_before;
			renderer_->GetLayeredElementRect(iter_user_name->second, rect_name_before);
			int32_t x_pos = 0, y_pos = 0;
			//x_pos = rect_unmute_before.left + rect_unmute_before.width;
			if (lower_third_name_.empty())
				x_pos = rect_unmute_before.left + rect_unmute_before.width;
			else
				x_pos = 0;

			y_pos = rect_unmute_before.top;
			RendererRect rect_name_update = { x_pos,y_pos,rect_name_before.width,rect_name_before.height };
			renderer_->ModifyLayeredElementRect(iter_user_name->second, rect_name_update);
		}
	}
	else
	{
		if (!dynamic_cast<ActiveVideoLayoutItem*>(this))
		{
			UpdateRenderUserName();
			if (user_)
			{
				if (user_->GetVideoPipe())
				{
					ZoomVideoSDKVideoStatus video_status = user_->GetVideoPipe()->getVideoStatus();
					if (!video_status.isOn)
					{
						AddUserAvatarIcon();
					}
				}
			}
		}

	}

	for (auto iter = map_layed_element_.begin(); iter != map_layed_element_.end(); iter++)
	{
		if (iter->first == _T("Unmute") || iter->first == _T("UserName"))
		{
			continue;
		}

		RendererRect rect_before;
		renderer_->GetLayeredElementRect(iter->second, rect_before);
		CalLayeredImageElementPos(iter->first, rect_before);
		renderer_->ModifyLayeredElementRect(iter->second, rect_before);
	}
}

Gdiplus::Bitmap* VideoLayoutItemBase::LoadResImage(int res_id)
{
	HMODULE hModule = GetModuleHandle(0);
	HRSRC hRsrc = ::FindResource(hModule, MAKEINTRESOURCE(res_id), _T("PNG")); // type 
	if (!hRsrc)
		return nullptr;
	// load resource into memory 
	DWORD len = SizeofResource(hModule, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hModule, hRsrc);
	if (!lpRsrc)
		return nullptr;
	// Allocate global memory on which to create stream 
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, lpRsrc, len);    	
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	// load from stream 
	Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromStream(pstm);	
	// free/release stuff 
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);

	return image;
}