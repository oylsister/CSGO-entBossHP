/**
 * ====================
 *     EntBossHP
 *   File: entbosshp.inc
 *   Author: Oylsister
 * ==================== 
 */

#if defined _entbosshp_included_
  #endinput
#endif
#define _entbosshp_included_

/**
 * Get the Boss name from config files.
 *
 * @param index   Boss Data index.
 * @param buffer  Buffer to store the string.
 * @param maxlen  Maximum length of Buffer
 *
 * @return   The boss name will be store in buffer if it's found in config file.
 */   
native void entBossHP_GetBossName(int index, char[] buffer, int maxlen);

/**
 * Get the Boss index.
 *
 * @param entityname   The entity's string name.
 *
 * @return   If found it, then return boss index, else -1 = failed.
 */   
native int entBossHP_GetBossIndex(const char[] entityname);

/**
 * Check if Boss entity is in config or not.
 *
 * @param entityname   The entity's string name.
 *
 * @return   If found it, true, else false.
 */   
native bool entBossHP_IsBossValid(const char[] entityname);