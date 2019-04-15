#pragma once
#include <stdio.h>
#include <string>
#include "rapidxml/rapidxml.hpp"

class G18Snippet
{
protected:
	string _name;
	string _description;
	string _shortcut;
	string _code;
public:
	G18Snippet();
	~G18Snippet();
	bool Parse(rapidxml::xml_node<>* snippet);
};

