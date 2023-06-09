#include "ZoomVideoSDKVideoSource.h"
#include "helpers/zoom_video_sdk_user_helper_interface.h"

//sendRawVideo
using namespace ZOOMVIDEOSDK;




void ZoomVideoSDKVideoSource::onInitialize(IZoomVideoSDKVideoSender* sender, IVideoSDKVector<VideoSourceCapability >* support_cap_list, VideoSourceCapability& suggest_cap) {

	//sendRawVideo
	this->video_sender_ = sender;

}


void ZoomVideoSDKVideoSource::onPropertyChange(IVideoSDKVector<VideoSourceCapability >* support_cap_list, VideoSourceCapability suggest_cap) {}

void ZoomVideoSDKVideoSource::onStartSend() {
	//sendRawVideo
	printf("ZoomVideoSDKVideoSource::onStartSend() fired \n");
}
void ZoomVideoSDKVideoSource::onStopSend() {}

void ZoomVideoSDKVideoSource::onUninitialized() {}

