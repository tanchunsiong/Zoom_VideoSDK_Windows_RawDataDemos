//sendRawAudio
#include "helpers/zoom_video_sdk_audio_send_rawdata_interface.h"
using namespace ZOOMVIDEOSDK;
using namespace std;


class ZoomVideoSDKVirtualAudioMic :
    public IZoomVideoSDKVirtualAudioMic
{
public:


  

    //IZoomVideoSDKVirtualAudioMic
    virtual void onMicInitialize(ZOOM_VIDEO_SDK_NAMESPACE::IZoomVideoSDKAudioSender* rawdata_sender);
    virtual void onMicStartSend();
    virtual void onMicStopSend();
    virtual void onMicUninitialized();


protected:
    ZOOM_VIDEO_SDK_NAMESPACE::IZoomVideoSDKAudioSender* virtual_audio_sender_;

};



