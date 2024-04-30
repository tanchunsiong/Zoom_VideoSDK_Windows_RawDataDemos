#include "VideoEncoder.h"
#include <cstdlib>

bool VideoEncoder::EncodeYUV420ToMP4(const std::string& yuv420FilePath, const std::string& mp4FilePath, int width, int height, int fps) {
    //std::string command = "ffmpeg -f rawvideo -video_size " + std::to_string(width) + "*" + std::to_string(height) +
    //    " -pix_fmt yuv420p -framerate " + std::to_string(fps) + " -i " + yuv420FilePath +
    //    " -f mp4  " + mp4FilePath;
    std::string command = "ffmpeg -f rawvideo -video_size " + std::to_string(width) + "*" + std::to_string(height) +
        " -pix_fmt yuv420p " + " -i " + yuv420FilePath +
        " -f mp4  " + mp4FilePath;
    return std::system(command.c_str()) == 0;
}
bool VideoEncoder::EncodePCMToWAV(const std::string& pcmFilePath, const std::string& wavFilePath, int sampleRate, int channels, int bitDepth) {
    // Construct the FFmpeg command for PCM to MP3 conversion
    std::string formatFlag = bitDepth == 16 ? "s16le" : "s32le"; // Example for 16-bit or 32-bit PCM, adjust as needed
    std::string command = "ffmpeg -f " + formatFlag + " -ar " + std::to_string(sampleRate) +
        " -ac " + std::to_string(channels) + " -i " + pcmFilePath +
        " " + wavFilePath;

    // Execute the command
    return std::system(command.c_str()) == 0;
}
bool VideoEncoder::CombineAudioVideo(const std::string& videoFilePath, const std::string& audioFilePath, const std::string& outputFilePath) {
    std::string command = "ffmpeg -i " + videoFilePath + " -i " + audioFilePath +
        " -c copy " + outputFilePath;
    return std::system(command.c_str()) == 0;
}