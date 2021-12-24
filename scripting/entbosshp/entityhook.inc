/**
 * ====================
 *     EntBossHP
 *   File: entityhook.inc
 *   Author: Oylsister
 * ==================== 
 */

void HookInit()
{
    HookEntityOutput("math_counter", "OutValue", OnMathCounterOutValue);
    HookEntityOutput("func_physbox_multiplayer", "OnDamaged", OnBreakableDamaged);
    HookEntityOutput("func_physbox", "OnHealthChanged", OnBreakableDamaged);
    HookEntityOutput("func_breakable", "OnHealthChanged", OnBreakableDamaged);
}

public void OnMathCounterOutValue(const char[] output, int caller, int activator, float delay)
{
    g_fClientLastHit[activator] = GetEngineTime();

    if(IsValidEntity(caller))
    {
        char entityname[64];
        int offset;
        int iOutValue;

        GetEntPropString(caller, Prop_Data, "m_iName", entityname, sizeof(entityname));
        offset = FindDataMapInfo(caller, "m_OutValue");

        if(offset == -1)
        {
            return;
        }

        float fOutValue = GetEntDataFloat(caller, offset);

        int index = GetBossEntityIndex(entityname);

        if(index != -1 && g_bLoaded)
        {
            BossType itype = GetBossType(index);

            if(g_iShowHitmarker[activator] > 0)
            {
                ShowHitmarker(activator);
            }

            if(itype == Type_MathCounter)
            {
                SetBossLastChangeTime(index);

                float lastchange = GetBossLastChangeTime(index);

                char bossname[64];
                GetBossName(index, bossname, sizeof(bossname));

                iOutValue = RoundToNearest(fOutValue);

                int iMode = GetMathCounterMode(index);
                int realhp; 

                if(iMode == 1)
                {
                    realhp = g_MathCounterList[index].MC_InitValue + (iOutValue - g_MathCounterList[index].MC_MIN);
                }
                else
                {
                    realhp = g_MathCounterList[index].MC_InitValue + (g_MathCounterList[index].MC_MAX - iOutValue);
                }

                SetCurrentBossHP(index, realhp);

                if (lastchange + 5.0 > GetEngineTime())
                {
                    int currenthp = GetCurrentBossHP(index);
                    int maxhp = GetBossMaxHP(index);

                    if(currenthp > maxhp)
                    {
                        SetBossMaxHP(index, currenthp);
                    }

                    if(currenthp <= 0)
                    {
                        SetBossMaxHP(index, 0);
                    }

                    PushBossDataToArray(bossname, currenthp, maxhp);
                }
            }
            else if(itype == Type_HPBar)
            {
                SetBossLastChangeTime(index);

                float lastchange = GetBossLastChangeTime(index);

                char bossname[64];
                GetBossName(index, bossname, sizeof(bossname));

                iOutValue = RoundToNearest(fOutValue);

                int iteratormode = GetIteratorMode(index);
                if(iteratormode == 1)
                {
                    g_HPBarList[index].HPBar_Multi = g_HPBarList[index].HPBar_InitValue - g_HPBarList[index].HPBar_Min;
                }
                else
                {
                    g_HPBarList[index].HPBar_Multi = g_HPBarList[index].HPBar_Max - g_HPBarList[index].HPBar_InitValue;
                }

                if(g_HPBarList[index].HPBar_Multi <= 0)
                {
                    SetCurrentBossHP(index, 0);
                }
                else
                {
                    int iMode = GetMathCounterMode(index);
                    int realhp; 
                    int multi = g_HPBarList[index].HPBar_Multi;

                    if(iMode == 1)
                    {
                        realhp = (multi - 1) * g_MathCounterList[index].MC_InitValue + (iOutValue - g_MathCounterList[index].MC_MIN);
                    }
                    else
                    {
                        realhp = (multi - 1) * g_MathCounterList[index].MC_InitValue + (g_MathCounterList[index].MC_MAX - iOutValue);
                    }

                    SetCurrentBossHP(index, realhp);
                }

                if (lastchange + 5.0 > GetEngineTime())
                {
                    int currenthp = GetCurrentBossHP(index);
                    int maxhp = GetBossMaxHP(index);

                    if(currenthp > maxhp)
                    {
                        SetBossMaxHP(index, currenthp);
                    }

                    if(currenthp <= 0)
                    {
                        SetBossMaxHP(index, 0);
                    }

                    PushBossDataToArray(bossname, currenthp, maxhp);
                }
            }
        }
        else
        {
            if(g_fLastShowHP + 3.0 < GetEngineTime() && IsValidEntity(caller) && activator > 0 && activator <= MAXPLAYERS)
            {
                if(g_fClientLastHit[activator] < GetEngineTime() - 0.1)
                {
                    return;
                }

                GetEntPropString(caller, Prop_Data, "m_iName", g_sClientEntityHitName[activator], 64);

                if(strlen(g_sClientEntityHitName[activator]) == 0)
                {
                    Format(g_sClientEntityHitName[activator], 64, "Health");
                }
                g_iClientEntityHit[activator] = 0;

                if(g_iShowHitmarker[activator] == HITMARKER_ALL)
                {
                    ShowHitmarker(activator);
                }

                if(fOutValue > 0.0)
                {
                    SendNormalBhud(activator, 0, entityname, RoundToNearest(fOutValue));
                }
            }
        }
    }
}

