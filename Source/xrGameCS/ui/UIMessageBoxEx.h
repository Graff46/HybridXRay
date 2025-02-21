﻿#pragma once
#include "UIDialogWnd.h"
#include "UIWndCallback.h"

class CUIMessageBox;

class CUIMessageBoxEx: public CUIDialogWnd, public CUIWndCallback
{
public:
    CUIMessageBoxEx();
    virtual ~CUIMessageBoxEx();
    void                          SetText(LPCSTR text);
    LPCSTR                        GetText();
    virtual void                  InitMessageBox(LPCSTR xml_template);
    virtual void                  SendMessage(CUIWindow* pWnd, s16 msg, void* pData = NULL);

    LPCSTR                        GetHost();
    LPCSTR                        GetPassword();

    void                          SetTextEditURL(LPCSTR text);
    LPCSTR                        GetTextEditURL();

    CUIWndCallback::void_function func_on_ok;
    void                          OnOKClicked(CUIWindow*, void*);

    virtual bool                  IR_OnKeyboardPress(int dik);

    CUIMessageBox*                m_pMessageBox;
};
