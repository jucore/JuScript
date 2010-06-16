/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Swamp_of_Sorrows
SD%Complete: 100
SDComment: Quest support: 1393
SDCategory: Swap of Sorrows
EndScriptData */

/* ContentData
npc_galen_goodward
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"

/*######
## npc_galen_goodward
######*/

enum Galen
{
    QUEST_GALENS_ESCAPE     = 1393,

    GO_GALENS_CAGE          = 37118,

    SAY_PERIODIC            = -1000588,
    SAY_QUEST_ACCEPTED      = -1000587,
    SAY_ATTACKED_1          = -1000586,
    SAY_ATTACKED_2          = -1000585,
    SAY_QUEST_COMPLETE      = -1000584,
    EMOTE_WHISPER           = -1000583,
    EMOTE_DISAPPEAR         = -1000582
};

struct MANGOS_DLL_DECL npc_galen_goodwardAI : public npc_escortAI
{
    npc_galen_goodwardAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_uiGalensCageGUID = 0;
        Reset();
    }

    uint64 m_uiGalensCageGUID;
    uint32 m_uiPeriodicSay;

    void Reset()
    {
        m_uiPeriodicSay = 6000;
    }

    void Aggro(Unit* pWho)
    {
        if (HasEscortState(STATE_ESCORT_ESCORTING))
            DoScriptText(urand(0, 1) ? SAY_ATTACKED_1 : SAY_ATTACKED_2, m_creature, pWho);
    }

    void WaypointStart(uint32 uiPointId)
    {
        switch (uiPointId)
        {
            case 0:
                {
                    GameObject* pCage = NULL;
                    if (m_uiGalensCageGUID)
                        pCage = m_creature->GetMap()->GetGameObject(m_uiGalensCageGUID);
                    else
                        pCage = GetClosestGameObjectWithEntry(m_creature, GO_GALENS_CAGE, INTERACTION_DISTANCE);
                    if (pCage)
                    {
                        pCage->UseDoorOrButton();
                        m_uiGalensCageGUID = pCage->GetGUID();
                    }
                    break;
                }
            case 21:
                DoScriptText(EMOTE_DISAPPEAR, m_creature);
                break;
        }
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch (uiPointId)
        {
            case 0:
                if (GameObject* pCage = m_creature->GetMap()->GetGameObject(m_uiGalensCageGUID))
                    pCage->ResetDoorOrButton();
                break;
            case 20:
                if (Player* pPlayer = GetPlayerForEscort())
                {
                    m_creature->SetFacingToObject(pPlayer);
                    DoScriptText(SAY_QUEST_COMPLETE, m_creature, pPlayer);
                    DoScriptText(EMOTE_WHISPER, m_creature, pPlayer);
                    pPlayer->GroupEventHappens(QUEST_GALENS_ESCAPE, m_creature);
                }
                SetRun(true);
                break;
        }
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {

        if (m_uiPeriodicSay < uiDiff)
        {
            if (HasEscortState(STATE_ESCORT_NONE))
                DoScriptText(SAY_PERIODIC, m_creature);
            m_uiPeriodicSay = 6000;
        }
        else
            m_uiPeriodicSay -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

bool QuestAccept_npc_galen_goodward(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_GALENS_ESCAPE)
    {

        if (npc_galen_goodwardAI* pEscortAI = dynamic_cast<npc_galen_goodwardAI*>(pCreature->AI()))
        {
            pEscortAI->Start(true, false, pPlayer->GetGUID(), pQuest);
            pCreature->setFaction(FACTION_ESCORT_N_NEUTRAL_ACTIVE);
            DoScriptText(SAY_QUEST_ACCEPTED, pCreature);
        }
    }
    return true;
}

CreatureAI* GetAI_npc_galen_goodward(Creature* pCreature)
{
    return new npc_galen_goodwardAI(pCreature);
}

void AddSC_swamp_of_sorrows()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "npc_galen_goodward";
    newscript->GetAI = &GetAI_npc_galen_goodward;
    newscript->pQuestAccept = &QuestAccept_npc_galen_goodward;
    newscript->RegisterSelf();
}