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
#include <dhooks>

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
    version = "Beta 1.0", 
    url = "https://github.com/oylsister/"
};

public void OnPluginStart()
{
    if(GetEngineVersion() != Engine_CSGO)
    {
        SetFailState("current plugin only support for CS:GO!");
        return;
    }

    ConVarInit();
    ArrayInit();
    HookInit();
    CookiesInit();
    DhookInit();
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