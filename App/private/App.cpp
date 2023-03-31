//***************************************************************************************
// App.cpp by potoshopDev (C) 2023 All Rights Reserved.
//***************************************************************************************
#include "App.h"
#include <stdexcept>

namespace dx
{
    WSIZE get_app_rec(const WSIZE base_size);

    bool register_win_class(LPCSTR class_name);
    void prepare_win(HWND hwnd);

    std::unique_ptr<App> prepare_app(LPCSTR app_name, const WSIZE base_size, LPCSTR class_name);

    HWND create_win(LPCSTR app_name, WSIZE base_saze, LPCSTR class_name);
    void message_processing(MSG &msg);

    std::unique_ptr<App> init_app(HWND hWnd, const WSIZE base_size);
    std::unique_ptr<App> make_app(HWND hWnd, const WSIZE base_size);
    void init_proc_queue(App *app);

    LRESULT CALLBACK MainWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    //----------------------------------------------------------------

    std::unique_ptr<App> create_app(LPCSTR app_name /*= "App"*/, const WSIZE base_size, LPCSTR class_name /*= class_app*/)
    {
        if (!register_win_class(class_name))
            throw std::runtime_error("Register class abort");

        return prepare_app(app_name, base_size, class_name);
    }


    bool register_win_class(LPCSTR class_name)
    {
        WNDCLASSEXA wc;
        wc.cbClsExtra = 0;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.cbWndExtra = 0;
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
        wc.hInstance = nullptr;
        wc.lpszClassName = class_name;
        wc.lpszMenuName = 0;
        wc.style = 0;
        wc.lpfnWndProc = &MainWinProc;

        if (!RegisterClassExA(&wc))
            return false;

        return true;
    }

    std::unique_ptr<App> prepare_app(LPCSTR app_name, const WSIZE base_size, LPCSTR class_name)
    {
        if (auto hwnd{create_win(app_name, base_size, class_name)})
        {
            return init_app(hwnd, base_size);
        }

        throw std::runtime_error("Connot create app");
    }

    HWND create_win(LPCSTR app_name, WSIZE base_saze, LPCSTR class_name)
    {
        const auto [width, height] = get_app_rec(base_saze);

        HINSTANCE hInstance{};
        const auto hWnd = CreateWindowExA(WS_EX_OVERLAPPEDWINDOW,
                                          class_name,
                                          app_name,
                                          WS_OVERLAPPEDWINDOW,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
                                          width,
                                          height,
                                          0,
                                          0,
                                          hInstance,
                                          0);

        return hWnd;
    }

    std::unique_ptr<App> init_app(HWND hWnd, const WSIZE base_size)
    {
        prepare_win(hWnd);

        auto main_app{make_app(hWnd, base_size)};
        main_app->add_proc(main_app.get());

        init_proc_queue(main_app.get());

        return main_app;
    }

    void prepare_win(HWND hwnd)
    {
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
    }

    std::unique_ptr<App> make_app(HWND hWnd, const WSIZE base_size)
    {
        const auto [width, height] = get_app_rec(base_size);
        return std::make_unique<App>(hWnd, width, height);
    }

    void init_proc_queue(App *app)
    {
        CREATESTRUCT cstr;
        cstr.lpCreateParams = app;

        UINT msg{WM_NULL};
        LPARAM lParam{reinterpret_cast<LPARAM>(&cstr)};
        SendMessage(app->get_hwnd(), msg, WPARAM(), lParam);
    }

    WSIZE get_app_rec(const WSIZE base_size)
    {
        const auto [bwidth, bheight] = base_size;
        RECT rw{0L, 0L, bwidth, bheight};
        AdjustWindowRect(&rw, WS_OVERLAPPEDWINDOW, false);

        const auto width{rw.right - rw.left};
        const auto height{rw.bottom - rw.top};

        return {width, height};
    }

    LRESULT CALLBACK App::AppMainProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        switch (msg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);

            break;
        }
        return LRESULT();
    }

    LRESULT CALLBACK MainWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        static std::vector<App *> apps;

        switch (message)
        {
        case WM_NULL:
            if (auto pcstr = reinterpret_cast<CREATESTRUCTA *>(lParam))
            {
                if (auto app = static_cast<App *>(pcstr->lpCreateParams))
                    apps.emplace_back(app);
            }

            break;
        default:
        {
            if (!apps.empty())
            {
                for (auto app : apps)
                {
                    if (app && app->get_hwnd() == hWnd)
                        app->queue_work(hWnd, message, wParam, lParam);
                }
            }
        }
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    void show_err_box(LPCSTR err_desc)
    {
        MessageBoxA(nullptr, err_desc, "error", MB_OK | MB_ICONERROR);
    }

    void App::queue_work(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        dx::AppMainProc(proc_queue, hwnd, message, wParam, lParam);
    }

    int run_app()
    {
        MSG msg{};

        while (msg.message != WM_QUIT)
            message_processing(msg);

        return (int)msg.wParam;
    }

    void message_processing(MSG &msg)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
} // namespace dx
