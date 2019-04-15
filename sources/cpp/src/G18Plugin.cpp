#include "G18Plugin.h"
#include "resource.h"

namespace rapidxml
{
	template<class Ch> class xml_node;
}

G18Plugin plugin;

G18Plugin* G18Plugin::Get() { return plugin._instance; }
G18Plugin::G18Plugin() 
{ 
	_instance = this; 
	_selStart = 0;
	_selEnd = 0; 
	_startPos = 0;
	_curPos = 0;
	_endPos = 0;
}
G18Plugin::~G18Plugin() { }

void G18Plugin::Init() { 
	loadLang(NppManager::GLOBAL);
}
void G18Plugin::loadLang(int lang)
{
	TCHAR filePath[MAX_PATH];
	_stprintf(
		filePath,
		_T("%s\\Config\\G18Snippet\\%s.%s"),
		NppManager::Get()->GetPluginPath(), LANG_NAMES[lang], _T("xml"));

	HANDLE fin;
	fin = CreateFile(
		filePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (fin != INVALID_HANDLE_VALUE) {
		// Can't handle a snippet file up to 4GB(2^32) bytes, which is unlikely to happen.
		DWORD readSize;
		DWORD fileSize = GetFileSize(fin, NULL);
		char* content = new char[fileSize + 1];

		::ReadFile(fin, content, fileSize, &readSize, NULL);
		content[readSize] = '\0';
		parse(content, lang);
		delete[] content;
	}
	CloseHandle(fin);
}
bool G18Plugin::parse(char * src, int lang)
{
	using namespace rapidxml;
	G18Plugin::SnippetList *table = _langTable[lang];
	if (table->size() != 0) table->clear();
	try {
		xml_document<> doc;
		doc.parse<0>(src);

		xml_node<>* root = doc.first_node();

		if (strcmp("G18Snippet", root->name()) != 0) return false;

		for (xml_node<>* snippet = root->first_node("snippet");
			snippet; snippet = snippet->next_sibling("snippet"))
		{
			G18Snippet* newSnippet = new G18Snippet();
			if (newSnippet->Parse(snippet))
			{
				table->push_back(newSnippet);
			}
			else
			{
				delete(newSnippet);
			}
		}
	}
	catch (parse_error& er) {
		_lastError = er.what();
		// ErrorMessage(_T("XML format error!"));
		return false;
	}
	return true;
}
void G18Plugin::ReloadLang()
{
	loadLang(NppManager::GLOBAL);
}

void G18Plugin::ShowAbout()
{
	NppManager* npp = NppManager::Get();
	/*INT_PTR ret = */DialogBoxParam(
		(HINSTANCE)npp->GethModule(),
		MAKEINTRESOURCE(IDD_G18SNIPPET_DLG),
		npp->GetNppData()->_nppHandle,
		ProcAbout,
		(LPARAM)0);
}
void G18Plugin::onCommandSelChange(HWND hWnd)
{
	HWND hList = ::GetDlgItem(hWnd, IDC_LISTG18);
	int selIndex = SendMessage(hList, LB_GETCURSEL, 0, 0L);
	wchar_t Temp[250];
	SendMessage(hList, LB_GETTEXT, selIndex, (LPARAM)Temp);

	wstring ws(Temp);
	string cmdName(ws.begin(), ws.end());
	G18Snippet* snippet = getSnippet(cmdName);
	if (snippet != nullptr)
	{
		showSnippetVars(hWnd,snippet);
	}
}

void G18Plugin::onAboutCommand(HWND hWnd, int id, int)
{
	switch (id) {
	case IDOK:
	{
		::EndDialog(hWnd, id);
		break;
	}
	case IDCANCEL:
		::EndDialog(hWnd, id);
		break;
	}
}
void G18Plugin::onAboutInitDialog(HWND /*hWnd*/, int, int)
{
/*	HWND hTxt = ::GetDlgItem(hWnd, IDC_NBSNIPPET);
	string nbGlobalSnip = "Number of Snippets : ";*/
	//string centerMsg = nbGlobalSnip + to_string(_langTable[NppManager::GLOBAL]->size());
	//SetWindowTextA(hTxt, centerMsg.c_str());
}
INT_PTR CALLBACK ProcAbout(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM /*lParam*/)
{
	int id = LOWORD(wParam);
	int id2 = HIWORD(wParam);
	switch (uMsg) {
	case WM_COMMAND:
	{
		G18Plugin::Get()->onAboutCommand(hWnd, id, id2);
		break;
	}
	case WM_INITDIALOG:
	{
		G18Plugin::Get()->onAboutInitDialog(hWnd, id, id2);
		break;
	}
	default:
		return FALSE;
	}
	return TRUE;
}

G18Plugin::SnippetList* G18Plugin::getCmdList(string pShortcut, int pLang)
{
	G18Plugin::SnippetList* result =new G18Plugin::SnippetList;
	result->clear();
	for (G18Plugin::SnippetList::iterator itr = _langTable[pLang]->begin(); itr != _langTable[pLang]->end(); ++itr)
	{
		G18Snippet* s = *itr;
		string shortcut = s->GetShortcut(); 
		if (strcmp(pShortcut.c_str(), shortcut.c_str()) == 0)
		{
			result->push_back(s);
		}
	}
	return result;
}
G18Snippet * G18Plugin::getSnippet(string cmdName)
{
	G18Plugin::SnippetList* _cmds = getCmdList(_selectedText, NppManager::GLOBAL);
	if (_cmds->size() == 0) return nullptr;

	for (G18Plugin::SnippetList::iterator itr = _cmds->begin(); itr != _cmds->end(); ++itr)
	{
		G18Snippet* s = *itr;
		if (s->GetName() == cmdName)
			return s;
	}
	return nullptr;
}
void G18Plugin::SnipIt()
{
	NppManager* npp = NppManager::Get();

	string text = "<div>Inserted</div>";
	string validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890*µ&_=;$%";

	_selStart = npp->GetSelectionStart();
	_selEnd = npp->GetSelectionEnd();

	if (_selStart != _selEnd) { npp->cmd(SCI_TAB); return; }
	
	npp->SetWordChars(&validChars);

	_curPos = npp->GetCurrentPos();
	_startPos = npp->GetWordStartPos(_curPos);
	_endPos = npp->GetWordEndPos(_curPos);

	npp->SelectRange(_startPos, _endPos);
	npp->GetSelectedText(_selectedText);
	if (getCmdList(_selectedText, NppManager::GLOBAL)->size()>0)
		ShowSnip();
	else {
		npp->SelectRange(_curPos, _curPos);
		npp->cmd(SCI_TAB);
	}
}
void G18Plugin::ShowSnip()
{
	NppManager* npp = NppManager::Get(); 
	/*INT_PTR ret = */DialogBoxParam(
		(HINSTANCE)npp->GethModule(),
		MAKEINTRESOURCE(IDD_SNIP_DLG),
		npp->GetNppData()->_nppHandle,
		ProcSnip,
		(LPARAM)0);
}

void G18Plugin::validateSnip(HWND hWnd)
{
	NppManager* npp = NppManager::Get();
	HWND hList = ::GetDlgItem(hWnd, IDC_LISTG18);
	int selIndex = SendMessage(hList, LB_GETCURSEL, 0, 0L);
	wchar_t Temp[250];
	SendMessage(hList, LB_GETTEXT, selIndex, (LPARAM)Temp);

	wstring ws(Temp);
	string cmdName(ws.begin(), ws.end());
	G18Snippet* snippet = getSnippet(cmdName);
	if (snippet != nullptr)
	{
		string code = snippet->GetCode();
		string shortcut = snippet->GetShortcut();

		string indent;
		npp->GetIndent(_curPos, indent);
		ReindentText(code, indent);

		processVars(hWnd,snippet, code);
		processEnd(code);

		npp->ReplaceSelected(&code);
		_curPos -= shortcut.length();
		npp->SelectRange(_curPos, _curPos);
	}
	else
	{
		npp->SelectRange(_curPos, _curPos);
		npp->cmd(SCI_TAB);
	}
}
void G18Plugin::ReindentText(string & text, const string & indent)
{
	size_t lineStart = 0;
	size_t lineEnd = text.find_first_of("\n\r");

	// If there is only single line, nothing change
	if (lineEnd == string::npos)
		return;

	string result(text.begin(), text.begin() + lineEnd);
	result.append(indent);

	while (true) {
		// Simply skip all EOL chars
		if (text[lineEnd] == '\r' && text[lineEnd + 1] == '\n') {
			lineStart = lineEnd + 2;
		}
		else {
			lineStart = lineEnd + 1;
		}

		if (lineStart >= text.size())
			break;

		lineEnd = text.find_first_of("\n\r", lineStart);
		if (lineEnd != string::npos) {
			result.append(text, lineStart, lineEnd - lineStart);
			result.append(indent);
		}
		else {
			result.append(text, lineStart, text.size() - lineStart);
			break;
		}
	}
	text.swap(result);
}

void G18Plugin::FindReplace(string& text, const string& find, const string& replacement)
{
	size_t repLen = replacement.size();
	size_t findLen = find.size();
	size_t pos = text.find(find, 0);

	while (pos != string::npos) {
		text.replace(pos, findLen, replacement);
		if (pos + repLen >= text.size())
			break;
		pos = text.find(find, pos + repLen);
	}
}

void G18Plugin::processVars(HWND hWnd, G18Snippet* pSnippet, string & text)
{
	int cnt = pSnippet->GetVarCount();
	for (int i = 0; i < cnt; i++)
	{
		G18SnippetVar* var = pSnippet->GetVarByIndex(i);
		int sz = 0;
		wchar_t txt[255];
		if (i == 0) sz = GetWindowText(::GetDlgItem(hWnd, IDCVAL1), txt, 255);
		if (i == 1) sz = GetWindowText(::GetDlgItem(hWnd, IDCVAL2), txt, 255);
		if (i == 2) sz = GetWindowText(::GetDlgItem(hWnd, IDCVAL3), txt, 255);
		if (i == 3) sz = GetWindowText(::GetDlgItem(hWnd, IDCVAL4), txt, 255);
		if (i == 4) sz = GetWindowText(::GetDlgItem(hWnd, IDCVAL5), txt, 255);
		if (i == 5) sz = GetWindowText(::GetDlgItem(hWnd, IDCVAL6), txt, 255);
		wstring ws(txt);
		string val(ws.begin(), ws.end());
		FindReplace(text, "$"+var->name+"$", val);
	}
}

void G18Plugin::processEnd(string & text)
{
	size_t pos = text.find("$end$", 0);
	_curPos += pos;
	FindReplace(text, "$end$", "");
}

void G18Plugin::showSnippetVars(HWND hWnd, G18Snippet* pSnippet)
{
	// Retrieve Handle
	HWND hDesc = ::GetDlgItem(hWnd, IDCDESCRIPTION);
	HWND hVar1 = ::GetDlgItem(hWnd, IDCVAR1);
	HWND hVar2 = ::GetDlgItem(hWnd, IDCVAR2);
	HWND hVar3 = ::GetDlgItem(hWnd, IDCVAR3);
	HWND hVar4 = ::GetDlgItem(hWnd, IDCVAR4);
	HWND hVar5 = ::GetDlgItem(hWnd, IDCVAR5);
	HWND hVar6 = ::GetDlgItem(hWnd, IDCVAR6);
	HWND hVal1 = ::GetDlgItem(hWnd, IDCVAL1);
	HWND hVal2 = ::GetDlgItem(hWnd, IDCVAL2);
	HWND hVal3 = ::GetDlgItem(hWnd, IDCVAL3);
	HWND hVal4 = ::GetDlgItem(hWnd, IDCVAL4);
	HWND hVal5 = ::GetDlgItem(hWnd, IDCVAL5);
	HWND hVal6 = ::GetDlgItem(hWnd, IDCVAL6);
	// Show all Except desc
	ShowWindow(hDesc, SW_SHOW);
	ShowWindow(hVar1, SW_HIDE);
	ShowWindow(hVar2, SW_HIDE);
	ShowWindow(hVar3, SW_HIDE);
	ShowWindow(hVar4, SW_HIDE);
	ShowWindow(hVar5, SW_HIDE);
	ShowWindow(hVar6, SW_HIDE);
	ShowWindow(hVal1, SW_HIDE);
	ShowWindow(hVal2, SW_HIDE);
	ShowWindow(hVal3, SW_HIDE);
	ShowWindow(hVal4, SW_HIDE);
	ShowWindow(hVal5, SW_HIDE);
	ShowWindow(hVal6, SW_HIDE);
	// Show 
	SetWindowTextA(hDesc, pSnippet->GetDescription().c_str() );
	int cnt = pSnippet->GetVarCount();
	if ( cnt >= 1 ) { ShowWindow(hVar1, SW_SHOW); ShowWindow(hVal1, SW_SHOW); EnableWindow(hVal1,true); SetWindowTextA( hVar1, pSnippet->GetVarByIndex(0)->label.c_str() ); }
	if ( cnt >= 2 ) { ShowWindow(hVar2, SW_SHOW); ShowWindow(hVal2, SW_SHOW); EnableWindow(hVal2,true); SetWindowTextA( hVar2, pSnippet->GetVarByIndex(1)->label.c_str() ); }
	if ( cnt >= 3 ) { ShowWindow(hVar3, SW_SHOW); ShowWindow(hVal3, SW_SHOW); EnableWindow(hVal3,true); SetWindowTextA( hVar3, pSnippet->GetVarByIndex(2)->label.c_str() ); }
	if ( cnt >= 4 ) { ShowWindow(hVar4, SW_SHOW); ShowWindow(hVal4, SW_SHOW); EnableWindow(hVal4,true); SetWindowTextA( hVar4, pSnippet->GetVarByIndex(3)->label.c_str() ); }
	if ( cnt >= 5 ) { ShowWindow(hVar5, SW_SHOW); ShowWindow(hVal5, SW_SHOW); EnableWindow(hVal5,true); SetWindowTextA( hVar5, pSnippet->GetVarByIndex(4)->label.c_str() ); }
	if ( cnt == 6 ) { ShowWindow(hVar6, SW_SHOW); ShowWindow(hVal6, SW_SHOW); EnableWindow(hVal6,true); SetWindowTextA( hVar6, pSnippet->GetVarByIndex(5)->label.c_str() ); }

}

void G18Plugin::onSnipCommand(HWND hWnd, int id, int id2)
{
	switch (id) {
		case IDOK:
		{
			validateSnip(hWnd);
			::EndDialog(hWnd, id);
			break;
		}
		case IDCANCEL:
		{
			::EndDialog(hWnd, id);
			break;
		}
		case IDC_LISTG18:
		{
			if (id2==LBN_SELCHANGE)
				onCommandSelChange(hWnd);
			break;

		}
	}
}
void G18Plugin::onSnipInitDialog(HWND hWnd, int , int )
{
	G18Plugin::SnippetList* _cmds = getCmdList(_selectedText, NppManager::GLOBAL);
	if (_cmds->size() == 0) {
		onSnipCommand(hWnd, IDCANCEL, 0);
		return;
	}

	HWND hList = ::GetDlgItem(hWnd, IDC_LISTG18);
	SendMessage(hList, LB_RESETCONTENT, 0, 0);

	for (G18Plugin::SnippetList::iterator itr = _cmds->begin(); itr != _cmds->end(); ++itr)
	{
		G18Snippet* s = *itr;
		string sname = s->GetName();
		wstring wname = wstring(sname.begin(), sname.end());
		const wchar_t* buf = wname.c_str();
		SendMessage(hList, LB_ADDSTRING, 0, LPARAM(buf));
	}
	SendMessage(hList, LB_SETCURSEL, 0, 0);
	SetFocus(hList);
	onCommandSelChange(hWnd);
}

INT_PTR CALLBACK ProcSnip(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM /*lParam*/)
{
	int id = LOWORD(wParam);
	int id2 = HIWORD(wParam);
	switch (uMsg) {
	case WM_COMMAND:
	{
		G18Plugin::Get()->onSnipCommand(hWnd, id, id2);
		break;
	}
	case WM_INITDIALOG:
	{
		G18Plugin::Get()->onSnipInitDialog(hWnd, id, id2);
		break;
	}
	default:
		return FALSE;
	}
	return TRUE;
}
