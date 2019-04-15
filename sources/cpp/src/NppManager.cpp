#include "NppManager.h"
#include "PluginInterface.h"
#include <string>

NppManager nppManager;
NppData nppData;

NppManager* NppManager::Get()
{
	return nppManager._instance;
}

NppManager::NppManager()
{
	_instance = this;
}

NppManager::~NppManager()
{
}

void NppManager::SetModuleHandle(HMODULE hModule)
{
	_hModule = hModule;
	::GetModuleFileName(_hModule, _pluginPath, MAX_PATH);
	TCHAR* backslash = _tcsrchr(_pluginPath, _T('\\'));
	if (backslash != NULL)
		*backslash = _T('\0');
}

void NppManager::SetNppData(NppData data)
{
	_nppData = data;
	int which = -1;
	::SendMessage(_nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1) return;
	_curScintilla = (which == 0) ? _nppData._scintillaMainHandle : _nppData._scintillaSecondHandle;
}

NppData * NppManager::GetNppData()
{
	return &_nppData;
}

ShortcutKey * NppManager::CreateShortCut(bool alt, bool ctrl, bool shift, UCHAR key)
{
	if (key == 0)
		return nullptr;

	ShortcutKey* result = new ShortcutKey();
	result->_isAlt = alt;
	result->_isCtrl = ctrl;
	result->_isShift = shift;
	result->_key = key;
	
	return result;
}

bool NppManager::SetCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit)
{
	if (index >= NB_FUNC)
		return false;

	if (!pFunc)
		return false;

	lstrcpy(_funcItem[index]._itemName, cmdName);
	_funcItem[index]._pFunc = pFunc;
	_funcItem[index]._init2Check = check0nInit;
	_funcItem[index]._pShKey = sk;

	return true;
}

LangType NppManager::GetLangType()
{
	LangType langtype;
	::SendMessage(_nppData._nppHandle, NPPM_GETCURRENTLANGTYPE, 0, (LPARAM)&langtype);
	return langtype;
}

void NppManager::SelectRange(int startPos, int endPos)
{
	::SendMessage(_curScintilla, SCI_SETSELECTIONSTART, startPos, 0);
	::SendMessage(_curScintilla, SCI_SETSELECTIONEND, endPos, 0);

}

void NppManager::GetSelectedText(string &text)
{
	int len = static_cast<int>(::SendMessage(_curScintilla, SCI_GETSELTEXT, 0, 0));
	char* buf = new char[len];
	::SendMessage(_curScintilla, SCI_GETSELTEXT, 0, (LPARAM)buf);
	text.assign(buf, buf + len - 1);    // Don't copy the last '\0'
	delete[] buf;
}

bool NppManager::FindInTarget(string* text, int textLen, int start, int end, int flag)
{
	::SendMessage(_curScintilla, SCI_SETSEARCHFLAGS, flag, 0);
	::SendMessage(_curScintilla, SCI_SETTARGETSTART, start, 0);
	::SendMessage(_curScintilla, SCI_SETTARGETEND, end, 0);

	int f = static_cast<int>(::SendMessage(_curScintilla, SCI_SEARCHINTARGET, textLen, (LPARAM)text->c_str()));

	if (f != -1) {
		start = static_cast<int>(::SendMessage(_curScintilla, SCI_GETTARGETSTART, 0, 0));
		end = static_cast<int>(::SendMessage(_curScintilla, SCI_GETTARGETEND, 0, 0));
		SelectRange(start, end);
		return true;
	}
	return false;

}

void NppManager::GetIndent(int pos, string &indent)
{
	// Ignore indent over 60 chars.
	const int kIndentMax = 60;

	int lineNumber = static_cast<int>(::SendMessage(_curScintilla, SCI_LINEFROMPOSITION, pos, 0));
	int lineStart = static_cast<int>(::SendMessage(_curScintilla, SCI_POSITIONFROMLINE, lineNumber, 0));
	int eolMode = static_cast<int>(::SendMessage(_curScintilla, SCI_GETEOLMODE, 0, 0));

	char buf[kIndentMax + 1];

	switch (eolMode) {
	case SC_EOL_CRLF:
		indent = "\r\n";
		break;
	case SC_EOL_CR:
		indent = "\r";
		break;
	case SC_EOL_LF:
		indent = "\n";
		break;
	}

	Sci_TextRange range;
	range.chrg.cpMin = lineStart;
	range.chrg.cpMax = (pos > lineStart + kIndentMax) ? lineStart + kIndentMax : pos;
	range.lpstrText = buf;

	int len = static_cast<int>(::SendMessage(_curScintilla, SCI_GETTEXTRANGE, 0, (LPARAM)&range));

	indent.append(buf, len);

	size_t i = indent.find_first_not_of(" \t\r\n");
	if (i != string::npos) {
		indent.erase(i);
	}
}


