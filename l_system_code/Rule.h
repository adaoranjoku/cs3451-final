/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

#pragma once

#include "Module.h"
#include <List>

// Class to represent one rule to change modules
class Rule
{
public:
	virtual bool satisfied(Module m) const {
		return false;
	};
	virtual std::list<Module> parse(Module m) const {
		return std::list<Module>();
	}
};

// Meant for parsing the angle module
class A_Rule : public Rule
{
public:
	virtual bool satisfied(Module m) const override;
	virtual std::list<Module> parse(Module m) const override;
};

// Meant for parsing the branch module
class B_Rule : public Rule
{
public:
	virtual bool satisfied(Module m) const override;
	virtual std::list<Module> parse(Module m) const override;
};