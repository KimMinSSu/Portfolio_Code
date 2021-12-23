#pragma once

#include "iType.h"
#include "iPoint.h"
#include "iOpenGL.h"

extern int filterNum;
enum FilterNum
{
    filterA = 0,
    filterB,
    filterC,

    filterMax
};

struct FilterFrag
{
    const char* strFrag;
    Texture* tex[4];
    TextureFilter filter[4];
    TextureWrap wrap[4];
};

struct FilterBind
{
    const char* strFrag;
    Texture* tex[4];
    TextureFilter filter[4];
    TextureWrap wrap[4];
};

struct FilterInfo
{
    const char* strVert;
    FilterFrag frag;
};

class iFilter
{
public:
    iFilter(FilterInfo* info);
    virtual ~iFilter();

    void paint(float dt);

public:
    GLuint vbo, vbe;
    GLuint programID;

    FilterBind bind;

    float takeTime;
};
