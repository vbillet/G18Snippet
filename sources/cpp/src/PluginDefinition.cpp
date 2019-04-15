#include "PluginDefinition.h"
#include "NppManager.h"
#include "G18Plugin.h"

/***************************************************
 * STEP 3 : Initialize your Plugin class
 ****************************************************/
void pluginInit(HANDLE hModule) {
	NppManager::Get()->SetModuleHandle((HMODULE)hModule); 
	G18Plugin::Get()->Init();
}
// Here you can do the clean up, save the parameters (if any) for the next session
void pluginCleanUp() { }

/***************************************************
 * STEP 4 : Define your commands
 ****************************************************/
 void commandMenuInit()
{
	NppManager* npp = NppManager::Get();

	ShortcutKey* tab = npp->CreateShortCut(false, false, false, VK_TAB);
	npp->SetCommand(0, TEXT("Snipp it!!"), SnipIt, tab, false);

	npp->SetCommand(1, TEXT("Reload Snippets"), ReloadLang, NULL, false);
	npp->SetCommand(2, TEXT("About"), AuboutBox, NULL, false);
 }
// Here you can do the clean up (especially for the shortcut)
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}

/***************************************************
 * STEP 5 : Associated Functions
 ****************************************************/
void SnipIt()			{ G18Plugin::Get()->SnipIt(); }
void ReloadLang()		{ G18Plugin::Get()->ReloadLang(); }
void AuboutBox()		{ G18Plugin::Get()->ShowAbout(); }
//::MessageBox(NULL, TEXT("G18 Snippet"), TEXT("Notepad++ G18 Snippet Plugin."), MB_OK); 
