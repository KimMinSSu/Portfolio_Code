#include "iWindow.h"
#include "Game.h"

WCHAR szTitle[100] = TEXT("Portfolio");
WCHAR szWindowClass[100] = TEXT("Portfolio");

HINSTANCE hInst;
HWND hWnd;
HDC hdc;

bool runWnd;
bool isFullscreen;

iFilter** filter;
void mainLoop();
void updateWindow();

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RUN));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_BULLET);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    int x = -1000000, y = -1000000, width = DEV_WIDTH, height = DEV_HEIGHT;
    DWORD style = WS_OVERLAPPED |
        WS_CAPTION |
        WS_SYSMENU |
        WS_THICKFRAME |
        WS_MINIMIZEBOX |
        WS_MAXIMIZEBOX;
    hWnd = CreateWindowW(szWindowClass, szTitle, style,
        x, y, width, height, NULL, NULL, hInstance, NULL);
   
    if (hWnd == NULL)
        return FALSE;

    hdc = GetDC(hWnd);
    startApp(hdc, loadGame);

    ShowWindow(hWnd, nCmdShow);
    //UpdateWindow(hWnd);
    updateWindow();
    loadCursor();
    goFullscreen();

    MSG msg;
    for (runWnd = true; runWnd; )
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            mainLoop();
        }
    }

    ReleaseDC(hWnd, hdc);
    freeCursor();
    endApp(freeGame);

    for (int i = 0; i < filterMax; i++)
        delete filter[i];
    delete filter;

    return (int)msg.wParam;
}

