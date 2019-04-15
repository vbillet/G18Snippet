#ifndef PLUGINDEFINITION_H
#define PLUGINDEFINITION_H
#include "PluginInterface.h"

/***************************************************
 * All difinitions of plugin interface
 ****************************************************/
void pluginInit(HANDLE hModule);		// Initialization of your plugin data
void pluginCleanUp();					// Cleaning of your plugin
void commandMenuInit();					// Initialization of your plugin commands
void commandMenuCleanUp();				// Clean up your plugin commands allocation (if any)

/***************************************************
 * STEP 1 : Define your plugin name
 ****************************************************/
const TCHAR NPP_PLUGIN_NAME[] = TEXT("G18 Snippet");

/***************************************************
 * STEP 2 : Define your commands
 ****************************************************/
static const int NB_FUNC = 3;
void SnipIt();
void ReloadLang();
void AuboutBox();

#endif //PLUGINDEFINITION_H
