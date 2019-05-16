/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

#pragma once

#include "Rule.h"
#include <List>
#include <memory>

// Class to represent a full set of Rules for a module
class Rules
{
private:
	std::vector<std::shared_ptr<Rule>> _rules;
public:
	//Constructors
	Rules(std::vector<std::shared_ptr<Rule>> rules);
	Rules();

	void addRule(Rule* rule);
	virtual std::list<Module> parse(Module m) const;
};

class NullRules : public Rules {
	virtual std::list<Module> parse(Module m) const override;
};