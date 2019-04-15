#pragma once
#ifndef NPP_MANAGER_H
#define NPP_MANAGER_H

#include <stdio.h>
#include <string>

#include "menuCmdID.h"
#include "PluginInterface.h"
#include "PluginDefinition.h"

using namespace std;

class NppManager
{
public:
	static NppManager* Get();
	NppManager* _instance;
	NppManager();
	NppManager& operator= (const NppManager&) {}
	NppManager(const NppManager&) {}
	~NppManager();

private:
	HMODULE			_hModule;
	FuncItem		_funcItem[NB_FUNC];
	NppData			_nppData;
	HWND			_curScintilla;
	TCHAR			_pluginPath[MAX_PATH];

public:
	enum {
		GLOBAL = L_EXTERNAL + 1,
		TABLE_COUNT
	};
	void SetModuleHandle(HMODULE hModule);
	void SetNppData(NppData data);
	NppData* GetNppData();
	HMODULE GethModule() { return _hModule; }
	FuncItem *GetPluginFunctions() { return _funcItem; }

	ShortcutKey* CreateShortCut(bool alt, bool ctrl, bool shift, UCHAR key);
	bool SetCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit);

	void FileNewDocument() { ::SendMessage(_nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW); }
	
	void cmd(UINT msg) { ::SendMessage(_curScintilla, msg, 0, 0); }

	void SetText(string* text) { ::SendMessage(_curScintilla, SCI_SETTEXT, 0, (LPARAM)text->c_str()); }
	void InsertText(string* text) { ::SendMessage(_curScintilla, SC_MOD_INSERTTEXT, 0, (LPARAM)text->c_str()); }
	LangType GetLangType();

	void SelectRange(int startPos, int endPos);
	void GetSelectedText(string& text);
	
	bool FindInTarget(string* text, int textLen, int start, int end, int flag);
	void GetIndent(int pos, string& indent);
	
	void EraseSelected() { ::SendMessage(_curScintilla, SCI_REPLACESEL, 0, (LPARAM) ""); }
	
	void ReplaceSelected(string* text) { ::SendMessage(_curScintilla, SCI_REPLACESEL, 0, (LPARAM)text->c_str()); }

	int GetSelectionStart() { return static_cast<int>(::SendMessage(_curScintilla, SCI_GETSELECTIONSTART, 0, 0));}
	int GetSelectionEnd() { return static_cast<int>(::SendMessage(_curScintilla, SCI_GETSELECTIONEND, 0, 0)); }

	int GetCurrentPos() { return static_cast<int>(::SendMessage(_curScintilla, SCI_GETCURRENTPOS, 0, 0)); }

	void SetWordChars(string* chars) { ::SendMessage(_curScintilla, SCI_SETWORDCHARS, 0, (LPARAM)chars->c_str()); }
	int GetWordStartPos(int curPos) { return static_cast<int>(::SendMessage(_curScintilla, SCI_WORDSTARTPOSITION, curPos, (LPARAM)true)); }
	int GetWordEndPos(int curPos) { return static_cast<int>(::SendMessage(_curScintilla, SCI_WORDENDPOSITION, curPos, (LPARAM)true)); }
	TCHAR* GetPluginPath() { return _pluginPath; }
};

const TCHAR* const LANG_NAMES[NppManager::TABLE_COUNT] = {
	_T("txt"),      _T("php"),      _T("c"),        _T("cpp"),      _T("cs"),
	_T("objc"),     _T("java"),     _T("rc"),       _T("html"),     _T("xml"),
	_T("makefile"), _T("pascal"),   _T("batch"),    _T("ini"),      _T("nfo"),
	_T("user"),     _T("asp"),      _T("sql"),      _T("vb"),       _T("js"),
	_T("css"),      _T("perl"),     _T("python"),   _T("lua"),      _T("tex"),
	_T("fortran"),  _T("bash"),     _T("flash"),    _T("nsis"),     _T("tcl"),
	_T("lisp"),     _T("scheme"),   _T("asm"),      _T("diff"),     _T("props"),
	_T("ps"),       _T("ruby"),     _T("smalltalk"),_T("vhdl"),     _T("kix"),
	_T("au3"),      _T("caml"),     _T("ada"),      _T("verilog"),  _T("matlab"),
	_T("haskell"),  _T("inno"),     _T("search"),   _T("cmake"),    _T("yaml"),
	_T("cobol"),	_T("gui4cli"),	_T("d"),		_T("power"),	_T("r"),
	_T("jsp"),		_T("coffee"),	_T("json"),		_T("javascript"),_T("fortran"),	
	_T("external"),	_T("global")
};

#endif // !NPP_MANAGER_H
