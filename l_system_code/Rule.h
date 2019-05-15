/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

#pragma once

#include "Module.h";
#include "LinkedList.h";

class Rule
{
public:
	virtual bool satisfied(Module m) const {
		return false;
	};
	virtual LinkedList<Module> parse(Module m) const {
		return LinkedList<Module>();
	}
};

// Meant for parsing the angle module
class A_Rule : public Rule
{
public:
	bool satisfied(Module m) const;
	LinkedList<Module> parse(Module m) const;
};

// Meant for parsing the branch module
class B_Rule : public Rule
{
public:
	bool satisfied(Module m) const;
	LinkedList<Module> parse(Module m) const;
};