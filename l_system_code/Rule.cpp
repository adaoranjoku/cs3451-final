#include "Rule.h"
/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

bool B_Rule::satisfied(Module m) const {
	//TODO
	return true;
}

std::list<Module> B_Rule::parse(Module m) const
{
	//TODO
	return std::list<Module>(1, m);
}

bool A_Rule::satisfied(Module m) const
{
	//TODO
	return true;
}

std::list<Module> A_Rule::parse(Module m) const
{
	//TODO
	auto list = std::list<Module>(1, m);
	list.push_back(Module('B'));
	return list;
}
