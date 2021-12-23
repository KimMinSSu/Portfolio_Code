#include "iStd.h"

namespace STD {

	iSize devSize;

	iRect viewport;

	bool* keyBuf;

	void appInitialize()
	{
		srand(time(NULL));

		keyBuf = new bool[256];
		memset(keyBuf, 0x00, 256);
	}

	int keyState = 0, keyDown = 0;
	void setKeys(int& keys, iKeyState state, int c);
	void setKeyStat(iKeyState state, int c)
	{
		setKeys(keyState, state, c);
	}

	void setKeyDown(iKeyState stat, int c)
	{
		if (stat == iKeyStateBegan)
		{
			if (keyBuf[c])
				return;

			keyBuf[c] = true;
			setKeys(keyDown, stat, c);
		}
		else
		{
			keyBuf[c] = false;
		}
	}

	void setKeys(int& keys, iKeyState state, int c)
	{
		if (state == iKeyStateBegan)
		{
			if (c == VK_LEFT)
				keys |= keysLeftArrow;
			else if (c == VK_RIGHT)
				keys |= keysRightArrow;
			else if (c == VK_UP)
				keys |= keysUpArrow;
			else if (c == VK_DOWN)
				keys |= keysDownArrow;

			else if (c == VK_SHIFT)
				keys |= keysShift;
			else if (c == VK_ESCAPE)
				keys |= keysESC;
			else if (c == VK_RETURN)
				keys |= keysEnter;
			else if (c == 'Q' || c == 'q')
				keys |= keysDie;

			else
				keys |= AnyKeys;
		}
		else// if (state == iKeyStateEnded)
		{
			if (c == VK_LEFT)
				keys &= ~keysLeftArrow;
			else if (c == VK_RIGHT)
				keys &= ~keysRightArrow;
			else if (c == VK_UP)
				keys &= ~keysUpArrow;
			else if (c == VK_DOWN)
				keys &= ~keysDownArrow;

			else if (c == VK_SHIFT)
				keys &= ~keysShift;
			else if (c == VK_ESCAPE)
				keys &= ~keysESC;
			else if (c == VK_RETURN)
				keys &= ~keysEnter;
			else if (c == 'Q' || c == 'q')
				keys &= ~keysDie;

			else
				keys &= ~AnyKeys;
		}
	}

	float _cos(float degree)
	{
		return cos(degree * M_PI / 180);
	}

	float _sin(float degree)
	{
		return sin(degree * M_PI / 180);
	}

	float clamp(float x, float minVal, float maxVal)
	{
		if (x < minVal)
			return minVal;
		else if (x > maxVal)
			return maxVal;
		return x;
	}

	iPoint clamp(iPoint x, iPoint minval, iPoint maxVal)
	{
		return iPointMake(clamp(x.x, minval.x, maxVal.x), clamp(x.y, minval.y, maxVal.y));
	}

	static float _r = 1.0f, _g = 1.0f, _b = 1.0f, _a = 1.0f;
	float uint8ToFloat(uint8 c)
	{
		return c / 255.0f;
	}
	uint8 floatToUint8(float c)
	{
		return (uint8)(c * 0xFF);
	}

	void setRGBA(iColor4f c)
	{
		_r = c.r; _g = c.g; _b = c.b; _a = c.a;
	}

	void setRGBA(iColor4b c)
	{
		_r = uint8ToFloat(c.r); _g = uint8ToFloat(c.g); _b = uint8ToFloat(c.b); _a = uint8ToFloat(c.a);
	}

	void setRGBA(float r, float g, float b, float a)
	{
		_r = r;	_g = g;	_b = b; _a = a;
	}

	void getRGBA(float& r, float& g, float& b, float& a)
	{
		r = _r; g = _g; b = _b; a = _a;
	}

	static float _lineWidth = 1.0f;
	void setLineWidth(float width)
	{
		_lineWidth = width;
	}
	float getLineWidth()
	{
		return _lineWidth;
	}

