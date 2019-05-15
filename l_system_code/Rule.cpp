#include "Rule.h"
/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

bool B_Rule::satisfied(Module m) const
{
	//TODO
	return true;
}

LinkedList<Module> B_Rule::parse(Module m) const
{
	//TODO
	auto list = LinkedList<Module>();
	list.insertNode(m);
	return list;
}

bool A_Rule::satisfied(Module m) const
{
	//TODO
	return true;
}

LinkedList<Module> A_Rule::parse(Module m) const
{
	//TODO
	auto list = LinkedList<Module>();
	list.insertNode(m);
	return list;
}
