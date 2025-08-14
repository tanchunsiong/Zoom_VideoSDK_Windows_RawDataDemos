#ifndef _VIDEO_LAYOUT_ITEM_UI_H_
#define _VIDEO_LAYOUT_ITEM_UI_H_

//#include "zoom_video_sdk_include_interface.h"
#include "zoom_videosdk_renderer_def.h"

struct LowerThirdColorInfo;
struct StringBMPParam
{
	wstring text;
	wstring font_type;
	uint32_t font_size;
	uint32_t font_lower_third_size;
	Gdiplus::Color font_color;
	Gdiplus::Color font_bk_color;
	Gdiplus::RectF font_bk_rect;
	Gdiplus::StringAlignment align_type;
	StringBMPParam()
	{
		font_size = 10;
		font_lower_third_size = 13;
		font_type = _T("Microsoft YaHei");
		font_color = { 255,255,255 };
		font_bk_color = Gdiplus::Color(68, 75, 83, 1);
		font_bk_rect = { 0,0,0,0 };
		align_type = Gdiplus::StringAlignmentNear;
	}
};

class IZoomVideoSDKRenderer;
class VideoLayoutItemBase : public IZoomVideoSDKRawDataPipeDelegate
{
public:
	VideoLayoutItemBase(IZoomVideoSDKRenderer* renderer);
	virtual ~VideoLayoutItemBase();
	virtual void Subscribe(IZoomVideoSDKUser* pUser, ZoomVideoSDKRawDataType dataType, ZoomVideoSDKResolution eResolution);
	void unSubscribe();
	void Clean();
	void SetPos(RendererRect rect);
	void SetLowerThirdInfo(wstring lower_third_name, wstring lower_third_description, LowerThirdColorInfo& color_info);
	
	RendererRect GetPos() const;
	bool IsIdle();
	void SwitchRenderMode(RendererMode mode);
	RendererMode GetRenderMode() const;
	void set_is_selected(bool is_selected);
	bool get_is_selected() const;
	void set_user_emoji_resid(int res_id);
	int  get_user_emoji_resid();
	void SetRenderMode(RendererMode mode);
	IZoomVideoSDKUser* GetUser() const;

	void OnShareStatusChange();
	void OnAudioStatusChange();
	void OnEmojiStatusChange(int res_id);
	void OnLowerThirdChange();
	void AddRendererVideoItem(IZoomVideoSDKUser* user);
	void AddUserAvatarIcon();
	bool GetUserHasSubscribed();
	bool GetUserHasHandUp();
	void SetUserHasHandUp(bool is_raise_hand);
	void SetEmojiPos(POINT emoji_point);

	
public: //IZoomVideoSDKRawDataPipeDelegate
	virtual void onRawDataFrameReceived(YUVRawDataI420* data);
	virtual void onRawDataStatusChanged(RawDataStatus status);
	virtual void onShareCursorDataReceived(ZoomVideoSDKShareCursorData info) {}

protected:
	void UpdateLayeredElementRect();
	Gdiplus::Bitmap* CreateBMPFromString(const StringBMPParam& string_param,int max_width, int muted_width = 0);
	Gdiplus::Bitmap* CreateBMPFromLowerThirdString(const StringBMPParam& string_param, int max_width);
	void GetImageBitmapData(Gdiplus::Bitmap* image, Gdiplus::BitmapData& bitmap_data);
	void AddLayeredImageElement(wstring element_name, const Gdiplus::BitmapData& bitmap_data, RendererRect& rect, uint32_t zorder);
	void CalLayeredImageElementPos(wstring element_name, RendererRect& rect);
	void UpdateRenderUserName();
	void UpdateAudioUserName();
	void UpdateUserEmoji(int res_id = -1);
	void UpdateLowerThird();
	void UpdateMoreBtn();
	void UpdateMouseHoverLowerThird();
	void RemoveRendererVideoItem();
	void Trim(wstring& str); //Remove space at both ends
	wstring CalUserAvatar(const wstring& user_name);
	void DeleteUserAvatarIcon();
	Gdiplus::Bitmap* LoadResImage(int res_id);
	Gdiplus::Bitmap* GetUserNameBitmap(int max_width,int muted_width = 0);
	Gdiplus::Bitmap* GetLowerThirdBitmap(int max_width, int muted_width, bool is_hover = false);
protected:
	IZoomVideoSDKRenderer* renderer_;
	IZoomVideoSDKUser* user_;
	ItemHandle renderer_item_handle_;
	RendererRect rect_;
	RendererMode mode_;
	ZoomVideoSDKRawDataType raw_data_type_;
	RendererRect more_btn_rect_;

	bool is_mouse_hover_;
	int user_emoji_resid_;
	wstring lower_third_name_;
	wstring lower_third_description_;
	wstring lower_third_color_;
	Gdiplus::Color lower_third_font_color_;
	Gdiplus::Color lower_third_font_bk_color_;
	bool user_has_subscribed_;
	bool user_has_hand_up_;
	POINT emoji_middle_point_;

	std::map<wstring, LayeredElementHandle> map_layed_element_;
};

#endif //_VIDEO_LAYOUT_ITEM_UI_H_