public void OnBreakableDamaged(const char[] output, int caller, int activator, float delay)
{
    g_fClientLastHit[activator] = GetEngineTime();

    if(IsValidEntity(caller))
    {
        char entityname[64];
        int iHealth;

        GetEntPropString(caller, Prop_Data, "m_iName", entityname, sizeof(entityname));
        iHealth = GetEntProp(caller, Prop_Data, "m_iHealth");

        int index = GetBossEntityIndex(entityname);

        if(index != -1 && g_bLoaded)
        {
            if(g_iShowHitmarker[activator] > 0)
            {
                ShowHitmarker(activator);
            }

            BossType itype = GetBossType(index);

            if(itype != Type_Breakable)
            {
                return;
            }

            if(iHealth <= 0)
            {
                iHealth = 0;
            }

            SetBossLastChangeTime(index);

            float lastchange = GetBossLastChangeTime(index);

            char bossname[64];
            GetBossName(index, bossname, sizeof(bossname));

            SetCurrentBossHP(index, iHealth);

            // Put the boss data on the Array
            if (lastchange + 5.0 > GetEngineTime())
            {
                int currenthp = GetCurrentBossHP(index);
                int maxhp = GetBossMaxHP(index);

                if(currenthp > maxhp)
                {
                    SetBossMaxHP(index, currenthp);
                }

                if(currenthp <= 0)
                {
                    SetBossMaxHP(index, 0);
                }

                PushBossDataToArray(bossname, currenthp, maxhp);
            }

            // Print Single Boss Format
            if(array_hp.Length == 1)
            {
                SendBossHP(index, itype, iHealth);
            }

            // Print Multi Boss Format
            else
            {
                SendMultiBossHP(activator, iHealth);
            }
        }
        else
        {
            if(g_fLastShowHP + 3.0 < GetEngineTime() && g_fClientLastHit[activator] > GetEngineTime() - 0.2)
            {
                if(g_iClientEntityHit[activator] != caller)
                {
                    GetEntPropString(caller, Prop_Data, "m_iName", g_sClientEntityHitName[activator], 64);

                    if(strlen(g_sClientEntityHitName[activator]) == 0)
                    {
                        Format(g_sClientEntityHitName[activator], 64, "Health");
                    }
                    g_iClientEntityHit[activator] = caller;
                }
                if(g_iShowHitmarker[activator] == HITMARKER_ALL)
                {
                    ShowHitmarker(activator);
                }

                SendNormalBhud(activator, caller, entityname, iHealth);
            }
        }
    }
}

public void OnEntityCreated(int entity, const char[] entityclass)
{
    if(!g_bLoaded)
    {
        return;
    }

    if(StrEqual(entityclass, "math_counter", false))
    {
        if(g_hAcceptInput != INVALID_HANDLE)
        {
            DHookEntity(g_hAcceptInput, false, entity);
        }
        CreateTimer(0.0, GetMathCounterInfo, entity);
    }
}

public Action GetMathCounterInfo(Handle timer, any entity)
{
    if(!IsValidEntity(entity) && !IsValidEdict(entity))
    {
        return;
    }

    char entityname[64];
    GetEntPropString(entity, Prop_Data, "m_iName", entityname, sizeof(entityname));

    for(int i = 0; i < g_iTotal; i++)
    {
        BossType type = GetBossType(i);

        if(type == Type_MathCounter || type == Type_HPBar)
        {
            if(StrEqual(entityname, g_BossList[i].BOSS_ENTITY, false))
            {
                g_MathCounterList[i].MC_MIN = RoundToNearest(GetEntPropFloat(entity, Prop_Data, "m_flMin"));
                g_MathCounterList[i].MC_MAX = RoundToNearest(GetEntPropFloat(entity, Prop_Data, "m_flMax"));
                g_MathCounterList[i].MC_InitValue = RoundToNearest(GetEntDataFloat(entity, FindDataMapInfo(entity, "m_OutValue")));

                if(g_MathCounterList[i].MC_MODE == -1)
                {
                    if(GetEntProp(entity, Prop_Data, "m_bHitmin"))
                    {
                        g_MathCounterList[i].MC_MODE = 2;
                    }
                    else
                    {
                        g_MathCounterList[i].MC_MODE = 1;
                    }
                }
                break;
            }
        }
        if(type == Type_HPBar)
        {
            if(StrEqual(entityname, g_BossList[i].BOSS_ITERATOR, false))
            {
                g_HPBarList[i].HPBar_Min = RoundToNearest(GetEntPropFloat(entity, Prop_Data, "m_flMin"));
                g_HPBarList[i].HPBar_Max = RoundToNearest(GetEntPropFloat(entity, Prop_Data, "m_flMax"));
                g_HPBarList[i].HPBar_InitValue = RoundToNearest(GetEntDataFloat(entity, FindDataMapInfo(entity, "m_OutValue")));

                if(g_HPBarList[i].HPBar_Mode == -1)
                {
                    if(GetEntProp(entity, Prop_Data, "m_bHitmin"))
                    {
                        g_HPBarList[i].HPBar_Mode = 2;
                    }
                    else
                    {
                        g_HPBarList[i].HPBar_Mode = 1;
                    }
                }
                break;
            }

            if(StrEqual(entityname, g_BossList[i].BOSS_BACKUP, false))
            {
                g_HPBarList[i].HPBar_Backup_Min = RoundToNearest(GetEntPropFloat(entity, Prop_Data, "m_flMin"));
                g_HPBarList[i].HPBar_Backup_Max = RoundToNearest(GetEntPropFloat(entity, Prop_Data, "m_flMax"));
                g_HPBarList[i].HPBar_Backup_InitValue = RoundToNearest(GetEntDataFloat(entity, FindDataMapInfo(entity, "m_OutValue")));
                break;
            }
        }
    }
}

void PushBossDataToArray(const char[] bossname, int currenthp, int maxhp)
{
    array_bossname.PushString(bossname);
    array_hp.Push(currenthp);
    array_maxhp.Push(maxhp);
}