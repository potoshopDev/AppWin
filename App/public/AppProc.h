//***************************************************************************************
// AppProc.h by potoshopDev (C) 2023 All Rights Reserved.
//***************************************************************************************
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <memory>
#include <utility>
#include <vector>
#include <windows.h>

namespace dx
{
    // The class is not the owner of the resource!
    // The resource should not be moved or deleted until the end of the program
    class AppProc final
    {

    private:
        class IAppProcObject
        {
        public:
            virtual ~IAppProcObject() {}
            virtual LRESULT CALLBACK AppMainProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) = 0;
        };

        template <typename T>
        class AppProcObject final: public IAppProcObject
        {

            T *data;

        public:
            AppProcObject(T *t) : data(t) {}

            LRESULT CALLBACK AppMainProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override
            {
                return data->AppMainProc(hwnd, msg, wparam, lparam);
            }
        };

    private:
        std::unique_ptr<IAppProcObject> self_;

    public:
        template <typename T>
        AppProc(T *t) : self_(new AppProcObject(t)) {}

    public:
        AppProc(const AppProc &other) = delete;
        AppProc(AppProc &&other) noexcept = default;
        AppProc &operator=(AppProc other) = delete;

    public:
        friend LRESULT CALLBACK AppMainProc(const AppProc &app_proc, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            return app_proc.self_->AppMainProc(hwnd, msg, wParam, lParam);
        }
    };

    using MainProcQueue = std::vector<AppProc>;
    LRESULT CALLBACK AppMainProc(const MainProcQueue &proc_stack, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

} // namespace dx