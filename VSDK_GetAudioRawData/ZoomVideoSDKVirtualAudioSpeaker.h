#pragma once
//getRawAudio

#include "helpers/zoom_video_sdk_audio_send_rawdata_interface.h"
using namespace ZOOMVIDEOSDK;

class ZoomVideoSDKVirtualAudioSpeaker :
    public IZoomVideoSDKVirtualAudioSpeaker
{
public:


    //IZoomVideoSDKVirtualAudioSpeaker
    virtual void onVirtualSpeakerMixedAudioReceived(AudioRawData* data_);
    virtual void onVirtualSpeakerOneWayAudioReceived(AudioRawData* data_, ZOOM_VIDEO_SDK_NAMESPACE::IZoomVideoSDKUser* pUser);
    virtual void onVirtualSpeakerSharedAudioReceived(AudioRawData* data_);


protected:


};



