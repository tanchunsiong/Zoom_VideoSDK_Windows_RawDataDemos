// VSDK_RawDataDemos.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "main_frame.h"
#include <string>


// Global Variables:
HINSTANCE hInst;                                // current instance


void CreateConsoleWindow() {
    AllocConsole();
    FILE* pConsoleOutput;
    freopen_s(&pConsoleOutput, "CONOUT$", "w", stdout); // Redirect stdout to the console
    freopen_s(&pConsoleOutput, "CONOUT$", "w", stderr); // Redirect stderr to the console
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
  


    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Create the console window
    CreateConsoleWindow();

    // TODO: Place code here.

    MainFrame* mf = new MainFrame();
    mf->InitVideoSDK();
    mf->JoinSession();

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

    // Close the console window when your application exits
    FreeConsole();

    return 0;
}
