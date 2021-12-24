/**
 * ====================
 *     EntBossHP
 *   File: cookies.inc
 *   Author: Oylsister
 * ==================== 
 */

#define HITMARKER_NONE 0
#define HITMARKER_BOSSONLY 1
#define HITMARKER_ALL 2

#define BOSSHP_HANDLE 0
#define BHUD_HANDLE 1
#define HITMARKER_HANDLE 2

Handle g_hShowBossHPCookie = INVALID_HANDLE;
Handle g_hShowBhudCookie = INVALID_HANDLE;
Handle g_hHitmarkerCookie = INVALID_HANDLE;

bool g_bShowBossHP[MAXPLAYERS+1];
bool g_bShowBhud[MAXPLAYERS+1];
int g_iShowHitmarker[MAXPLAYERS+1];

void CookiesInit()
{
    CookiesCommand();

    g_hShowBossHPCookie = RegClientCookie("entbosshp_showbosshp", "Toggle show hp option", CookieAccess_Protected);
    g_hShowBhudCookie = RegClientCookie("entbosshp_showbhud", "Toggle show bhud option", CookieAccess_Protected);
    g_hHitmarkerCookie = RegClientCookie("entbosshp_hitmarker", "Toggle show hitmarker option", CookieAccess_Protected);

    for (int i = 1; i < MaxClients; i++)
    {
        if(!AreClientCookiesCached(i))
        {
            OnClientCookiesCached(i);
        }
    }
}

void CookiesCommand()
{
    RegConsoleCmd("sm_entbosshp", Command_BossHP);
    RegConsoleCmd("sm_bosshp", Command_BossHP);
    RegConsoleCmd("sm_bhud", Command_BossHP);
}

public Action Command_BossHP(int client, int args)
{
    BossHPCookieMenu(client);
    return Plugin_Handled;
}

