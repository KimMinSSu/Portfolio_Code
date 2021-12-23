#include "iFilter.h"
#include "iStd.h"

int filterNum = filterA;
iFilter::iFilter(FilterInfo* info)
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    float position[16] = {
        -1, 1, 0, 1,     1, 1, 0, 1,
        -1,-1, 0, 1,     1,-1, 0, 1,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, position, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbe);
    GLubyte indices[6] = {0, 1, 2,  1, 2, 3};
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 6, indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    int length;
    char* str = loadFile(info->strVert, length);
    GLuint vertID = createShaderID(str, 0);
    delete str;

    FilterFrag* frag = &info->frag;
    char* strBody = loadFile(frag->strFrag, length);
    str = new char[length + 1024];
    const char* strFirst =
            "#version 150                           \n\
            #ifdef GL_ES                            \n\
            precision mediump float;                \n\
            #endif                                  \n\
													\n\
			uniform vec3      iResolution;          \n\
			uniform float     iTime;                \n\
			uniform float     iTimeDelta;           \n\
			uniform sampler2D iChannel0;            \n\
			uniform sampler2D iChannel1;            \n\
			uniform sampler2D iChannel2;            \n\
			uniform sampler2D iChannel3;            \n";
    strcpy(str, strFirst);
    strcpy(&str[strlen(str)], strBody); 
    delete strBody;

    GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragID, 1, &str, NULL);
    glCompileShader(fragID);
    checkShaderID(fragID);

    programID = createProgramID(vertID, fragID);
    freeShaderID(fragID);
    freeShaderID(vertID);

    for(int i = 0; i < 4; i++)
    {
        bind.tex[i] = info->frag.tex[i];
        bind.filter[i] = info->frag.filter[i];
        bind.wrap[i] = info->frag.wrap[i];
    }

    takeTime = 0.0f;
}

iFilter::~iFilter()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vbe);
    freeProgramID(programID);

}

void iFilter::paint(float dt)
{
    glBlendFunc(GL_ONE, GL_ALPHA);
    glDisable(GL_BLEND);

    GLuint id = programID;
    glUseProgram(id);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLuint attr = glGetAttribLocation(id, "position");
    glEnableVertexAttribArray(attr);
    glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, (const void*)0);
    GLuint attrPosition = attr;

    GLuint loc = glGetUniformLocation(id, "iResolution");
    glUniform3f(loc, devSize.width, devSize.height, 0.0f);

    loc = glGetUniformLocation(id, "iTime");
    glUniform1f(loc, takeTime);

    loc = glGetUniformLocation(id, "iTimeDelta");
    glUniform1f(loc, dt);

    FilterBind* b = &bind;
    for (int i = 0; i < 4; i++)
    {
        Texture* tex = b->tex[i];
        if (tex == NULL) continue;

        char s[32];
        sprintf(s, "iChannel%d", i);
        loc = glGetUniformLocation(id, s);
        glUniform1i(loc, i);

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, tex->texID);
        setTexture(b->wrap[i], b->filter[i]);
    }

    // 런
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(attrPosition);
    //glBindTexture(GL_TEXTURE_2D, 0);
    for(int i=0; i<4; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glEnable(GL_BLEND);

    takeTime += dt;
}

