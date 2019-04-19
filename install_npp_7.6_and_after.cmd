@echo off
if exist C:\Program Files (x86)\Notepad++\notepad++.exe (
	md "C:\Program Files (x86)\Notepad++\plugins\G18NppSnippet\Config\G18Snippet\"

	copy plugins\G18NppSnippet_Releasex86.dll plugins\G18NppSnippet.dll
	xcopy plugins\G18NppSnippet.dll  "C:\Program Files (x86)\Notepad++\plugins\G18NppSnippet" /R /Y

	xcopy plugins\Config\G18Snippet\global.xml "C:\Program Files (x86)\Notepad++\plugins\G18NppSnippet\Config\G18Snippet\global.xml" /R /Y
)

if exist C:\Program Files\Notepad++\notepad++.exe (
	md "C:\Program Files\Notepad++\plugins\G18NppSnippet\Config\G18Snippet\"

	copy plugins\G18NppSnippet_Releasex64.dll plugins\G18NppSnippet.dll
	xcopy plugins\G18NppSnippet.dll  "C:\Program Files\Notepad++\plugins\G18NppSnippet" /R /Y

	xcopy plugins\Config\G18Snippet\global.xml "C:\Program Files\Notepad++\plugins\G18NppSnippet\Config\G18Snippet\global.xml" /R /Y
)
pause