	void clearRect()
	{
		glClearColor(_r, _g, _b, _a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void setClip(float x, float y, float width, float height)
	{	// #issue
		if (width == 0.0f || height == 0.0f)
		{
			glDisable(GL_SCISSOR_TEST);
			//glScissor(0, 0, devSize.width, devSize.height);
		}
		else
		{
			glEnable(GL_SCISSOR_TEST);
			glScissor(x, y, width, height);
		}
	}

	static float dotSize_ = 1.0f;
	void setDotSize(float size)
	{
		dotSize_ = size;
	}

	void drawDot(float x, float y)
	{
		struct Dot
		{
			float position[4];
			float color[4];
		};
		float radius = dotSize_ / 2;
		Dot dot[4] =
		{
			{ {-radius - 0.5f, -radius - 0.5f,0,1}, {_r,_g,_b,_a}}, {{+radius + 0.5f, -radius - 0.5f,0,1}, {_r,_g,_b,_a}},
			{ {-radius - 0.5f, +radius + 0.5f,0,1}, {_r,_g,_b,_a}}, {{+radius + 0.5f, +radius + 0.5f,0,1}, {_r,_g,_b,_a}},
		};

		GLuint id = vtx->useProgram("dot", "dot");
		glBindBuffer(GL_ARRAY_BUFFER, vtx->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Dot) * 4, dot);

		GLuint attr = glGetAttribLocation(id, "position");
		glEnableVertexAttribArray(attr);
		glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, sizeof(Dot), (const void*)offsetof(Dot, position));
		GLuint attrPosition = attr;

		attr = glGetAttribLocation(id, "color");
		glEnableVertexAttribArray(attr);
		glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, sizeof(Dot), (const void*)offsetof(Dot, color));
		GLuint attrColor = attr;

