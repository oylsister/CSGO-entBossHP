/**
 * ==============================
 *     GFL BossHP File Transform
 *   File: gfl_bosshp_transform.sp
 *   Author: Oylsister
 * ==============================
 */

#include <sourcemod>

#define MATH_COUNTER 0
#define HPBAR 1
#define BREAKABLE 2

enum struct Old_Config
{
    //Math_Counter and HPBar method
    char sCounter[64];
    int iCounterMode;

    char sIterator[64];
    char iIteratorMode;

    char sBackup[64];

    // Breakable
    char sType[64];
    char sBreakable[64];

    char sBossName[64];

    int iType;
}

Old_Config g_oldConfig[64];

int itotal;

public Plugin myinfo =
{
    name = "GFL BossHP Config Transform to EntBossHP", 
    author = "Oylsister", 
    description = "Showing Entity Health from the map", 
    version = "1.0", 
    url = "https://github.com/oylsister/"
};

public void OnPluginStart()
{
    RegConsoleCmd("sm_readfolder", ReadFolder);
}

public Action ReadFolder(int client, int args)
{
    SomeFunction();
    return Plugin_Handled;
}

void SomeFunction()
{
    char fileBuffer[PLATFORM_MAX_PATH];
    char Path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, Path, sizeof(Path), "configs/MapBossHP");
    if (!DirExists(Path))
    {
        LogError("Could not find %s folder", Path);
        return;
    }
    
    DirectoryListing dL = OpenDirectory(Path);
    while (dL.GetNext(fileBuffer, sizeof(fileBuffer))) 
    {
        PrintToServer("Found: %s", fileBuffer);
        TransformFile(fileBuffer);
    } 
}

void TransformFile(const char[] configfile)
{
    char Path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, Path, sizeof(Path), "configs/MapBossHP/%s", configfile);

    char sTemp[64];

    if (!FileExists(Path))
    {
        LogMessage("Boss Config is Invalid: %s", Path);
        return;
    }

    itotal = 0;

    KeyValues oldkv = CreateKeyValues("math_counter");
    FileToKeyValues(oldkv, Path);

    if(KvGotoFirstSubKey(oldkv))
    {
        do
        {
            KvGetSectionName(oldkv, sTemp, sizeof(sTemp));
            
            if(!StrEqual(sTemp, "config", false))
            {
                KvGetString(oldkv, "HP_counter", sTemp, sizeof(sTemp));
                FormatEx(g_oldConfig[itotal].sCounter, 64, "%s", sTemp);

                g_oldConfig[itotal].iCounterMode = KvGetNum(oldkv, "HP_Mode", -1);
                
                KvGetString(oldkv, "HPbar_counter", sTemp, sizeof(sTemp));
                FormatEx(g_oldConfig[itotal].sIterator, 64, "%s", sTemp);

                KvGetString(oldkv, "HPinit_counter", sTemp, sizeof(sTemp));
                FormatEx(g_oldConfig[itotal].sBackup, 64, "%s", sTemp);

                g_oldConfig[itotal].iIteratorMode = KvGetNum(oldkv, "HPbar_mode", -1);

                KvGetString(oldkv, "Type", sTemp, sizeof(sTemp));
                FormatEx(g_oldConfig[itotal].sType, 64, "%s", sTemp);

                KvGetString(oldkv, "BreakableName", sTemp, sizeof(sTemp));
                FormatEx(g_oldConfig[itotal].sBreakable, 64, "%s", sTemp);

                KvGetString(oldkv, "CustomText", sTemp, sizeof(sTemp));
                FormatEx(g_oldConfig[itotal].sBossName, 64, "%s", sTemp);

                // If find Iterator entity then it's hpbar
                if(strlen(g_oldConfig[itotal].sIterator) > 0)
                {
                    g_oldConfig[itotal].iType = HPBAR;
                }

                // if only find math_counter then it's counter
                else if(strlen(g_oldConfig[itotal].sIterator) <= 0 && strlen(g_oldConfig[itotal].sCounter) > 0)
                {
                    g_oldConfig[itotal].iType = MATH_COUNTER;
                }

                // if can't find both then it's breakable
                else
                {
                    g_oldConfig[itotal].iType = BREAKABLE;
                }

                itotal++;
            }
        }
        while(KvGotoNextKey(oldkv));
    }
    delete oldkv;

    CreateNewConfig(configfile);
}

void CreateNewConfig(const char[] configfile)
{
    char newfile[PLATFORM_MAX_PATH];
    strcopy(newfile, sizeof(newfile), configfile);

    ReplaceString(newfile, sizeof(newfile), ".txt", "", false);

    char Path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, Path, sizeof(Path), "configs/entbosshp/%s.cfg", newfile);

    char sTemp[64];

    if (FileExists(Path))
    {
        LogMessage("Boss Config %s is already existed", Path);
        return;
    }

    File bossfile = OpenFile(Path, "a+");
    WriteFileLine(bossfile, "\"bosses\"");
    WriteFileLine(bossfile, "{");
    WriteFileLine(bossfile, "}");

    delete bossfile;

    int index = 0;

    KeyValues kv = CreateKeyValues("bosses");
    
    for(int i = 0; i < itotal; i++)
    {
        IntToString(index, sTemp, sizeof(sTemp));
        KvJumpToKey(kv, sTemp, true)

        if(g_oldConfig[i].sBossName[0] == '\0')
            KvSetString(kv, "name", "Boss");

        else
            KvSetString(kv, "name", g_oldConfig[i].sBossName);

        if(g_oldConfig[i].iType == BREAKABLE)
            KvSetString(kv, "type", "breakable");

        else if(g_oldConfig[i].iType == MATH_COUNTER)
            KvSetString(kv, "type", "math_counter");

        else if(g_oldConfig[i].iType == HPBAR)
            KvSetString(kv, "type", "hpbar");

        if(g_oldConfig[i].iType == MATH_COUNTER || g_oldConfig[i].iType == HPBAR)
            KvSetString(kv, "entity", g_oldConfig[i].sCounter);

        else
            KvSetString(kv, "entity", g_oldConfig[i].sBreakable);

        if(g_oldConfig[i].iType == MATH_COUNTER || g_oldConfig[i].iType == HPBAR)
        {
            if(g_oldConfig[i].iCounterMode > 0)
                KvSetNum(kv, "countermode", g_oldConfig[i].iCounterMode);

            if(g_oldConfig[i].iType == HPBAR)
            {
                if(strlen(g_oldConfig[i].sIterator) > 0)
                    KvSetString(kv, "iterator", g_oldConfig[i].sIterator);
                
                if(g_oldConfig[i].iIteratorMode > 0)
                    KvSetNum(kv, "iteratormode", g_oldConfig[i].iIteratorMode);

                if(strlen(g_oldConfig[i].sBackup) > 0)
                    KvSetString(kv, "backup", g_oldConfig[i].sBackup);
            }
        }
        KvRewind(kv);
        index++;
    }
    
    PrintToServer("File: %s has been created. The map contain: %d bosses", newfile, index + 1);
    KeyValuesToFile(kv, Path);
    delete kv;
}


