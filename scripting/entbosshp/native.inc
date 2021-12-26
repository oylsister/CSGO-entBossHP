/**
 * ====================
 *     EntBossHP
 *   File: native.inc
 *   Author: Oylsister
 * ==================== 
 */

void NativeInit()
{
    CreateNative("entBossHP_GetBossName", Native_GetBossName);
    CreateNative("entBossHP_GetBossIndex", Native_GetBossIndex);

    CreateNative("entBossHP_IsBossValid", Native_IsBossValid);
}

public int Native_GetBossName(Handle plugin, int params)
{
    int index = GetNativeCell(1);
    int maxlen = GetNativeCell(3);

    if(maxlen <= 0)
    {
        return;
    }

    char bossname[64];
    GetBossName(index, bossname, maxlen);

    SetNativeString(2, bossname, maxlen);
}

public int Native_GetBossIndex(Handle plugin, int params)
{
    int len;
    GetNativeStringLength(1, len);

    if(len <= 0)
    {
        return -1;
    }

    char[] entityname = new char[len+1];
    GetNativeString(1, entityname, len+1);

    return GetBossEntityIndex(entityname);
}

public int Native_IsBossValid(Handle plugin, int params)
{
    int len;
    GetNativeStringLength(1, len);

    if(len <= 0)
    {
        return false;
    }

    char[] entityname = new char[len+1];
    GetNativeString(1, entityname, len+1);

    int index = GetBossEntityIndex(entityname);
    if(index != -1)
    {
        return true;
    }
    return false;
}