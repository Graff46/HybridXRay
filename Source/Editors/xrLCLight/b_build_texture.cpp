﻿#include "stdafx.h"
#include "b_build_texture.h"
#include "serialize.h"
#include "StbImage\stb_image.h"
/*
struct b_texture
{
    string128			name;
    u32					dwWidth;
    u32					dwHeight;
    BOOL				bHasAlpha;
    u32*				pSurface;
};
STextureParams	THM;
*/

void clear(b_BuildTexture& texture)
{
    texture.pSurface.Clear();
}

void b_BuildTexture::read(INetReader& r)
{
    ::read(r, *this);
    THM.read(r);
}

void b_BuildTexture::write(IWriter& w) const
{
    ::write(w, *this);
    THM.write(w);
}
