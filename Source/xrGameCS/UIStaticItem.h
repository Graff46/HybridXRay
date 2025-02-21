﻿#pragma once

#include "ui/uiabstract.h"
#include "uicustomitem.h"

#include "../xrRender/Public/FactoryPtr.h"

void CreateUIGeom();
void DestroyUIGeom();

class IUIShader;

class CUIStaticItem: public IUISimpleTextureControl, public CUICustomItem
{
    ui_shader hShader;

    Fvector2  iPos;
    u32       dwColor;
    Ivector2  iTile;
    Fvector2  iRem;
    int       alpha_ref;

protected:
    typedef CUICustomItem inherited;

public:
    using CUICustomItem::SetOriginalRect;

#ifdef DEBUG
    shared_str dbg_tex_name;
#endif
    CUIStaticItem();
    virtual ~CUIStaticItem();

    void SetAlphaRef(int val)
    {
        alpha_ref = val;
    };
    virtual void CreateShader(const char* tex, const char* sh = "hud\\default");
    virtual void SetShader(const ui_shader& sh);
    virtual void SetTextureColor(u32 color)
    {
        SetColor(color);
    }
    virtual u32 GetTextureColor() const
    {
        return GetColor();
    }
    virtual void SetOriginalRect(const Frect& r)
    {
        iOriginalRect = r;
        uFlags.set(flValidOriginalRect, TRUE);
    }
    virtual void SetOriginalRectEx(const Frect& r)
    {
        iOriginalRect = r;
        uFlags.set(flValidOriginalRect, TRUE);
        SetRect(0, 0, r.width(), r.height());
    }

    virtual const Frect& GetOriginalRect() const
    {
        return iOriginalRect;
    }

    void    Init(LPCSTR tex, LPCSTR sh, float left, float top, u32 align);

    void    Render();
    void    Render(float angle);

    IC void SetTile(int tile_x, int tile_y, float rem_x, float rem_y)
    {
        R_ASSERT((tile_x >= 0) && (tile_y >= 0) && (rem_x >= 0) && (rem_y >= 0));
        iTile.x = tile_x;
        iTile.y = tile_y;
        iRem.x  = rem_x;
        iRem.y  = rem_y;
    }
    IC void SetPos(float left, float top)
    {
        iPos.set(left, top);
    }
    IC void SetPosX(float left)
    {
        iPos.x = left;
    }
    IC void SetPosY(float top)
    {
        iPos.y = top;
    }

    IC float GetPosX()
    {
        return iPos.x;
    }
    IC float GetPosY()
    {
        return iPos.y;
    }

    IC void SetColor(u32 clr)
    {
        dwColor = clr;
    }
    IC void SetColor(Fcolor clr)
    {
        dwColor = clr.get();
    }
    IC u32 GetColor() const
    {
        return dwColor;
    }
    IC u32& GetColorRef()
    {
        return dwColor;
    }
    ui_shader& GetShader()
    {
        return hShader;
    }
};

extern ENGINE_API BOOL g_bRendering;
