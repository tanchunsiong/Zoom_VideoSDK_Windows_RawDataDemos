// VSDK_RawDataDemos.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "VSDK_SkeletonDemo.h"
#include "main_frame.h"
#include <string>
#include "Windows.h"


// Global Variables:
HINSTANCE hInst;                                // current instance

// Declaration of the callback function
INT_PTR CALLBACK MainDialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);



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

    // Create the main dialog, dreamtcs this is for UI
    DialogBoxW(hInstance, MAKEINTRESOURCE(IDD_VSDKRAWDATADEMOS_DIALOG), NULL, MainDialogProc);

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

// Dialog procedure for the main dialog
INT_PTR CALLBACK MainDialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        // Handle button clicks
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON1:
          
            CMainFrame::GetInstance().StartMicRecording();
            break;

        case IDC_BUTTON2:
            
            CMainFrame::GetInstance().StopMicRecording();
            break;

        case IDC_BUTTON3:
           
            CMainFrame::GetInstance().PlayMicRecording();
            break;
        case IDC_BUTTON4:

            CMainFrame::GetInstance().StartSpeakerTest();
            break;

        case IDC_BUTTON5:

            CMainFrame::GetInstance().StopSpeakerTest();
            break;
        }

        break;

    case WM_CLOSE:
        EndDialog(hwndDlg, 0);
        break;

    default:
        return FALSE;
    }
    return TRUE;
}

