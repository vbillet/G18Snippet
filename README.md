# G18Snippet

Version: 0.1<br/>
(c) Vincent BILLET - 2019

<h1>Introduction:</h1>
G18 Snippet is a plugin for Notepad++ (Npp) allows you to insert code snippets into Notepad++.<br/>
Into Notepad++, type a shortcut followed by [TAB], G18Snippet will show you a window of all code snippets corresponding to this shortcut.<br/>
In addition, it will prompt for additional variables needed to build the snippet.<br/>

<h2>Content:</h2>
G18 Snippet is shipped with binary (64 bits), and source files.<br/>
<h3>plugins:</h3>
	plugins directory content should be copied into Notepad++ plugins directory (C:\Program Files (x86)\Notepad++\plugins).<br/>
	global.xml contains code snippets declarations. Use this file to configure <br/>
<h3>sources:</h3>
	contains c++ source code and visual studio project.<br/>

<h2>Installation</h2>
* Be sure Notepad++ is closed
* Copy G18Snippet.dll and config folder into C:\Program Files (x86)\Notepad++\plugins.
* Launch Notepad++

In Plugins Menu you should have G18 Snippet submenu. 

<h1>Languages:</h1>
<pre>
+-------------------+--------------------------------------------------------------------------------------+
| Language          | Shortcuts                                                                            |
+-------------------+--------------------------------------------------------------------------------------+
| pUML              | pumluc pumlseq pumlcl pumlgantt                                                      |
|                   |                                                                                      |
+-------------------+--------------------------------------------------------------------------------------+
</pre>

<h1>Writing Snippets:</h1>
To add snippets simply edit global.xml and add new snippet nodes. 
