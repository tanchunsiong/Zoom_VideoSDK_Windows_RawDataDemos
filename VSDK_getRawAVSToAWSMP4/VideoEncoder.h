#ifndef VIDEOENCODER_H
#define VIDEOENCODER_H

#include <string>

class VideoEncoder {
public:
    static bool EncodeYUV420ToMP4(const std::string& yuv420FilePath, const std::string& mp4FilePath, int width, int height, int fps);
    static bool EncodePCMToWAV(const std::string& pcmFilePath, const std::string& mp3FilePath, int sampleRate, int channels, int bitDepth);
    static bool CombineAudioVideo(const std::string& videoFilePath, const std::string& audioFilePath, const std::string& outputFilePath);
};


#endif // VIDEOENCODER_H