		GLint loc = glGetUniformLocation(id, "projMatrix");
		glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixProj->d());

		loc = glGetUniformLocation(id, "viewMatrix");
		matrixView->push();
		matrixView->translate(x, y, 0);
		glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixView->d());

		// 좌표값 (x,y) & 반지름 (radius)
		loc = glGetUniformLocation(id, "center");
		glUniform2f(loc, x, devSize.height - y);
		loc = glGetUniformLocation(id, "radius");
		glUniform1f(loc, radius);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vtx->vbe);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(attrPosition);
		glDisableVertexAttribArray(attrColor);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		matrixView->pop();
	}

	void drawCircle(float x, float y, float radius)
	{
		struct Dot
		{
			float position[4];
			float color[4];
		};
		Dot dot[4] =
		{
			{ {-radius - 0.5f, -radius - 0.5f,0,1}, {_r,_g,_b,_a}}, {{+radius + 0.5f, -radius - 0.5f,0,1}, {_r,_g,_b,_a}},
			{ {-radius - 0.5f, +radius + 0.5f,0,1}, {_r,_g,_b,_a}}, {{+radius + 0.5f, +radius + 0.5f,0,1}, {_r,_g,_b,_a}},
		};

		GLuint id = vtx->useProgram("dot", "drawCircle");
		glBindBuffer(GL_ARRAY_BUFFER, vtx->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Dot) * 4, dot);

		GLuint attr = glGetAttribLocation(id, "position");
		glEnableVertexAttribArray(attr);
		glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, sizeof(Dot), (const void*)offsetof(Dot, position));
		GLuint attrPosition = attr;

		attr = glGetAttribLocation(id, "color");
		glEnableVertexAttribArray(attr);
		glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, sizeof(Dot), (const void*)offsetof(Dot, color));
		GLuint attrColor = attr;

		GLint loc = glGetUniformLocation(id, "projMatrix");
		glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixProj->d());

		loc = glGetUniformLocation(id, "viewMatrix");
		matrixView->push();
		matrixView->translate(x, y, 0);
		glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixView->d());

		// 좌표값 (x,y) & 반지름 (radius)
		loc = glGetUniformLocation(id, "center");
		glUniform2f(loc, x, devSize.height - y);
		loc = glGetUniformLocation(id, "radius");
		glUniform1f(loc, radius);
		loc = glGetUniformLocation(id, "lineWidth");
		glUniform1f(loc, _lineWidth);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vtx->vbe);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(attrPosition);
		glDisableVertexAttribArray(attrColor);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		matrixView->pop();
	}
	void fillCircle(float x, float y, float radius)
	{
		float ds = dotSize_;
		setDotSize(radius * 2);

		drawDot(x, y);

		dotSize_ = ds;
	}

	//void drawLine(const iPoint& sp, const iPoint& ep);
	void drawLine(iPoint sp, iPoint ep)
	{
		glLineWidth(_lineWidth);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		iPoint p[2] = { sp, ep };
		glVertexPointer(2, GL_FLOAT, sizeof(iPoint), p);

		iColor4f c[2] = { {_r, _g, _b, _a}, {_r, _g, _b, _a} };
		glColorPointer(4, GL_FLOAT, sizeof(iColor4f), c);

		glDrawArrays(GL_LINES, 0, 2);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
	void drawLine(float x0, float y0, float x1, float y1)
	{
		glLineWidth(_lineWidth);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		iPoint p[2] = { {x0, y0}, {x1, y1} };
		glVertexPointer(2, GL_FLOAT, sizeof(iPoint), p);

		iColor4f c[2] = { {_r, _g, _b, _a}, {_r, _g, _b, _a} };
		glColorPointer(4, GL_FLOAT, sizeof(iColor4f), c);

		glDrawArrays(GL_LINES, 0, 2);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	//void drawRect(const iRect& rt, float radius); 
	void drawRect(iRect rt, float radius)
	{
		drawRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
	}
	void drawRect(float x, float y, float width, float height, float radius)
	{
		drawLine(x, y, x + width, y);
		drawLine(x, y + height, x + width, y + height);
		drawLine(x, y, x, y + height);
		drawLine(x + width, y, x + width, y + height);
	}

	//void fillRect(const iRect& rt, float radius); 
	void fillRect(iRect rt, float radius)
	{
		fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
	}
	void fillRect(float x, float y, float width, float height, float radius)
	{
		float p[4][4] = {
			{-width / 2 - 0.5f, -height / 2 - 0.5f, 0, 1}, {+width / 2 + 0.5f, -height / 2 - 0.5f, 0, 1},
			{-width / 2 - 0.5f, +height / 2 + 0.5f, 0, 1}, {+width / 2 + 0.5f, +height / 2 + 0.5f, 0, 1}
		};
		GLuint id = vtx->useProgram("dot", "fillRect");
		glBindBuffer(GL_ARRAY_BUFFER, vtx->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, p);

		GLuint attr = glGetAttribLocation(id, "position");
		glEnableVertexAttribArray(attr);
		glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, 0, (const void*)0);

		GLint loc = glGetUniformLocation(id, "projMatrix");
		glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixProj->d());

		loc = glGetUniformLocation(id, "viewMatrix");
		matrixView->push();
		matrixView->translate(x + width / 2, y + height / 2, 0);
		glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)matrixView->d());

		loc = glGetUniformLocation(id, "rect");
		glUniform4f(loc, x + width / 2, devSize.height - (y + height / 2), width / 2, height / 2);

		loc = glGetUniformLocation(id, "radius");
		glUniform1f(loc, radius);

		//loc = glGetUniformLocation(id, "lineWidth");
		//glUniform1f(loc, _lineWidth);

		loc = glGetUniformLocation(id, "color");
		glUniform4f(loc, _r, _g, _b, _a);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vtx->vbe);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(attr);
		matrixView->pop();
	}

	Texture** createImageDevide(int numX, int numY, const char* szFormat, ...)
	{
		char szText[1024];
		va_start_end(szText, szFormat);

		wchar_t* ws = utf8_to_utf16(szText);
		Bitmap* bmp = Bitmap::FromFile(ws, false);
		delete ws;

		int width = bmp->GetWidth();
		int height = bmp->GetHeight();
		Rect rect(0, 0, width, height);
		BitmapData bmpData;
		bmp->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

		int stride = bmpData.Stride / 4;
		uint32* rgba = (uint32*)bmpData.Scan0;

		Texture** texs = new Texture * [numX * numY];
		int w = width / numX;
		int h = height / numY;
		int potW = nextPot(w);
		int potH = nextPot(h);
		uint32* pixel = new uint32[potW * potH];
		for (int j = 0; j < numY; j++)
		{
			for (int i = 0; i < numX; i++)
			{
				copyBmp2Rgba(rgba, stride, w * i, h * j, w, h, pixel, w, h);
				texs[numX * j + i] = createImageWithRGBA((uint8*)pixel, w, h);
			}
		}
		delete pixel;

		bmp->UnlockBits(&bmpData);
		delete bmp;

		return texs;
	}

	uint32 nextPot(uint32 x)
	{
		x = x - 1;
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		x = x | (x >> 8);
		x = x | (x >> 16);
		return x + 1;
	}

	void copyBmp2Rgba(uint32* src, int stride, int x, int y, int w, int h,
		uint32* dst, int dw, int dh)
	{
		int potWidth = nextPot(dw);
		int potHeight = nextPot(dh);
		memset(dst, 0x00, sizeof(uint32) * potWidth * potHeight);
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				uint8* d = (uint8*)&dst[potWidth * j + i];
				uint8* s = (uint8*)&src[stride * (y + j) + (x + i)];
				d[0] = s[2];// r
				d[1] = s[1];// g
				d[2] = s[0];// b
				d[3] = s[3];// a
			}
		}
	}

	Texture* createImageWithRGBA(uint8* rgba, int width, int height)
	{
		int potWidth = nextPot(width);
		int potHeight = nextPot(height);

		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		setTexture();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, potWidth, potHeight, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, rgba);
		glBindTexture(GL_TEXTURE_2D, 0);

		Texture* tex = new Texture;
		tex->texID = texID;
		tex->width = width;
		tex->height = height;
		tex->potWidth = potWidth;
		tex->potHeight = potHeight;
		tex->retainCount = 1;
