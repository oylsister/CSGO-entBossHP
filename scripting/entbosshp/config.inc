/**
 * ====================
 *     EntBossHP
 *   File: config.inc
 *   Author: Oylsister
 * ==================== 
 */

bool g_bLoaded = false;

enum BossType
{
    Type_Invalid = -1,
    Type_Breakable = 0,
    Type_MathCounter = 1,
    Type_HPBar = 2,
}

enum struct BossData
{
    char BOSS_NAME[64];
    BossType BOSS_TYPE;
    char BOSS_ENTITY[64];

    // HPBar Method
    char BOSS_ITERATOR[64];
    char BOSS_BACKUP[64];
}

enum struct MathCounterData
{
    int MC_MODE;
    int MC_MAX;
    int MC_MIN;
    int MC_InitValue;
}

enum struct HPBarData
{
    int HPBar_Multi;
    int HPBar_Min;
    int HPBar_Max;
    int HPBar_InitValue;
    int HPBar_Mode;
    int HPBar_Backup;
    int HPBar_Backup_Max;
    int HPBar_Backup_Min;
    int HPBar_Backup_InitValue;
}

BossData g_BossList[48];
MathCounterData g_MathCounterList[48];
HPBarData g_HPBarList[48];

int g_iTotal;

void LoadBossConfig()
{
    char sConfig[PLATFORM_MAX_PATH];
    char sMapName[PLATFORM_MAX_PATH];

    GetCurrentMap(sMapName, sizeof(sMapName));

    char sTemp[32];
    int iTemp;

    BuildPath(Path_SM, sConfig, sizeof(sConfig), "configs/entbosshp/%s.cfg", sMapName);
    LogMessage("Loading Boss Config: %s", sConfig);

    if (!FileExists(sConfig))
    {
        LogMessage("Couldn't find Boss Config: %s", sConfig);
        return;
    }

    g_iTotal = 0;
    g_bLoaded = true;

    KeyValues kv = CreateKeyValues("bosses");
    FileToKeyValues(kv, sConfig);

    if(KvGotoFirstSubKey(kv))
    {
        do
        {
            KvGetString(kv, "name", sTemp, sizeof(sTemp));
            FormatEx(g_BossList[g_iTotal].BOSS_NAME, 64, "%s", sTemp);

            KvGetString(kv, "type", sTemp, sizeof(sTemp));
            g_BossList[g_iTotal].BOSS_TYPE = GetConfigBossType(sTemp);

            KvGetString(kv, "entity", sTemp, sizeof(sTemp));
            FormatEx(g_BossList[g_iTotal].BOSS_ENTITY, 64, "%s", sTemp);

            if(g_BossList[g_iTotal].BOSS_TYPE == Type_MathCounter || g_BossList[g_iTotal].BOSS_TYPE == Type_HPBar)
            {
                iTemp = KvGetNum(kv, "countermode", -1);
                g_MathCounterList[g_iTotal].MC_MODE = iTemp;
            }

            if(g_BossList[g_iTotal].BOSS_TYPE == Type_HPBar)
            {
                KvGetString(kv, "iterator", sTemp, sizeof(sTemp));
                FormatEx(g_BossList[g_iTotal].BOSS_ITERATOR, 64, "%s", sTemp);

                iTemp = KvGetNum(kv, "iteratormode", -1);
                g_HPBarList[g_iTotal].HPBar_Mode = iTemp; 

                KvGetString(kv, "backup", sTemp, sizeof(sTemp));
                FormatEx(g_BossList[g_iTotal].BOSS_BACKUP, 64, "%s", sTemp);
            }

            g_iTotal++;
        }
        while(KvGotoNextKey(kv));
    }
    delete kv;
}

void LoadGFLBossConfig()
{
    char sConfig[PLATFORM_MAX_PATH];
    char sMapName[PLATFORM_MAX_PATH];

    GetCurrentMap(sMapName, sizeof(sMapName));

    char sTemp[32];
    int iTemp;

    BuildPath(Path_SM, sConfig, sizeof(sConfig), "configs/bosshud/%s.txt", sMapName);
    LogMessage("Loading Boss Config: %s", sConfig);

    if (!FileExists(sConfig))
    {
        LogMessage("Couldn't find Boss Config: %s", sConfig);
        return;
    }

    g_iTotal = 0;
    g_bLoaded = true;

    KeyValues kv = CreateKeyValues("math_counter");
    FileToKeyValues(kv, sConfig);

    if(KvGotoFirstSubKey(kv))
    {
        do
        {
            KvGetString(kv, "Type", sTemp, sizeof(sTemp));
            g_BossList[g_iTotal].BOSS_TYPE = GetConfigBossType(sTemp);

            if(g_BossList[g_iTotal].BOSS_TYPE == Type_Breakable)
            {
                KvGetString(kv, "BreakableName", sTemp, sizeof(sTemp));
                FormatEx(g_BossList[g_iTotal].BOSS_ENTITY, 64, "%s", sTemp);

                KvGetString(kv, "CustomText", sTemp, sizeof(sTemp));
                FormatEx(g_BossList[g_iTotal].BOSS_NAME, 64, "%s", sTemp);
            }

            else 
            {
                KvGetString(kv, "HP_Counter", sTemp, sizeof(sTemp));
                FormatEx(g_BossList[g_iTotal].BOSS_ENTITY, 64, "%s", sTemp);

                iTemp = KvGetNum(kv, "HP_Mode", -1);
                g_MathCounterList[g_iTotal].MC_MODE = iTemp;

                KvGetString(kv, "HPinit_Counter", sTemp, sizeof(sTemp));
                FormatEx(g_BossList[g_iTotal].BOSS_BACKUP, 64, "%s", sTemp);

                KvGetString(kv, "HPbar_Counter", sTemp, sizeof(sTemp));
                FormatEx(g_BossList[g_iTotal].BOSS_ITERATOR, 64, "%s", sTemp);

                iTemp = KvGetNum(kv, "HPbar_mode", -1);
                g_HPBarList[g_iTotal].HPBar_Mode = iTemp; 

                if(strlen(g_BossList[g_iTotal].BOSS_BACKUP) <= 0 && strlen(g_BossList[g_iTotal].BOSS_ITERATOR) <= 0)
                {
                    g_BossList[g_iTotal].BOSS_TYPE = Type_MathCounter;
                }
                else
                {
                    g_BossList[g_iTotal].BOSS_TYPE = Type_HPBar;
                }

                KvGetString(kv, "CustomText", sTemp, sizeof(sTemp));
                FormatEx(g_BossList[g_iTotal].BOSS_NAME, 64, "%s", sTemp);
            }

            g_iTotal++;
        }
        while(KvGotoNextKey(kv));
    }
    delete kv;
}

BossType GetConfigBossType(const char[] typename)
{
    if(StrEqual(typename, "breakable", false))
    {
        return Type_Breakable;
    }

    else if(StrEqual(typename, "math_counter", false))
    {
        return Type_MathCounter;
    }

    else if(StrEqual(typename, "hpbar", false))
    {
        return Type_HPBar;
    }

    return Type_Invalid;
}
