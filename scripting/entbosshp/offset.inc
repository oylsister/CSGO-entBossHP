/**
 * ====================
 *     EntBossHP
 *   File: offset.inc
 *   Author: Oylsister
 * ==================== 
 */

#include <dhooks>

Handle g_hAcceptInput = INVALID_HANDLE;

void OffsetInit()
{
    if(GetEngineVersion() != Engine_CSGO)
    {
        SetFailState("current plugin only support for csgo!");
        return;
    }

    GameData hGameData = new GameData("sdktools.games/engine.csgo");

    if(hGameData == null)
    {
        SetFailState("make sure you have gamedata file \"sdktools.games/engine.csgo.txt\"!");
        return;
    }

    int offset = GameConfGetOffset(hGameData, "AcceptInput");
    g_hAcceptInput = DHookCreate(offset, HookType_Entity, ReturnType_Bool, ThisPointer_CBaseEntity, AcceptInput);
    DHookAddParam(g_hAcceptInput, HookParamType_CharPtr);
    DHookAddParam(g_hAcceptInput, HookParamType_CBaseEntity);
    DHookAddParam(g_hAcceptInput, HookParamType_CBaseEntity);
    DHookAddParam(g_hAcceptInput, HookParamType_Object, 20);
    DHookAddParam(g_hAcceptInput, HookParamType_Int);

    delete hGameData;
}

public MRESReturn AcceptInput(int entity, Handle hReturn, Handle hParams)
{
    if(!IsValidEntity(entity))
    {
        return MRES_Ignored;
    }

    char entityname[64];
    char command[16];
    char sParam[16];

    GetEntPropString(entity, Prop_Data, "m_iName", entityname, sizeof(entityname));

    DHookGetParamString(hParams, 1, command, sizeof(command));
    int type = -1;
    int param = 0;

    type = DHookGetParamObjectPtrVar(hParams, 4, 16, ObjectValueType_Int);

    if(type == 1)
    {
        param = DHookGetParamObjectPtrVar(hParams, 4, 0, ObjectValueType_Int);
    }
    else if(type == 2)
    {
        DHookGetParamObjectPtrString(hParams, 4, 0, ObjectValueType_String, sParam, sizeof(sParam));
        StringToIntEx(sParam, param);
    }

    if(StrEqual(command, "SetValueNoFire", false))
    {
        for(int i = 0; i < g_iTotal; i++)
        {
            BossType type_boss = GetBossType(i);

            if(type_boss == Type_HPBar)
            {
                if(StrEqual(entityname, g_BossList[i].BOSS_ITERATOR, false))
                {
                    if(g_MathCounterList[i].MC_MODE == 1)
                    {
                        g_HPBarList[i].HPBar_Multi = param - g_HPBarList[i].HPBar_Min;
                    }
                    else
                    {
                        g_HPBarList[i].HPBar_Multi = g_HPBarList[i].HPBar_Max - param;
                    }
                    break;
                }
                if(StrEqual(entityname, g_BossList[i].BOSS_BACKUP, false))
                {
                    g_HPBarList[i].HPBar_Backup = param;
                    break;
                }
            }
        }
    }
    else if(StrEqual(command, "SetHitMax", false))
    {
        for(int i = 0; i < g_iTotal; i++)
        {
            BossType type_boss = GetBossType(i);

            if(type_boss == Type_HPBar || type_boss == Type_MathCounter)
            {
                if(StrEqual(entityname, g_BossList[i].BOSS_ENTITY, false))
                {
                    g_MathCounterList[i].MC_MAX = param;
                    break;
                }
            }

            if(type_boss == Type_HPBar)
            {
                if(StrEqual(entityname, g_BossList[i].BOSS_ITERATOR, false))
                {
                    g_HPBarList[i].HPBar_Max = param;
                    break;
                }
                if(StrEqual(entityname, g_BossList[i].BOSS_BACKUP, false))
                {
                    g_HPBarList[i].HPBar_Backup_Max = param;
                    break;
                }
            }
        }
    }
    else if(StrEqual(command, "SetHitMin", false))
    {
        for(int i = 0; i < g_iTotal; i++)
        {
            BossType type_boss = GetBossType(i);

            if(type_boss == Type_HPBar || type_boss == Type_MathCounter)
            {
                if(StrEqual(entityname, g_BossList[i].BOSS_ENTITY, false))
                {
                    g_MathCounterList[i].MC_MIN = param;
                    break;
                }
            }

            if(type_boss == Type_HPBar)
            {
                if(StrEqual(entityname, g_BossList[i].BOSS_ITERATOR, false))
                {
                    g_HPBarList[i].HPBar_Min = param;
                    break;
                }
                if(StrEqual(entityname, g_BossList[i].BOSS_BACKUP, false))
                {
                    g_HPBarList[i].HPBar_Backup_Min = param;
                    break;
                }
            }
        }
    }
    return MRES_Ignored;
}