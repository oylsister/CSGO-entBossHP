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

    char menutitle[64];
    Format(menutitle, sizeof(menutitle), "%t %t", "prefix menu", "cookie menu choice");
    SetCookieMenuItem(entBossHPMenu_Cookie, 0, menutitle);

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

public void entBossHPMenu_Cookie(int client, CookieMenuAction action, any info, char[] buffer, int maxlen)
{
    switch(action)
    {
        case CookieMenuAction_SelectOption:
        {
            BossHPCookieMenu(client);
        }
    }
}

public Action Command_BossHP(int client, int args)
{
    BossHPCookieMenu(client);
    return Plugin_Handled;
}

void BossHPCookieMenu(int client)
{
    char showhp[64];
    char showbhud[64];
    char showhm[64];

    Menu menu = new Menu (BossHPCookieMenuHandler, MENU_ACTIONS_ALL);
    menu.SetTitle("%T %T", "prefix menu", client, "cookie menu title", client);

    Format(showhp, sizeof(showhp), "%T", "Show BossHP", client);
    Format(showbhud, sizeof(showbhud), "%T", "Show BHud", client);
    Format(showhm, sizeof(showhm), "%T", "Show Hitmarker", client);

    menu.AddItem("Show BossHP", showhp);
    menu.AddItem("Show BHud", showbhud);
    menu.AddItem("Show Hitmarker", showhm);

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
                    Format(display, sizeof(display), "%T %T", "Show BossHP", param1, "no", param1);
                    RedrawMenuItem(display);
                }
                else
                {
                    Format(display, sizeof(display), "%T: %T", "Show BossHP", param1, "yes", param1);
                    RedrawMenuItem(display);
                }
            }
            else if(StrEqual(info, "Show BHud", false))
            {
                char display[64];
                if(!g_bShowBhud[param1])
                {
                    Format(display, sizeof(display), "%T: %T", "Show BHud", param1, "no", param1);
                    RedrawMenuItem(display);
                }
                else
                {
                    Format(display, sizeof(display), "%T: %T", "Show BHud", param1, "yes", param1);
                    RedrawMenuItem(display);
                }
            }
            else
            {
                char display[64];
                if(g_iShowHitmarker[param1] == 0)
                {
                    Format(display, sizeof(display), "%T: %T", "Show Hitmarker", param1, "no", param1);
                    RedrawMenuItem(display);
                }
                else if(g_iShowHitmarker[param1] == 1)
                {
                    Format(display, sizeof(display), "%T: %T", "Show Hitmarker", param1, "Only Boss", param1);
                    RedrawMenuItem(display);
                }
                else
                {
                    Format(display, sizeof(display), "%T: %T", "Show Hitmarker", param1, "All", param1);
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
                    CPrintToChat(param1, "%T %T \x06%T.", "prefix", param1, "Showing Boss health has been", param1, "Enabled", param1);
                }
                else
                {
                    g_bShowBossHP[param1] = false;
                    CPrintToChat(param1, "%T %T \x06%T.", "prefix", param1, "Showing Boss health has been", param1, "Disabled", param1);
                }
                SaveClientCookie(param1, BOSSHP_HANDLE);
            }
            else if(StrEqual(info, "Show BHud", false))
            {
                if(!g_bShowBhud[param1])
                {
                    g_bShowBhud[param1] = true;
                    CPrintToChat(param1, "%T %T \x06%T.", "prefix", param1, "Showing Bhud health has been", param1, "Enabled", param1);
                }
                else
                {
                    g_bShowBhud[param1] = false;
                    CPrintToChat(param1, "%T %T \x06%T.", "prefix", param1, "Showing Bhud health has been", param1, "Disabled", param1);
                }
                SaveClientCookie(param1, BHUD_HANDLE);
            }
            else
            {
                if(g_iShowHitmarker[param1] == HITMARKER_ALL)
                {
                    g_iShowHitmarker[param1] = HITMARKER_NONE;
                    CPrintToChat(param1, "%T %T \x06%T.", "prefix", param1, "Showing Hitmarker has been set to", param1, "None", param1);
                }
                else
                {
                    g_iShowHitmarker[param1]++;
                    if(g_iShowHitmarker[param1] == HITMARKER_BOSSONLY)
                    {
                        CPrintToChat(param1, "%T %T \x06%T.", "prefix", param1, "Showing Hitmarker has been set to", param1, "Boss Only", param1);
                    }
                    else
                    {
                        CPrintToChat(param1, "%T %T \x06%T.", "prefix", param1, "Showing Hitmarker has been set to", param1, "Show All", param1);
                    }
                }
                SaveClientCookie(param1, HITMARKER_HANDLE);
            }
        }
        case MenuAction_Cancel:
        {
            ShowCookieMenu(param1);
        }
        case MenuAction_End:
        {
            delete menu;
        }
    }
    return 0;
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