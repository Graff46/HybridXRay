﻿#pragma once
class UIMainForm: public xrUI
{
public:
    UIMainForm();
    virtual ~UIMainForm();
    virtual void      Draw();
    bool              Frame();
    IC UILeftBarForm* GetLeftBarForm()
    {
        return m_LeftBar;
    }
    IC UITopBarForm* GetTopBarForm()
    {
        return m_TopBar;
    }
    IC UIRightBarForm* GetRightBarForm()
    {
        return m_RightBar;
    }

private:
    UITopBarForm*   m_TopBar;
    UIRenderForm*   m_Render;
    UIMainMenuForm* m_MainMenu;
    UILeftBarForm*  m_LeftBar;
    UIRightBarForm* m_RightBar;
};
extern UIMainForm* MainForm;
