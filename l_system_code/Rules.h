/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

#pragma once

#include "Rule.h"
#include "LinkedList.h"

// Class to represent a full set of Rules for a module
class Rules
{
private:
	std::vector<Rule> _rules;
public:
	//Constructors
	Rules(std::vector<Rule> rules);
	Rules();

	void addRule(Rule rule);
	virtual LinkedList<Module> parse(Module m) const;
};

static class NullRules : Rules {
	LinkedList<Module> parse(Module m) const;
};