/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 15 May 2019
*/

#pragma once

#include <map>
#include "Module.h"
#include "LinkedList.h"
#include "Rules.h"

// Class to represent the entire L System
class System {
private:
	std::map<char, int> _alphabet;
	LinkedList<Module> _axiom;
	LinkedList<Module> _curr_state;
	//If we need to keep going back and forth then we should store all the states in a std::vector
	int _n;
	std::map<char, Rules> _productions;

	LinkedList<Module> string_to_Modules(std::string st);
	Module one_Module(std::string st);
	void iterate();
	void reset();
public:
	System(std::map<char, int> alphabet, std::string axiom, std::map<char, Rules> productions);

	LinkedList<Module> currentSystem(void) const;
	void nextSystem(void);
	void moveToIteration(int i);

	std::string toString(void) const;
};