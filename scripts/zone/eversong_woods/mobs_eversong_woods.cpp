/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Mobs_Mana_Tapped
SD%Complete: 100
SDComment: For quest 8346
SDCategory: Eversong Woods
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"

// **** This script is still under Developement ****

struct MANGOS_DLL_DECL mobs_mana_tappedAI : public ScriptedAI
{
    mobs_mana_tappedAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
    }

    void Aggro(Unit *who)
    {
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (caster->GetTypeId() == TYPEID_PLAYER)
        {
            if( ((Player*)caster)->GetQuestStatus(8346) == QUEST_STATUS_INCOMPLETE && !((Player*)caster)->GetReqKillOrCastCurrentCount(8346, m_creature->GetEntry()) && spell->Id == 28734)
            {
                ((Player*)caster)->CastedCreatureOrGO(15468, m_creature->GetGUID(), 28734);
            }
        }
        return;
    }
}; 
CreatureAI* GetAI_mobs_mana_tapped(Creature *_Creature)
{
    return new mobs_mana_tappedAI (_Creature);
}

void AddSC_npcs_eversong_woods()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mobs_mana_tapped";
    newscript->GetAI = GetAI_mobs_mana_tapped;
    m_scripts[nrscripts++] = newscript;
}