#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include "rapidxml/rapidxml.hpp"

using namespace std;

class G18SnippetVar {
public:
	string name;
	bool optional;
	string label;
	G18SnippetVar() { name = ""; optional = false; label = ""; };
	~G18SnippetVar() {};
};

class G18Snippet
{
public:
	G18Snippet();
	~G18Snippet();

private:
	string _hello;
	string _name;
	string _description;
	string _shortcut;
	string _code;
	vector<G18SnippetVar*>* _snippetVars;

public:
	bool Parse(rapidxml::xml_node<>* snippet);
	string GetShortcut() { return _shortcut; }
	string GetName() { return _name; }
	string GetCode() { return _code; }
	string GetDescription() { return _description; }
	G18SnippetVar* GetVarByName(string pName);
	G18SnippetVar* GetVarByIndex(int idx) 
	{
		if (idx > int(_snippetVars->size())) return nullptr;
		return _snippetVars->at(idx); 
	}
	int GetVarCount() { return int(_snippetVars->size()); }
};

