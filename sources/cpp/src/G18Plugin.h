#pragma once
#ifndef G18_PLUGIN_H
#define G18_PLUGIN_H

#include <stdio.h>
#include <string>
#include <vector>
#include "NppManager.h"
#include "G18Snippet.h"
#include "rapidxml/rapidxml.hpp"

class G18Plugin
{
public:
	G18Plugin();
	~G18Plugin();

	G18Plugin* _instance;

	static G18Plugin* Get();
	G18Plugin& operator= (const G18Plugin&) {}
	G18Plugin(const G18Plugin&) {}
private:
	typedef vector<G18Snippet*> SnippetList;

	SnippetList _langTable[NppManager::GLOBAL];
	/*vector<G18Snippet*>* _cmds;*/
	string _lastError;

	int _selStart;
	int _selEnd;
	int _curPos;
	int _startPos;
	int _endPos;
	string _selectedText;

	void loadLang(int lang);
	bool parse(char* src, int lang);
	SnippetList* getCmdList(string pShortcut, int pLang);
	G18Snippet* getSnippet(string cmdName);
	void ShowSnip();
	void validateSnip(HWND hWnd);

	void ReindentText(string& text, const string& indent);
	void FindReplace(string& text, const string& find, const string& replacement);
	void processVars(HWND hWnd,G18Snippet* pSnippet,string& text);
	void processEnd(string& text);
	void showSnippetVars(HWND hWnd,G18Snippet* pSnippet);
public : 
	void Init();

	void SnipIt();
	void ReloadLang();
	void ShowAbout();
	void onCommandSelChange(HWND hWnd);

	void onAboutCommand(HWND hWnd, int id, int id2);
	void onAboutInitDialog(HWND hWnd, int id, int id2);
	void onSnipCommand(HWND hWnd, int id, int id2);
	void onSnipInitDialog(HWND hWnd, int id, int id2);

};

INT_PTR CALLBACK ProcAbout(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ProcSnip(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


#endif
