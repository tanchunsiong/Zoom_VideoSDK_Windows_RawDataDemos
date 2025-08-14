#pragma once

#include <magnification.h>
#include "enum_apps_info_mgr_comm_def.h"

AppList GetAppList();
bool IsWindowValidAndVisible(HWND window);
bool IsWindowResponding(HWND window);
bool CanSafelyMakeBlockingCalls(HWND hwnd);
bool IsWindowVisibleOnCurrentDesktop(HWND hwnd);
bool IsWindowOnCurrentDesktop(HWND hwnd);
bool IsWindowCloaked(HWND hwnd);