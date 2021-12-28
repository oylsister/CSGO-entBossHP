/**
 * ====================
 *     EntBossHP
 *   File: function.inc
 *   Author: Oylsister
 * ==================== 
 */

ArrayList array_bossname;
ArrayList array_hp;
ArrayList array_maxhp;

float g_fClientLastHit[MAXPLAYERS+1];
int g_iClientEntityHit[MAXPLAYERS+1];
char g_sClientEntityHitName[MAXPLAYERS+1];

enum struct BossTempData
{
    float data_lastchangetime;
    int data_maxhp;
    int data_currenthp;
}

BossTempData g_iTempData[48];

float g_fLastShowHP;

void ArrayInit()
{
    array_bossname = new ArrayList(64);
    array_hp = new ArrayList();
    array_maxhp = new ArrayList();
}

void SendBossHPText(int client, int health)
{
    float g_fTime = GetEngineTime();

    if(g_fClientLastHit[client] > g_fTime - 3.0 && g_fLastShowHP + 0.1 < g_fTime || health == 0)
    {
        ClearAllArray();

        char message[256];
        char bossname[64];
        int currenthp;
        int maxhp;

        message[0] = '\0';

        for (int index = 0; index < array_hp.Length; index++)
        {
            GetArrayBossName(index, bossname, sizeof(bossname));
            currenthp = GetArrayBossHP(index);
            maxhp = GetArrayBossMaxHP(index);

            float percent = float(currenthp) / float(maxhp) * 100.0;

            Format(message, sizeof(message), "%s%s: %dHP (%.1f%%)\n", message, bossname, currenthp, percent);

            if(currenthp == 0)
            {
                Format(message, sizeof(message), "%s%s: 0HP (DEAD)\n", message, bossname);
            }
        }

        for (int i = 1; i <= MaxClients; i++)
        {
            if(IsClientInGame(i) && g_bShowBossHP[i])
            {
                PrintHintText(i, "%s", message);
            }
        }
    }
    g_fLastShowHP = g_fTime;
}

void SendNormalBhud(int client, int entity, const char[] entityname, int health)
{
    char message[64];
    Format(message, sizeof(message), "%s: %d", entityname, health);

    float g_fTime = GetEngineTime();
    float g_fLastHit;

    if(g_fClientLastHit[client] > g_fTime - 3.0 && g_fLastHit + 0.1 < g_fTime)
    {
        int g_iCount = 0;
        int g_iTotalCT = 0;

        for(int i = 1; i < MaxClients; i++)
        {
            if(GetClientTeam(i) == CS_TEAM_CT)
            {
                g_iTotalCT++;
                if(g_fClientLastHit[i] > g_fTime - 7.0 && g_iClientEntityHit[i] == entity && StrEqual(g_sClientEntityHitName[i], entityname, false))
                {
                    g_iCount++;
                }
            }
        }

        if(g_iCount > g_iTotalCT / 2)
        {
            for(int i = 1; i < MaxClients; i++)
            {
                if(IsClientInGame(i) && g_bShowBhud[i])
                {
                    PrintHintText(i, message);
                }
            }
        }

        else
        {
            for(int i = 1; i < MaxClients; i++)
            {
                if(g_fClientLastHit[i] > g_fTime - 7.0 && g_iClientEntityHit[i] == entity && StrEqual(g_sClientEntityHitName[i], entityname, false))
                {
                    if(IsClientInGame(i) && g_bShowBhud[i])
                    {
                        PrintHintText(i, message);
                    }
                }
            }
        }
        g_fLastShowHP = g_fTime;
    }
}

void ShowHitmarker(int client)
{
    SetHudTextParams(-1.0, -1.0, 0.1, 255, 0, 0, 255, 1, 0.0, 0.1, 0.1);
    ShowHudText(client, g_iHitMarkerChannel, "◞  ◟\n◝  ◜");
}

void SetClientCash(int client)
{
    int cash = GetEntProp(client, Prop_Send, "m_iAccount");
    SetEntProp(client, Prop_Send, "m_iAccount", cash + g_iReward);
}

stock int GetBossEntityIndex(const char[] entityname)
{
    for (int i = 0; i < g_iTotal; i++)
    {
        if (StrEqual(entityname, g_BossList[i].BOSS_ENTITY, false))
        {
            return i;
        }
    }
    return -1;
}

void ClearAllArray()
{
    array_bossname.Clear();
    array_hp.Clear();
    array_maxhp.Clear();
}

// Config Data

stock BossType GetBossType(int index)
{
    return g_BossList[index].BOSS_TYPE;
}

stock int GetBossName(int index, char[] name, int maxlen)
{
    if(g_BossList[index].BOSS_NAME[0] != '\0')
    {
        return strcopy(name, maxlen, g_BossList[index].BOSS_NAME);
    }
    else
    {
        return strcopy(name, maxlen, "Boss");
    }
}

stock int GetBossEntityName(int index, char[] name, int maxlen)
{
    return strcopy(name, maxlen, g_BossList[index].BOSS_ENTITY);
}

stock int GetMathCounterMode(int index)
{
    return g_MathCounterList[index].MC_MODE;
}

stock int GetIteratorMode(int index)
{
    return g_HPBarList[index].HPBar_Mode;
}


// Temporary Data

stock void SetBossLastChangeTime(int index)
{
    g_iTempData[index].data_lastchangetime = GetEngineTime();
}

stock float GetBossLastChangeTime(int index)
{
    return g_iTempData[index].data_lastchangetime;
}

stock int GetCurrentBossHP(int index)
{
    return g_iTempData[index].data_currenthp;
}

stock void SetCurrentBossHP(int index, int health)
{
    g_iTempData[index].data_currenthp = health;
}

stock int SetBossMaxHP(int index, int value)
{
    g_iTempData[index].data_maxhp = value;
}

stock int GetBossMaxHP(int index)
{
    return g_iTempData[index].data_maxhp;
}

// Aray Data
stock int GetArrayBossName(int arrayindex, char[] buffer, int maxlen)
{
    return array_bossname.GetString(arrayindex, buffer, maxlen);
}

stock int GetArrayBossHP(int arrayindex)
{
    return array_hp.Get(arrayindex);
}

stock int GetArrayBossMaxHP(int arrayindex)
{
    return array_maxhp.Get(arrayindex);
}
