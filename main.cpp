//***************************************************************************************
// main.cpp by potoshopDev (C) 2023 All Rights Reserved.
//***************************************************************************************

#include "App.h"
#include <stdexcept>
#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
    try
    {
        auto app = dx::create_app();
        return dx::run_app(); 
    }
    catch (const std::runtime_error &e)
    {
        dx::show_err_box(e.what());
    }
    catch (...)
    {
        dx::show_err_box("App aborted");
    }
}
