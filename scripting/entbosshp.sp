/**
 * ====================
 *     EntBossHP
 *   File: entbosshp.sp
 *   Author: Oylsister
 * ==================== 
 */

#pragma semicolon 1

#include <sourcemod>
#include <sdktools>
#include <sdkhooks>
#include <cstrike>
#include <clientprefs>
#include <csgocolors_fix>

#pragma newdecls required

#include "entbosshp/cvar.inc"
#include "entbosshp/cookies.inc"
#include "entbosshp/config.inc"
#include "entbosshp/function.inc"
#include "entbosshp/offset.inc"
#include "entbosshp/entityhook.inc"
#include "entbosshp/event.inc"

public Plugin myinfo =
{
    name = "[CS:GO] EntBossHP", 
    author = "Oylsister", 
    description = "Showing Entity Health from the map", 
    version = "Beta 2.0", 
    url = "https://github.com/oylsister/"
};

public void OnPluginStart()
{
    if(GetEngineVersion() != Engine_CSGO)
    {
        SetFailState("current plugin only support for CS:GO!");
        return;
    }

    RegConsoleCmd("reloadbosshp", Command_ReloadConfig, "Reload Command", ADMFLAG_CONFIG);

    LoadTranslations("entbosshp.phrases");

    ConVarInit();
    ArrayInit();
    HookInit();
    CookiesInit();
    OffsetInit();
    EventInit();
}

public void OnMapStart()
{
    LoadBossConfig();
}

public void OnClientCookiesCached(int client)
{
    BossHPCookiesCached(client);
}

public Action Command_ReloadConfig(int client, int args)
{
    LoadBossConfig();
    CReplyToCommand(client, "%T %T", "prefix", client, "reload successful", client);
    return Plugin_Handled;
}