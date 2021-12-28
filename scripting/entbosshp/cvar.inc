/**
 * ====================
 *     EntBossHP
 *   File: cvar.inc
 *   Author: Oylsister
 * ==================== 
 */

ConVar g_Cvar_HitmarkerChannel;
ConVar g_Cvar_Reward;
ConVar g_Cvar_ConfigMode;

int g_iHitMarkerChannel;
int g_iReward;
int g_iConfigMode;

void ConVarInit()
{
    g_Cvar_HitmarkerChannel = CreateConVar("entbosshp_hitmarkerchannel", "0.0", "Specify hitmarker channel for this plugin", _, true, 0.0, true, 6.0);
    g_Cvar_Reward = CreateConVar("entbosshp_reward", "10.0", "Specify cash reward for damaging boss", _, true, 0.0, false);
    g_Cvar_ConfigMode = CreateConVar("entbosshp_configmode", "2.0", "Specify cash reward for damaging boss (1 = Original| 2 = GFL Config)", _, true, 1.0, true, 2.0);

    HookConVarChange(g_Cvar_HitmarkerChannel, OnHitmarkerChannelChanged);
    HookConVarChange(g_Cvar_Reward, OnRewardChanged);
    HookConVarChange(g_Cvar_ConfigMode, OnConfigModeChanged);

    AutoExecConfig(true, "entbosshp");
}

public void OnHitmarkerChannelChanged(ConVar cvar, char[] oldValue, char[] newValue)
{
    g_iHitMarkerChannel = g_Cvar_HitmarkerChannel.IntValue;
}

public void OnRewardChanged(ConVar cvar, char[] oldValue, char[] newValue)
{
    g_iReward = g_Cvar_Reward.IntValue;
}

public void OnConfigModeChanged(ConVar cvar, char[] oldValue, char[] newValue)
{
    g_iConfigMode = g_Cvar_ConfigMode.IntValue;
}

