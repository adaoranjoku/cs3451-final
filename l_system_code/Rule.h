/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

#pragma once

#include "Module.h";
#include "LinkedList.h";

// Class to represent one rule to change modules
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
static class A_Rule : public Rule
{
public:
	static bool satisfied(Module m);
	static LinkedList<Module> parse(Module m);
};

// Meant for parsing the branch module
static class B_Rule : public Rule
{
public:
	static bool satisfied(Module m);
	static LinkedList<Module> parse(Module m);
};