float timeDelta = 0;
void mainLoop()
{
    static Texture* texGame = createTexture(devSize.width, devSize.height);

    fbo->bind(texGame);
    glViewport(0, 0, devSize.width, devSize.height);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawApp(drawGame);
    // 원래는 여기 있는게 맞음.
    //drawCursor(iFPS::instance()->lastDt);
    fbo->unbind();

    fbo->bind();
    fbo->clear(0, 0, 0, 0);
    iPoint p = iPointZero;
    timeDelta = iFPS::instance()->lastDt;
    Texture* t = texGame;
    if (filter == NULL)
    {
        filter = new iFilter * [filterMax];
        FilterInfo info[filterMax] =
        {
            {
                "Assets/shaderfilter/main.vert",
                {
                    "Assets/shaderfilter/aaa.frag",
                    {t, NULL, NULL, NULL},
                    {NEAREST, NEAREST, NEAREST, NEAREST,},
                    {REPEAT, REPEAT, REPEAT, REPEAT}
                }
            },
            {
                "Assets/shaderfilter/main.vert",
                {
                    "Assets/shaderfilter/bbb.frag",
                    {t, NULL, NULL, NULL},
                    {NEAREST, NEAREST, NEAREST, NEAREST,},
                    {REPEAT, REPEAT, REPEAT, REPEAT}
                }
            },
            {
                "Assets/shaderfilter/main.vert",
                {
                    "Assets/shaderfilter/ccc.frag",
                    {t, NULL, NULL, NULL},
                    {NEAREST, NEAREST, NEAREST, NEAREST,},
                    {REPEAT, REPEAT, REPEAT, REPEAT}
                }
            },
        };
        for (int i = 0; i < filterMax; i++)
            filter[i] = new iFilter(&info[i]);
    }
    filter[filterNum]->paint(timeDelta);
    fbo->unbind();

    glViewport(viewport.origin.x, viewport.origin.y,
        viewport.size.width, viewport.size.height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setGLBlend(GLBlendMultiplied);
    setRGBA(1, 1, 1, 1);

    t = fbo->tex;
    drawImage(t, p.x, p.y, TOP | LEFT,
        0, 0, t->width, t->height, 1, 1, 2, 0, REVERSE_HEIGHT);

    setGLBlend(GLBlendAlpha);

    SwapBuffers(hdc);
}

iSize sizeMonitor;
static int win_border_width = 0, win_border_height = 0;

void updateWindow()
{
    RECT rect;
    GetWindowRect(hWnd, &rect);
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    int x = (sizeMonitor.width - w) / 2;
    int y = (sizeMonitor.height - h) / 2;

    SetWindowPos(hWnd, HWND_TOP,
        x, y, w + win_border_width, h + win_border_height, SWP_SHOWWINDOW);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
        {
            sizeMonitor = iSizeMake(GetSystemMetrics(SM_CXSCREEN),
                GetSystemMetrics(SM_CYSCREEN));

            RECT rt;
            GetWindowRect(hWnd, &rt);
            RECT rtWnd = rt;
            GetClientRect(hWnd, &rt);
            win_border_width = (rtWnd.right - rtWnd.left) - (rt.right - rt.left);
            win_border_height = (rtWnd.bottom - rtWnd.top) - (rt.bottom - rt.top);
            //GetSystemMetrics(SM_CXFRAME) * 2;// == win_border_width
            //GetSystemMetrics(SM_CYFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);// == win_border_height
            return 0;
        }

        case WM_GETMINMAXINFO:
        {
            if (win_border_width == 0 || win_border_height == 0)
                return 0;

            //int minWidth = sizeMonitor.width * RATE_MONITOR;
            //int minHeight = minWidth * (devSize.height / devSize.width);

            int minWidth = DEV_WIDTH;
            int minHeight = DEV_HEIGHT;

            MINMAXINFO* info = (MINMAXINFO*)lParam;
            info->ptMinTrackSize.x = minWidth + win_border_width;
            info->ptMinTrackSize.y = minHeight + win_border_height;
            return 0;
        }

        case WM_SIZING:
        case WM_MOVING:
        {
            //RECT rt;
            //GetWindowRect(hWnd, &rt);
            RECT& rect = *reinterpret_cast<LPRECT>(lParam);
            enforceResolution((int)wParam, rect, win_border_width, win_border_height);
            resizeApp(rect.right - rect.left - win_border_width,
                rect.bottom - rect.top - win_border_height);
            mainLoop();
            return 0;// break;
        }

        case WM_SIZE:
        {
            //RECT rt;
            //GetClientRect(hWnd, &rt);
            resizeApp(LOWORD(lParam), HIWORD(lParam));
            mainLoop();
            return 0;// break;
        }

        case WM_KEYDOWN:
        {
            if (wParam == VK_F1)
            {
                goFullscreen();
                return 0;
            }
            setKeyStat(iKeyStateBegan, wParam);
            setKeyDown(iKeyStateBegan, wParam);
            return 0;
        }

        case WM_KEYUP:
        {
            setKeyStat(iKeyStateEnded, wParam);
            setKeyDown(iKeyStateEnded, wParam);
            return 0;
        }

        case WM_SETCURSOR:
        {
            if (updateCursor(LOWORD(lParam) == HTCLIENT))
                return true;
            break;
        }

        case WM_LBUTTONDOWN:
        {
            cursor = coordinate(LOWORD(lParam), HIWORD(lParam));
            keyGame(iKeyStateBegan, cursor);
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            cursor = coordinate(LOWORD(lParam), HIWORD(lParam));
            keyGame(iKeyStateMoved, cursor);
            return 0;
        }
        case WM_LBUTTONUP:
        {
            cursor = coordinate(LOWORD(lParam), HIWORD(lParam));
            keyGame(iKeyStateEnded, cursor);
            return 0;
        }
        case WM_CLOSE:
        {
            runWnd = false;
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

void setCurrentMonitor(RECT& rt)
{
    GetWindowRect(hWnd, &rt);
    HMONITOR hMonitor = MonitorFromRect(&rt, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi;
    mi.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hMonitor, &mi);
    if (mi.dwFlags == MONITORINFOF_PRIMARY)
    {
        rt.left = 0;
        rt.right = sizeMonitor.width;
        rt.top = 0;
        rt.bottom = sizeMonitor.height;
    }
    else
    {
        memcpy(&rt, &mi.rcWork, sizeof(RECT));
    }
}

RECT rtPrev;
void goFullscreen()
{
    isFullscreen = !isFullscreen;
    if (isFullscreen)
    {
        GetWindowRect(hWnd, &rtPrev);
        RECT rt;
        setCurrentMonitor(rt);
        int x = rt.left,
            y = rt.top,
            w = rt.right - rt.left,
            h = rt.bottom - rt.top;
        SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
        SetWindowPos(hWnd, HWND_TOP, x, y, w, h, SWP_SHOWWINDOW);
    }
    else
    {
        int x = rtPrev.left,
            y = rtPrev.top,
            w = rtPrev.right - rtPrev.left,
            h = rtPrev.bottom - rtPrev.top;
        SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowPos(hWnd, HWND_TOP, x, y, w, h, SWP_SHOWWINDOW);
    }
}

void enforceResolution(int edge, RECT& rect, int win_border_width, int win_border_height)
{
    switch (edge)
    {
    case WMSZ_BOTTOM:
    case WMSZ_TOP:
    {
        // w : h = devSize.width : devSize.height
        int h = (rect.bottom - rect.top) - win_border_height;// client width
        int w = h * devSize.width / devSize.height + win_border_width;// window_width
        rect.left = (rect.left + rect.right) / 2 - w / 2;
        rect.right = rect.left + w;
        break;
    }
    case WMSZ_BOTTOMLEFT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        //if( w/h > devSize.width/devSize.height )
        if (w * devSize.height > devSize.width * h)
        {
            // w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.left = rect.right - w;
        }
        else
        {
            // w : h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + win_border_height;
            rect.bottom = rect.top + h;
        }
        break;
    }
    case WMSZ_BOTTOMRIGHT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        //if( w/h > devSize.width/devSize.height )
        if (w * devSize.height > devSize.width * h)
        {
            // w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.right = rect.left + w;
        }
        else
        {
            // w : h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + win_border_height;
            rect.bottom = rect.top + h;
        }
        break;
    }
    case WMSZ_LEFT:
    case WMSZ_RIGHT:
    {
        // w : h = devSize.width : devSize.height
        int w = (rect.right - rect.left) - win_border_width;
        int h = w * devSize.height / devSize.width + win_border_height;
        rect.top = (rect.top + rect.bottom) / 2 - h / 2;
        rect.bottom = rect.top + h;
        break;
    }
    case WMSZ_TOPLEFT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        //if( w/h > devSize.width/devSize.height )
        if (w * devSize.height > devSize.width * h)
        {
            // w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.left = rect.right - w;
        }
        else
        {
            // w : h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + win_border_height;
            rect.top = rect.bottom - h;
        }
        break;
    }
    case WMSZ_TOPRIGHT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        //if( w/h > devSize.width/devSize.height )
        if (w * devSize.height > devSize.width * h)
        {
            // w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.right = rect.left + w;
        }
        else
        {
            // w : h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + win_border_height;
            rect.top = rect.bottom - h;
        }
        break;
    }
    }
}


Texture* texCursor;
iPoint cursor;
bool bCursor;
void loadCursor()
{
    texCursor = createImage("Assets/Cursor/Cursor.png");

    cursor = iPointZero;
    bCursor = false;
}

void freeCursor()
{
    freeImage(texCursor);
}

void drawCursor(float dt)
{
    if (bCursor)
        drawImage(texCursor, cursor.x, cursor.y, TOP | LEFT);
}

bool updateCursor(bool inClient)
{
    if (bCursor == inClient)
        return false;

    bCursor = inClient;

    if (bCursor)
    {
        for (;;)
        {
            int n = ShowCursor(FALSE);
            if (n < 0) break;
        }
    }
    else
    {
        for (;;)
        {
            int n = ShowCursor(TRUE);
            if (n > -1) break;
        }
    }

    return true;
}



