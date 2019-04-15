#include "G18Snippet.h"
#include "G18Plugin.h"
#include "NppManager.h"
#include "rapidxml/rapidxml.hpp"

G18Snippet::G18Snippet()
{
	_name = "";
	_description = "No Description";
	_shortcut = "";
	_code = "";
	_snippetVars = new vector<G18SnippetVar*>();
	_snippetVars->clear();
}

G18Snippet::~G18Snippet()
{
}

bool G18Snippet::Parse(rapidxml::xml_node<>* snippet)
{
	using namespace rapidxml;
	xml_attribute<> * name = snippet->first_attribute("name");
	if (!name) { return false; }
	_name = name->value();

	xml_attribute<> * shortcut = snippet->first_attribute("shortcut");
	if (!shortcut) { return false; }
	_shortcut = string(shortcut->value());


	for (xml_node<>* var = snippet->first_node("var");
		var;
		var = var->next_sibling("var"))
	{
		xml_attribute<> * varname = var->first_attribute("name");
		xml_attribute<> * varoptional = var->first_attribute("optional");
		xml_attribute<> * varlabel = var->first_attribute("label");

		if (varname && varoptional && varlabel)
		{
			G18SnippetVar* newVar = new G18SnippetVar();
			newVar->name = string(varname->value());
			newVar->optional = string(varoptional->value()) == "1" ? true : false;
			newVar->label = string(varlabel->value());

			_snippetVars->push_back(newVar);
		}
	}


	xml_node<>* code = snippet->first_node("code"); 
	if (code && code->first_node() && code->first_node()->type() == node_cdata) {
		_code = code->first_node()->value();
	} else {
		return false;
	}
	xml_node<>* description = snippet->first_node("description");
	if(description)
		_description = description->value();


	return true;
}

G18SnippetVar * G18Snippet::GetVarByName(string pName)
{
	for (vector<G18SnippetVar*>::iterator itr = _snippetVars->begin(); itr != _snippetVars->end(); ++itr)
	{
		G18SnippetVar * i = *itr;
		if (i->name == pName)
			return *itr;
	}
	return nullptr;
}
