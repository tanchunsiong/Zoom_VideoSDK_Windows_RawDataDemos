//sendRawAudio
#include <iostream>
#include <cstdint>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <thread>

#include "ZoomVideoSDKVirtualAudioMic.h"
#include "helpers/zoom_video_sdk_user_helper_interface.h"

using namespace ZOOMVIDEOSDK;

using namespace std;
int audio_play_flag = -1;


void PlayAudioFileToVirtualMic(IZoomVideoSDKAudioSender* audio_sender, string audio_source)
{


    // execute in a thread.
    while (audio_play_flag > 0 && audio_sender) {

        // Check if the file exists
        ifstream file(audio_source, ios::binary | ios::ate);
        if (!file.is_open()) {
            std::cout << "Error: File not found. Tried to open " << audio_source << std::endl;
            return;
        }

        // Get the file size
        int file_size = file.tellg();

        // Read the file into a buffer
        vector<char> buffer(file_size);
        file.seekg(0, ios::beg);
        file.read(buffer.data(), file_size);

        // Send the audio data to the virtual camera
       ZoomVideoSDKErrors err = audio_sender->Send(buffer.data(), buffer.size(), 44100);
        if (err != ZoomVideoSDKErrors::ZoomVideoSDKErrors_Success) {
            std::cout << "Error: Failed to send audio data. Error code: " << err << std::endl;
            return;
        }
        file.close();

        //enables loop and no loop
        //audio_play_flag = -1;
    }

}


void ZoomVideoSDKVirtualAudioMic::onMicInitialize(IZoomVideoSDKAudioSender* rawdata_sender)
{
    //sendRawAudio
    printf("onMicInitialize\n");
    // Once the sender has been recieved from this callback, then Send can be called
    virtual_audio_sender_ = rawdata_sender;
}

void ZoomVideoSDKVirtualAudioMic::onMicStartSend()
{
    //sendRawAudio
    printf("onMicStartSend\n");
    std::cout << "onStartSend" << std::endl;
    if (virtual_audio_sender_ && audio_play_flag != 1) {
        while (audio_play_flag > -1) {}
        audio_play_flag = 1;
        std::string audio_source_ = "pcm1644m.wav";
        thread(PlayAudioFileToVirtualMic, virtual_audio_sender_, audio_source_).detach();

    }
  
}

void ZoomVideoSDKVirtualAudioMic::onMicStopSend()
{
    audio_play_flag = 0;

    // Virtual Mic stopped sending raw audio
}

void ZoomVideoSDKVirtualAudioMic::onMicUninitialized()
{
    virtual_audio_sender_ = nullptr;
}
