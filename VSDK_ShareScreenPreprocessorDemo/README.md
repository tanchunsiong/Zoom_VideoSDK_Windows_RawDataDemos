# Zoom Video SDK + OpenCV + Tesseract Integration

This sample demonstrates a Zoom Video SDK application with integrated OCR preprocessing using OpenCV and Tesseract. The application captures shared screen frames, detects PII content using OCR, masks it, and streams the preprocessed data to session participants.

## Features

- Zoom Video SDK screen sharing integration
- Real-time OCR processing with Tesseract
- PII detection using regex patterns
- Visual masking of matched PII on shared video
- Console logging for debugging OCR results
- Easy-to-extend design for additional preprocessing

## Dependencies via vcpkg

```sh
./vcpkg install jsoncpp
./vcpkg install opencv
./vcpkg install tesseract
```

Make sure `TESSDATA_PREFIX` environment variable is set correctly to point to the tessdata directory (e.g., `C:/vcpkg/installed/x64-windows/share/tessdata`).

## Notes

- OpenCV is used for YUV to BGR conversion and resizing before OCR.
- Tesseract expects `eng.traineddata` to be available in the tessdata path.
- Masked PII is directly altered on the Y-plane for visual censorship.
- All processing occurs within the `RemovePIIFromRawData` method in `main_frame.cpp`.

# Zoom Video SDK + OpenCV + Tesseract (PII Masking)

This project integrates the Zoom Video SDK, OpenCV, and Tesseract OCR in C++ to perform real-time screen sharing with automatic PII redaction using OCR.

---

## 🧩 Key Components

- **Zoom Video SDK**: Handles session joining, screen sharing, and raw YUV frame capture.
- **OpenCV**: Converts raw YUV video frames to BGR and resizes for OCR.
- **Tesseract OCR**: Detects text (including sensitive patterns) and allows masking them on the video stream.
- **Regex Matching**: You can customize the `std::regex` to search for names, numbers, keywords like `"thread"` or `"Zane"`.

---

## 🛠 Prerequisites

- Windows 10/11 with Visual Studio 2022
- vcpkg installed at `C:/vcpkg`
- Environment variable: `TESSDATA_PREFIX` set to `C:/vcpkg/installed/x64-windows/share/tessdata`

Install dependencies:

```bash
vcpkg install opencv4:x64-windows
vcpkg install tesseract:x64-windows
vcpkg integrate install
```

Download `eng.traineddata` from https://github.com/tesseract-ocr/tessdata if it's missing.

---

## 🧪 OCR and PII Removal

The key OCR logic is inside:

```cpp
void MainFrame::RemovePIIFromRawData(YUVRawDataI420* data_)
```

Steps:

1. Convert YUV420 to `cv::Mat`
2. Resize to 50%
3. Run Tesseract OCR
4. Search for words using `std::regex`
5. Mask detected text in the Y plane with neutral gray

Example:
```cpp
std::regex piiRegex(R"(Zane)", std::regex_constants::icase);
```

To search for another word like "thread", just replace the regex.

---

## 🖥 Real-Time Share Preprocessing

- Hooks into Zoom SDK's `onCapturedRawDataReceived()`
- Edits the video frame in-place
- Sends the modified frame using `m_pShareSender->sendPreprocessedData(data_)`

---

## Screen Sharing with Preprocessing (PII Masking)

To enable screen sharing with real-time preprocessing using OCR and masking of PII:

### High-Level Workflow
1. **Call `startShareWithPreprocessing`**
   - Initiates screen or window sharing but does not broadcast yet.
2. **SDK captures the screen and triggers `onCapturedRawDataReceived`**
   - This delivers YUV raw frame data to your app.
3. **App processes and masks the YUV data**
   - Convert YUV to BGR using OpenCV.
   - Use Tesseract to detect text in the frame.
   - Match patterns (e.g. names, SSNs) with regex and mask them in-place.
4. **Call `sendPreprocessedData`**
   - Submit the modified frame to Zoom SDK.
5. **Zoom SDK broadcasts the masked video frame**
   - Other session participants receive the PII-safe video.

This happens in real-time and is designed to protect sensitive information during screen sharing sessions.

## 🧠 Notes

- If OCR is too slow, disable `resize()` or optimize Tesseract config.
- For better accuracy, avoid heavy downscaling before OCR.
- Use logging to validate OCR output via confidence scores.

---

## 🧼 Cleanup

To leave the session:

```cpp
video_sdk_obj_->leaveSession(true);
```

To uninitialize:

```cpp
video_sdk_obj_->cleanup();
```

---

## 📄 License

MIT