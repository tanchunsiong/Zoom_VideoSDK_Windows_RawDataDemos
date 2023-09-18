//sendRawVideo
#include "helpers/zoom_video_sdk_video_source_helper_interface.h"
#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include "zoom_video_sdk_def.h"


constexpr auto WIDTH = 640;
constexpr auto HEIGHT = 480;

using namespace ZOOMVIDEOSDK;
using namespace std;

class ZoomVideoSDKVideoSource :
	public IZoomVideoSDKVideoSource
{
private:
	IZoomVideoSDKVideoSender* video_sender_;
	std::string video_source_;
public:
	ZoomVideoSDKVideoSource(std::string video_source);
	virtual	void onInitialize(IZoomVideoSDKVideoSender* sender, IVideoSDKVector<VideoSourceCapability >* support_cap_list, VideoSourceCapability& suggest_cap);
	virtual void onPropertyChange(IVideoSDKVector<VideoSourceCapability >* support_cap_list, VideoSourceCapability suggest_cap);
	virtual void onStartSend();
	virtual void onStopSend();
	virtual void onUninitialized();

protected:


};



