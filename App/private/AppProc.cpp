//***************************************************************************************
// AppProc.cpp by potoshopDev (C) 2023 All Rights Reserved.
//***************************************************************************************
#include "AppProc.h"
#include "App.h"

LRESULT dx::AppMainProc(const dx::MainProcQueue &proc_stack, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    for (const auto &local_proc : proc_stack)
    {
        AppMainProc(local_proc, hwnd, msg, wparam, lparam);
    }
    return LRESULT();
}