void BossHPCookieMenu(int client)
{
    Menu menu = new Menu (BossHPCookieMenuHandler, MENU_ACTIONS_ALL);
    menu.SetTitle("[entBossHP] Setting Menu");
    menu.AddItem("Show BossHP", "Show BossHP");
    menu.AddItem("Show BHud", "Show BHud");
    menu.AddItem("Show Hitmarker", "Show Hitmarker");
    menu.ExitButton = true;
    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int BossHPCookieMenuHandler(Menu menu, MenuAction action, int param1, int param2)
{
    switch(action)
    {
        case MenuAction_DisplayItem:
        {
            char info[64];
            menu.GetItem(param2, info, sizeof(info));
            if(StrEqual(info, "Show BossHP", false))
            {
                char display[64];
                if(!g_bShowBossHP[param1])
                {
                    Format(display, sizeof(display), "%s: No", info);
                    RedrawMenuItem(display);
                }
                else
                {
                    Format(display, sizeof(display), "%s: Yes", info);
                    RedrawMenuItem(display);
                }
            }
            else if(StrEqual(info, "Show BHud", false))
            {
                char display[64];
                if(!g_bShowBhud[param1])
                {
                    Format(display, sizeof(display), "%s: No", info);
                    RedrawMenuItem(display);
                }
                else
                {
                    Format(display, sizeof(display), "%s: Yes", info);
                    RedrawMenuItem(display);
                }
            }
            else
            {
                char display[64];
                if(g_iShowHitmarker[param1] == 0)
                {
                    Format(display, sizeof(display), "%s: No", info);
                    RedrawMenuItem(display);
                }
                else if(g_iShowHitmarker[param1] == 1)
                {
                    Format(display, sizeof(display), "%s: Only Boss", info);
                    RedrawMenuItem(display);
                }
                else
                {
                    Format(display, sizeof(display), "%s: All", info);
                    RedrawMenuItem(display);
                }
            }
        }
        case MenuAction_Select:
        {
            char info[64];
            menu.GetItem(param2, info, sizeof(info));
            if(StrEqual(info, "Show BossHP", false))
            {
                if(!g_bShowBossHP[param1])
                {
                    g_bShowBossHP[param1] = true;
                    PrintToChat(param1, " \x04%s\x01 Showing Boss health has been \x06Enabled.", sTag);
                }
                else
                {
                    g_bShowBossHP[param1] = false;
                    PrintToChat(param1, " \x04%s\x01 Showing Boss health has been \x06Disabled.", sTag);
                }
                SaveClientCookie(param1, BOSSHP_HANDLE);
            }
            else if(StrEqual(info, "Show BHud", false))
            {
                if(!g_bShowBhud[param1])
                {
                    g_bShowBhud[param1] = true;
                    PrintToChat(param1, " \x04%s\x01 Showing Bhud health has been \x06Enabled.", sTag);
                }
                else
                {
                    g_bShowBhud[param1] = false;
                    PrintToChat(param1, " \x04%s\x01 Showing Bhud health has been \x06Disabled.", sTag);
                }
                SaveClientCookie(param1, BHUD_HANDLE);
            }
            else
            {
                if(g_iShowHitmarker[param1] == HITMARKER_ALL)
                {
                    g_iShowHitmarker[param1] = HITMARKER_NONE;
                    PrintToChat(param1, " \x04%s\x01 Showing Hitmarker has been set to \x06None.", sTag);
                }
                else
                {
                    g_iShowHitmarker[param1]++;
                    if(g_iShowHitmarker[param1] == HITMARKER_BOSSONLY)
                    {
                        PrintToChat(param1, " \x04%s\x01 Showing Hitmarker has been set to \x06Boss Only.", sTag);
                    }
                    else
                    {
                        PrintToChat(param1, " \x04%s\x01 Showing Hitmarker has been set to \x06Show All.", sTag);
                    }
                }
                SaveClientCookie(param1, HITMARKER_HANDLE);
            }
        }
    }
}

void BossHPCookiesCached(int client)
{
    char sBossHPCookie[16];
    char sBhudCookie[16];
    char sHitmarkerCookie[16];

    GetClientCookie(client, g_hShowBossHPCookie, sBossHPCookie, sizeof(sBossHPCookie));
    GetClientCookie(client, g_hShowBhudCookie, sBhudCookie, sizeof(sBhudCookie));
    GetClientCookie(client, g_hHitmarkerCookie, sHitmarkerCookie, sizeof(sHitmarkerCookie));

    if(sBossHPCookie[0] != '\0')
    {
        g_bShowBossHP[client] = view_as<bool>(StringToInt(sBossHPCookie));    
    }
    else
    {
        g_bShowBossHP[client] = true;
    }

    if(sBhudCookie[0] != '\0')
    {
        g_bShowBhud[client] = view_as<bool>(StringToInt(sBhudCookie));
    }
    else
    {
        g_bShowBhud[client] = true;
    }

    if(sHitmarkerCookie[0] != '\0')
    {
        g_iShowHitmarker[client] = StringToInt(sHitmarkerCookie);
    }
    else
    {
        g_iShowHitmarker[client] = HITMARKER_BOSSONLY;
    }

    for(int i = 0; i < 3; i++)
    {
        SaveClientCookie(client, i);
    }
}

void SaveClientCookie(int client, int handletype)
{
    char sCookie[16];

    if(handletype == BOSSHP_HANDLE)
    {
        FormatEx(sCookie, sizeof(sCookie), "%b", g_bShowBossHP[client]);
        SetClientCookie(client, g_hShowBossHPCookie, sCookie);
    }
    else if(handletype == BHUD_HANDLE)
    {
        FormatEx(sCookie, sizeof(sCookie), "%b", g_bShowBhud[client]);
        SetClientCookie(client, g_hShowBhudCookie, sCookie);
    }
    else
    {
        FormatEx(sCookie, sizeof(sCookie), "%d", g_iShowHitmarker[client]);
        SetClientCookie(client, g_hHitmarkerCookie, sCookie);
    }
}