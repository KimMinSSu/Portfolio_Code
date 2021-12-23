#include "iWindow.h"

void startApp(HDC hdc, VOID_METHOD methodLoad)
{
    devSize.width = DEV_WIDTH;
    devSize.height = DEV_HEIGHT;

    iGraphics::share()->start(hdc);
    loadOpenGL(hdc);

    appInitialize();
    methodLoad();
}

void endApp(VOID_METHOD methodFree)
{
    methodFree();
    iGraphics::share()->end();
    freeOpenGL();
}

void drawApp(FLOAT_METHOD methodDraw)
{
#if 0
    int f = iFPS::instance()->framesPerSec;
#endif
    float dt = iFPS::instance()->update();
#if 1
    if (dt > 0.1f)
        dt = 0.001f;
#endif 

    methodDraw(dt);
    keyDown = 0;
    drawCursor(dt);
}

void resizeApp(int width, int height)
{
    float rx = width / devSize.width;
    float ry = height / devSize.height;
    if (rx < ry)
    {
        viewport.origin.x = 0;
        viewport.size.width = width;

        viewport.size.height = devSize.height * rx;
        viewport.origin.y = (height - viewport.size.height) / 2;
    }
    else
    {
        viewport.origin.y = 0;
        viewport.size.height = height;

        viewport.size.width = devSize.width * ry;
        viewport.origin.x = (width - viewport.size.width) / 2;
    }
}

iPoint coordinate(int x, int y)
{
    float r = devSize.width / viewport.size.width;
    //float r = devSize.height / viewport.size.height;
    iPoint p;
    p.x = (x - viewport.origin.x) * r;
    p.y = (y - viewport.origin.y) * r;

    return p;
}
