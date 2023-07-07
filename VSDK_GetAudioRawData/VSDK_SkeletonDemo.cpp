// VSDK_RawDataDemos.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "VSDK_SkeletonDemo.h"
#include "main_frame.h"
#include <string>


// Global Variables:
HINSTANCE hInst;                                // current instance



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    CMainFrame::GetInstance().InitVideoSDK();
    CMainFrame::GetInstance().JoinSession();


    // Main loop
    while (true) {


        // Do some work here
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Sleep for a short period of time to avoid consuming too much CPU
        Sleep(10);
    }

    return 0;
}
