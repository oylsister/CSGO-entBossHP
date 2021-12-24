/**
 * ====================
 *     EntBossHP
 *   File: event.inc
 *   Author: Oylsister
 * ==================== 
 */

void EventInit()
{
    HookEvent("round_start", OnRoundStart);
}

public void OnRoundStart(Event event, const char[] name, bool dontBroadcast)
{
    if(g_bLoaded)
    {
        for(int i = 0; i < g_iTotal; i++)
        {
            SetCurrentBossHP(i, 0);
            g_HPBarList[i].HPBar_Multi = 1;
            g_HPBarList[i].HPBar_Backup = 0;
            SetBossMaxHP(i, 0);
        }

        for(int client = 1; client < MaxClients; client++)
        {
            if(IsClientInGame(client))
            {
                g_iClientEntityHit[client] = -1;
                g_sClientEntityHitName[client] = '\0';

                CPrintToChat(client, "%T %T", "prefix", client, "round start", client);
            }
        }
    }
}