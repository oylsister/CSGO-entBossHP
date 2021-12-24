/**
 * ====================
 *     EntBossHP
 *   File: cvar.inc
 *   Author: Oylsister
 * ==================== 
 */

char sTag[32];

ConVar g_Cvar_Prefix;

void ConVarInit()
{
    g_Cvar_Prefix = CreateConVar("entbosshp_prefix", "[entBossHP]", "Prefix for this plugin");

    HookConVarChange(g_Cvar_Prefix, OnPrefixChange);

    AutoExecConfig(true, "entbosshp");
}

public void OnPrefixChange(ConVar cvar, char[] oldValue, char[] newValue)
{
    GetConVarString(g_Cvar_Prefix, sTag, sizeof(sTag));
}

