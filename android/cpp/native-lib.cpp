#include "native-lib.h"
#include "iNDK.h"
#include "Game.h"

static iPoint* pp;
#define ppMax 100

extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_loadGame(JNIEnv* env, jobject obj,
                                    jobject ipkFilePath, jobject ioPath){
    pp = new iPoint[ppMax];

    //jstringPrint((jstring)ioPath);
    setDevicePath((jstring)ioPath);
    devSize.width = DEV_WIDTH;
    devSize.height = DEV_HEIGHT;

    //iGraphics::share()->start(hdc);
    loadOpenGL();
    appInitialize();
    loadGame();
}
extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_freeGame(JNIEnv* env, jobject obj){
    delete pp;
    //xprint("freeGame");
}
extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_drawGame(JNIEnv* env, jobject obj){
    //xprint("drawGame");
    mainLoop();
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

extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_keyGame(JNIEnv* env, jobject obj,
                                      jint state, jfloat x, jfloat y){
    //xprint("keyGame(%.f, %.f)", x, y);
    keyGame((iKeyState)state, coordinate(x, y));
}

extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_multiKeyGame(JNIEnv* env, jobject obj,
                                      jint state, jint num, jfloatArray mx, jfloatArray my){
    float* x = env->GetFloatArrayElements(mx, NULL);
    float* y = env->GetFloatArrayElements(my, NULL);

    for(int i = 0; i < num; i++)
        pp[i] = coordinate(x[i], y[i]);
    keyGame((iKeyState)state, num, pp);
}

extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_resizeGame(JNIEnv* env, jobject obj,
                                      jint width, jint height){
    resizeApp(width, height);
    readyOpenGL();
}

extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_pauseGame(JNIEnv* env, jobject obj,
                                         jboolean pause){
    //xprint(pause==0 ? "pause" : "resume");
    //env->GetObjectClass("");
}

extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_backKeyDown(JNIEnv* env, jobject obj)
{
    extern bool showTerminate;
    showTerminate = true;
}

extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_homeButton(JNIEnv* env, jobject obj)
{
    extern bool pressHomeB;
    pressHomeB = true;
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

    xprint("한글 devSize(%.f,%.f), real(%d, %d), viewport(%.f,%.f,%.f,%.f)\n",
            devSize.width, devSize.height,
            width, height,
            viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
}

void drawApp(FLOAT_METHOD methodDraw)
{
#if 0
    int f = iFPS::instance()->framesPerSec;
#endif
    float dt = iFPS::instance()->update();
#if 1//def _DEBUG
    if (dt > 0.1f)
        dt > 0.001f;
#endif // _DEBUG

    //if( dt > 0.001 )
    //xprintf("drawGame %f %d\n", dt, f);

    // binding fbo
    methodDraw(dt);// drawGame

#if (OS == OS_WINDOW)
    keyDown = 0;


#if 1// 원래는 App.cpp 56 라인에 있어야 함.
    extern void drawCursor(float dt);
    drawCursor(iFPS::instance()->lastDt);
#endif

#endif
    // unbind fbo
    // draw fbo
}

iFilter** filter;
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
                "shaderfilter/main.vert",
                {
                    "shaderfilter/aaa.frag",
                    {t, NULL, NULL, NULL},
                    {NEAREST, NEAREST, NEAREST, NEAREST,},
                    {REPEAT, REPEAT, REPEAT, REPEAT}
                }
            },
            {
                "shaderfilter/main.vert",
                {
                    "shaderfilter/bbb.frag",
                    {t, NULL, NULL, NULL},
                    {NEAREST, NEAREST, NEAREST, NEAREST,},
                    {REPEAT, REPEAT, REPEAT, REPEAT}
                }
            },
            {
                "shaderfilter/main.vert",
                {
                    "shaderfilter/ccc.frag",
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
}
#if 0
#if 0//none fbo
    readyOpenGL();
    drawApp(drawGame);
    //glFlush();// glFinish()
    SwapBuffers(hDC);
#else
    //readyOpenGL();
fbo->bind();
    glViewport(0, 0, devSize.width, devSize.height);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawApp(drawGame);
    // 원래는 여기 있는게 맞음.
    //drawCursor(iFPS::instance()->lastDt);
fbo->unbind();
    glViewport(viewport.origin.x, viewport.origin.y,
               viewport.size.width, viewport.size.height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setGLBlend(GLBlendMultiplied);
    setRGBA(1, 1, 1, 1);

    iPoint p = iPointZero;
    float s = updateZoom(iFPS::instance()->lastDt, p);

    timeDelta = 0.016;
//drawImage(fbo->tex, 0, 0, TOP | LEFT);
    Texture* t = fbo->tex;

    if(shaderToy == NULL)
    {
        shaderToy = new iShaderToy*[3];
        STInfo info[3] =
        {
            {
                "shadertoy/std.vert",
                {
                    {NULL},
                    {NULL},
                    {NULL},
                    {NULL},
                    {"shadertoy/aaa.frag",
                        {t, NULL, NULL, NULL},
                        {-1, -1, -1, -1},
                        {NEAREST, NEAREST, NEAREST, NEAREST,},
                        {REPEAT, REPEAT, REPEAT, REPEAT}
                    },
                }
            },
            {
                "shadertoy/std.vert",
                {
                    {NULL},
                    {NULL},
                    {NULL},
                    {NULL},
                    {"shadertoy/bbb.frag",
                        {t, NULL, NULL, NULL},
                        {-1, -1, -1, -1},
                        {NEAREST, NEAREST, NEAREST, NEAREST,},
                        {REPEAT, REPEAT, REPEAT, REPEAT}
                    },
                }
            },
            {
                "shadertoy/std.vert",
                {
                    {NULL},
                    {NULL},
                    {NULL},
                    {NULL},
                    {"shadertoy/ccc.frag",
                        {t, NULL, NULL, NULL},
                        {-1, -1, -1, -1},
                        {NEAREST, NEAREST, NEAREST, NEAREST,},
                        {REPEAT, REPEAT, REPEAT, REPEAT}
                    },
                }
            }
        };
        for(int i = 0; i < 3; i++)
            shaderToy[i] = new iShaderToy(&info[i]);
    }
    shaderToy[0]->paint(timeDelta, p);

    //drawImage(t, p.x, p.y, TOP | LEFT,
    //                0, 0, t->width, t->height, s, s, 2, 0, REVERSE_HEIGHT);

//drawImage(t, devSize.width, devSize.height, BOTTOM | RIGHT,
//    0, 0, t->width, t->height, 0.3f, 0.3f, 2, 0, REVERSE_HEIGHT);

    setGLBlend(GLBlendAlpha);
    //glFlush();// glFinish()
#if (OS == OS_WINDOW)
    SwapBuffers(hDC);
#endif
#endif
#endif