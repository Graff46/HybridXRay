﻿#include "stdafx.h"
#pragma hdrstop

#include "gametype_chooser.h"

extern ENGINE_API xr_token rpoint_game_type[] = {{"Any game", rpgtGameAny}, {"Deathmatch", rpgtGameDeathmatch}, {"TeamDeathmatch", rpgtGameTeamDeathmatch}, {"ArtefactHunt", rpgtGameArtefactHunt}, {"CaptureTheArtefact", rpgtGameCaptureTheArtefact}, {0, 0}};

#ifndef MASTER_GOLD
bool GameTypeChooser::LoadStream(IReader& F)
{
    m_GameType.assign(F.r_u16());

    return true;
}

bool GameTypeChooser::LoadLTX(CInifile& ini, LPCSTR sect_name, bool bOldFormat)
{
    if (bOldFormat /*version==0x0014*/)
    {
        u8 tmp = ini.r_u8(sect_name, "game_type");
        m_GameType.zero();
        switch (tmp)
        {
            case rpgtGameAny:
                m_GameType.one();
                break;
            case rpgtGameDeathmatch:
                m_GameType.set(eGameIDDeathmatch, TRUE);
                break;
            case rpgtGameTeamDeathmatch:
                m_GameType.set(eGameIDTeamDeathmatch, TRUE);
                break;
            case rpgtGameArtefactHunt:
                m_GameType.set(eGameIDArtefactHunt, TRUE);
                break;
            case rpgtGameCaptureTheArtefact:
                m_GameType.set(eGameIDCaptureTheArtefact, TRUE);
                break;
        }
    }
    else
        m_GameType.assign(ini.r_u16(sect_name, "game_type"));
    return true;
}

void GameTypeChooser::SaveStream(IWriter& F)
{
    F.w_u16(m_GameType.get());
}

void GameTypeChooser::SaveLTX(CInifile& ini, LPCSTR sect_name)
{
    ini.w_u16(sect_name, "game_type", m_GameType.get());
}
#endif