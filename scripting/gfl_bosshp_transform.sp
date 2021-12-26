/**
 * ==============================
 *     GFL BossHP File Transform
 *   File: gfl_bosshp_transform.sp
 *   Author: Oylsister
 * ==============================
 */

#include <sourcemod>

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
    SomeFunction();
}

public Action ReadFolder(int client, int args)
{
    SomeFunction();
    return Plugin_Handled;
}

void SomeFunction()
{
    char fileBuffer[512];
    char Path[512];
    BuildPath(Path_SM, Path, sizeof(Path), "configs/bosshud");
    if (!DirExists(Path))
    {
        LogError("Could not find %s folder", Path);
        return;
    }
    
    DirectoryListing dL = OpenDirectory(Path);
    while (dL.GetNext(fileBuffer, sizeof(fileBuffer))) 
    {
        PrintToServer("%s", fileBuffer);
    } 
}

/*
void TrasnformFile(const char[] map)
{

}
*/

