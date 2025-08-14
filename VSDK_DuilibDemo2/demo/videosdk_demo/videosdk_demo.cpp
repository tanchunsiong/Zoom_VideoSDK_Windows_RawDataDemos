// video_sdk_demo.cpp 
//
#include "stdafx.h"
#include "videosdk_demo.h"
#include "main_frame.h"
#include "zoom_videosdk_renderer_def.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);

	::CoInitialize(NULL);
	::OleInitialize(NULL);

    DWORD dwExStyle = (WS_EX_APPWINDOW);
    HWND hWnd = CMainFrame::GetInstance().Create(NULL, MAIN_FRAME_TITLE, UI_WNDSTYLE_FRAME & ~WS_SYSMENU , dwExStyle);

    if (hWnd && ::IsWindow(hWnd))
    {
        CMainFrame::GetInstance().ShowWindow(false);
        CMainFrame::GetInstance().CenterWindow();
        CPaintManagerUI::MessageLoop();
    }

	::OleUninitialize();
	::CoUninitialize();

    return 0;
}
