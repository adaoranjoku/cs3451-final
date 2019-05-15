/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

#pragma once

#include "Rule.h"
#include "LinkedList.h"

class Rules
{
private:
	std::vector<Rule> _rules;
public:
	//Constructor
	Rules(std::vector<Rule> rules);

	LinkedList<Module> parse(Module m) const;
};