#if CHECK_TEXTURE
		texture_num++;
#endif

		return tex;
	}

	Texture* createTexture(int width, int height, bool rgba32f)
	{
		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		setTexture();
		if (rgba32f == false)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		}
		else
		{
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		}
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, 0);

		Texture* tex = new Texture;
		tex->texID = texID;
		tex->width = width;
		tex->height = height;
		tex->potWidth = width;
		tex->potHeight = height;
		tex->retainCount = 1;
#if CHECK_TEXTURE
		texture_num++;
#endif

		return tex;
	}
#if CHECK_TEXTURE
	int texture_num = 0;
#endif

	Texture* createImage(const char* szFormat, ...)
	{
		char szText[1024];
		va_start_end(szText, szFormat);

		wchar_t* ws = utf8_to_utf16(szText);
		Bitmap* bmp = Bitmap::FromFile(ws, false);
		delete ws;

		int width = bmp->GetWidth();
		int height = bmp->GetHeight();
		Rect rect(0, 0, width, height);
		BitmapData bmpData;
		bmp->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

		int stride = bmpData.Stride / 4;
		uint32* rgba = (uint32*)bmpData.Scan0;

		int potWidth = nextPot(width);
		int potHeight = nextPot(height);
		uint32* pixel = new uint32[potWidth * potHeight];
		copyBmp2Rgba(rgba, stride, 0, 0, width, height,
			pixel, width, height);

		Texture* tex = createImageWithRGBA((uint8*)pixel, width, height);
		delete pixel;

		bmp->UnlockBits(&bmpData);
		delete bmp;

		return tex;
	}

	void freeImage(Texture* tex)
	{
		if (tex->retainCount > 1)
		{
			tex->retainCount--;
			return;
		}
		glDeleteTextures(1, &tex->texID);
		delete tex;
#if CHECK_TEXTURE
		texture_num--;
#endif
	}

	void drawImage(Texture* tex, float x, float y, int anc,
		float ix, float iy, float iw, float ih,
		float rx, float ry, 
		int xyz, float degree, int reverse)
	{
		float w = iw * rx, h = ih * ry;

		switch (anc) {
		case TOP | LEFT:								break;
		case TOP | RIGHT:		x -= w;					break;
		case TOP | HCENTER:		x -= w / 2;				break;

		case BOTTOM | LEFT:					y -= h;		break;
		case BOTTOM | RIGHT:	x -= w;		y -= h;		break;
		case BOTTOM | HCENTER:	x -= w / 2;	y -= h;		break;

		case VCENTER | LEFT:				y -= h / 2;	break;
		case VCENTER | RIGHT:	x -= w;		y -= h / 2;	break;
		case VCENTER | HCENTER:	x -= w / 2;	y -= h / 2;	break;
		}
		iPoint dstPoint[4] = {
		{0, 0},		{w, 0},
		{0, h},		{w, h}
		};
		if (reverse == REVERSE_NONE);
		else if (reverse == REVERSE_WIDTH)
		{
			float t = dstPoint[0].x;
			dstPoint[0].x = dstPoint[1].x;
			dstPoint[1].x = t;

			dstPoint[2].x = dstPoint[0].x;
			dstPoint[3].x = dstPoint[1].x;
		}
		else if (reverse == REVERSE_HEIGHT)
		{
			float t = dstPoint[0].y;
			dstPoint[0].y = dstPoint[2].y;
			dstPoint[2].y = t;

			dstPoint[1].y = dstPoint[0].y;
			dstPoint[3].y = dstPoint[2].y;
		}

		matrixView->push();
		matrixView->translate(x, y, 0);
		iPoint t = iPointMake(x + w / 2, y + h / 2);
		if (xyz == 0)// x
		{
			matrixView->translate(w / 2, h / 2, 0);
			matrixView->rotate(1, 0, 0, degree);
			matrixView->translate(-w / 2, -h / 2, 0);
		}
		else if (xyz == 1)// y
		{
			matrixView->translate(w / 2, h / 2, 0);
			matrixView->rotate(0, 1, 0, degree);
			matrixView->translate(-w / 2, -h / 2, 0);
		}
		else if (xyz == 2)// z
		{
			matrixView->translate(w / 2, h / 2, 0);
			matrixView->rotate(0, 0, 1, degree);
			matrixView->translate(-w / 2, -h / 2, 0);
		}

		iPoint st[4] = {
			{ix / tex->potWidth, iy / tex->potHeight},			{(ix + iw) / tex->potWidth, iy / tex->potHeight},
			{ix / tex->potWidth, (iy + ih) / tex->potHeight},	{(ix + iw) / tex->potWidth, (iy + ih) / tex->potHeight},
		};

		iColor4f c[4] = {
			{_r, _g, _b, _a},	{_r, _g, _b, _a},
			{_r, _g, _b, _a},	{_r, _g, _b, _a},
		};

		GLuint id = vtx->useProgram("alpha", "alpha");
		VertexInfo vi[4];
		for (int i = 0; i < 4; i++)
		{
			memcpy(vi[i].position, &dstPoint[i], sizeof(iPoint));
			vi[i].position[2] = 0;
			vi[i].position[3] = 1;
			memcpy(&vi[i].color, &c[i], sizeof(iColor4f));
			memcpy(&vi[i].uv, &st[i], sizeof(iPoint));
		}
		vtx->bufferSubData(vi, 1, 1);
		GLuint attrP = vtx->enableVertexAttrArray("position", (const void*)offsetof(VertexInfo, position), sizeof(VertexInfo), 4, GL_FLOAT);
		GLuint attrC = vtx->enableVertexAttrArray("color", (const void*)offsetof(VertexInfo, color), sizeof(VertexInfo), 4, GL_FLOAT);
		GLuint attrT = vtx->enableVertexAttrArray("texcoord", (const void*)offsetof(VertexInfo, uv), sizeof(VertexInfo), 2, GL_FLOAT);

		vtx->setUniformMat("projMatrix", matrixProj);
		vtx->setUniformMat("viewMatrix", matrixView);
		vtx->setUniform1i("tex", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex->texID);

		vtx->drawElements();

		vtx->disableVertexAttrArray(attrP);
		vtx->disableVertexAttrArray(attrC);
		vtx->disableVertexAttrArray(attrT);
		vtx->bufferSubData(NULL, 0, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		matrixView->pop();
	}

	void drawImage(Texture* tex, float x, float y, int anc)
	{
		drawImage(tex, x, y, anc, 0, 0, tex->width, tex->height, 1.0f, 1.0f, 0, 0);
	}

	static char* stringName = NULL;
	static float stringSize = 20.0f;
	static float stringLineHeight = 30.0f;
	static float stringR = 1.0f, stringG = 1.0f, stringB = 1.0f, stringA = 1.0f;
	static float stringBorder = 0.0f;
	static float stringBorderR = 1.0f, stringBorderG = 1.0f, stringBorderB = 1.0f, stringBorderA = 1.0f;
	const char* getStringName()
	{
		return stringName;
	}

	void setStringName(const char* str)
	{
		if (stringName)
		{
			if (strcmp(stringName, str) == 0)
				return;

			delete stringName;
		}
		stringName = new char[strlen(str) + 1];
		strcpy(stringName, str);
	}

	float getStringSize()
	{
		return stringSize;
	}
	void setStringSize(float size)
	{
		stringSize = size;
	}

	void setStringLineHeight(float height)
	{
		stringLineHeight = height;
	}
	float getStringLineHeight()
	{
		return stringLineHeight;
	}


	void setStringRGBA(float r, float g, float b, float a)
	{
		stringR = r;
		stringG = g;
		stringB = b;
		stringA = a;
	}
	void getStringRGBA(float& r, float& g, float& b, float& a)
	{
		r = stringR;
		g = stringG;
		b = stringB;
		a = stringA;
	}

	float getStringBorder()
	{
		return stringBorder;
	}
	void setStringBorder(float border)
	{
		stringBorder = border;
	}

	void getStringBorderRGBA(float& r, float& g, float& b, float& a)
	{
		r = stringBorderR;
		g = stringBorderG;
		b = stringBorderB;
		a = stringBorderA;
	}
	void setStringBorderRGBA(float r, float g, float b, float a)
	{
		stringBorderR = r;
		stringBorderG = g;
		stringBorderB = b;
		stringBorderA = a;
	}

	//struct iFont
	//{
	//	float height;
	//	Texture** texs;
	//};
		//loadFont("bm.otf", 20.0f, "0123456789CriticalMiss");
		//drawString(0, 0, top | left, "Critical 1000");
	iFont* loadFont(const char* strOTFTTF, float height, const char* strUse)
	{
		iFont* f = new iFont;
		f->height = height;
		f->interval = 1;
		f->texs = new Texture * [256];
		memset(f->texs, 0x00, sizeof(Texture*) * 256);

		setStringName(strOTFTTF);
		setStringSize(height);
		setStringRGBA(1, 1, 1, 1);
		setStringBorder(0);
		iGraphics* g = iGraphics::share();

		//int i, len = strlen(strUse);
		//for (i = 0; i < len; i++)
		for (int i = 0; strUse[i]; i++)
		{
			char c = strUse[i];
			if (f->texs[c]) continue;

			if (c == ' ')
			{
				g->init(height * 0.6, height);

				f->texs[c] = g->getTexture();
				continue;
			}
			else if (c == ',')
			{
				g->init(height * 0.6, height);

				iRect rt = iGraphics::rectOfString(",");
				g->drawString(0, height - rt.size.height, TOP | LEFT, ",");

				f->texs[c] = g->getTexture();
				continue;
			}

			char str[2] = { c, 0 };
			iRect rt = iGraphics::rectOfString(str);
			g->init(rt.size);

			g->drawString(0, 0, TOP | LEFT, str);

			f->texs[c] = g->getTexture();
		}

		return f;
	}

	void freeFont(iFont* font)
	{
		for (int i = 0; i < 256; i++)
		{
			if (font->texs[i])
				freeImage(font->texs[i]);
		}
		delete font->texs;
		delete font;
	}

	iFont* fontCurr;
	void setFont(iFont* font)
	{
		fontCurr = font;
	}

	void drawString(float x, float y, int anc, const char* szFormat, ...)
	{
		char szText[128];
		va_start_end(szText, szFormat);
		drawString(x, y, 1.0f, 1.0f, anc, szText);
	}

	void drawString(float x, float y, float sx, float sy, int anc, const char* szFormat, ...)
	{
		char szText[128];
		va_start_end(szText, szFormat);

		float h = fontCurr->height;
		float w = 0;
		int i, len = strlen(szText) - 1;
		for (i = 0; i < len; i++)
			w += (fontCurr->texs[szText[i]]->width + fontCurr->interval);
		w += fontCurr->texs[szText[i]]->width;

		switch (anc) {
		case TOP | LEFT:								break;
		case TOP | RIGHT:		x -= w;					break;
		case TOP | HCENTER:		x -= w / 2;				break;

		case BOTTOM | LEFT:					y -= h;		break;
		case BOTTOM | RIGHT:	x -= w;		y -= h;		break;
		case BOTTOM | HCENTER:	x -= w / 2;	y -= h;		break;

		case VCENTER | LEFT:				y -= h / 2;	break;
		case VCENTER | RIGHT:	x -= w;		y -= h / 2;	break;
		case VCENTER | HCENTER:	x -= w / 2;	y -= h / 2;	break;
		}

		Texture* tex;
		len++;
		for (i = 0; i < len; i++)
		{
			tex = fontCurr->texs[szText[i]];
			drawImage(tex, x, y, TOP | LEFT);
			x += (tex->width + fontCurr->interval);
		}
	}


	iRect rectOfString(const char* szFormat, ...)
	{
		return iRectMake(0, 0, 0, 0);
	}

	char* loadFile(const char* fileName, int& length)
	{
		FILE* pf = fopen(fileName, "rb");
		if (!pf)
			return NULL;

		fseek(pf, 0, SEEK_END);
		length = ftell(pf);
		char* buf = new char[length + 1];
		fseek(pf, 0, SEEK_SET);
		fread(buf, sizeof(char), length, pf);
		buf[length] = 0;

		fclose(pf);

		return buf;
	}

	void saveFile(const char* fileName, char* buf, int length)
	{
		FILE* pf = fopen(fileName, "wb");
		fwrite(buf, sizeof(char), length, pf);
		fclose(pf);
	}

	Texture* createBall(float radius)
	{
		float r = radius;

		uint8* rgba = new uint8[32 * 32 * 4];
		for (int j = 0; j < 32; j++)
		{
			for (int i = 0; i < 32; i++)
			{
				uint8* c = &rgba[32 * 4 * j + 4 * i];
				memset(c, 0xFF, sizeof(uint8) * 3);

				float x = i + 0.5f;
				float y = j + 0.5f;
				float d = sqrtf((x - r) * (x - r) + (y - r) * (y - r));
				if (d < r)
					c[3] = 0xFF;
				else
					c[3] = 0;
			}
		}

		Texture* tex = createImageWithRGBA(rgba, 32, 32);
		delete rgba;

		return tex;
	}

	Texture* createBall2(float radius)
	{
		float r = radius + 10;

		uint8* rgba = new uint8[64 * 64 * 4];
		for (int j = 0; j < 64; j++)
		{
			for (int i = 0; i < 64; i++)
			{
				uint8* c = &rgba[64 * 4 * j + 4 * i];
				memset(c, 0xFF, sizeof(uint8) * 3);

				float x = i + 0.5f;
				float y = j + 0.5f;
				float d = sqrtf((x - r) * (x - r) + (y - r) * (y - r));
				if (d < r)
					c[3] = (1.0f - d / r) * 0xFF;
				else
					c[3] = 0;
			}
		}

		Texture* tex = createImageWithRGBA(rgba, 64, 64);
		delete rgba;

		return tex;
	}

	Texture* createTriangle(float width, float height)
	{
		iGraphics* g = iGraphics::share();
		iSize size = iSizeMake(width, height);
		g->init(size);
		g->fillTri(0, 0, size.width, size.height);
		return g->getTexture();
	}
	//namespace
}













