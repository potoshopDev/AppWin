//***************************************************************************************
// App.h by potoshopDev (C) 2023 All Rights Reserved.
//***************************************************************************************
#pragma once

#include "AppProc.h"
#include "helper.h"

namespace dx
{
    using GSLONG = getset<long>;
    using GSHWND = getset<HWND>;

    using WSIZE = std::pair<long, long>;
    struct Size
    {
        GSLONG width{0L};
        GSLONG height{0L};
    };

    class App final
    {
    private:
        //----------------------------------------------------------------
        class IAppSize
        {
        public:
            virtual ~IAppSize() {}
            inline virtual WSIZE get() const = 0;
        };

        class AppSize final : public IAppSize
        {
        private:
            Size m_size;

        public:
            AppSize(long width_, long height_) : m_size(Size{width_, height_}) {}
            inline WSIZE get() const override
            {
                const auto w = m_size.width.get();
                const auto h = m_size.height.get();
                return {w, h};
            };
        };

        class IAppId
        {
        public:
            virtual ~IAppId() {}
            inline virtual HWND get() const = 0;
        };

        class AppId final : public IAppId
        {
        private:
            GSHWND hwnd;

        public:
            AppId(HWND hwnd_) : hwnd(hwnd_) {}
            inline HWND get() const override { return hwnd.get(); }
        };

        //----------------------------------------------------------------
    private:
        std::unique_ptr<IAppId> id;
        std::unique_ptr<IAppSize> size;

    private:
        MainProcQueue proc_queue{};

    private:
        // The app cannot be copied, moved or assigned
        App(const App &) = delete;
        App &operator=(const App &) = delete;
        App(App &&) = delete;

    public:
        App(HWND hwnd_, long width_, long height_) : id(std::make_unique<AppId>(hwnd_)),
                                                     size(std::make_unique<AppSize>(width_, height_))
        {
        }

        ~App() = default;

        inline HWND get_hwnd() const noexcept { return id->get(); }
        inline WSIZE get_size() const noexcept { return size->get(); }

        template <typename T>
        void add_proc(T *t);
        void queue_work(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

        LRESULT CALLBACK AppMainProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    };

    //----------------------------------------------------------------
    std::unique_ptr<App> create_app(LPCSTR app_name = "App", const WSIZE base_size = {1280L, 960L}, LPCSTR class_mame = "class_app");
    int run_app();
    void show_err_box(LPCSTR err_desc);

    template <typename T>
    void App::add_proc(T *t)
    {
        proc_queue.emplace_back(t);
    }

} // end namespace dx
