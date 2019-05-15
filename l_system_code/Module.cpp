/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/
#include "Module.h"

Module::Module(char letter)
{ 
	l = letter;
	p = std::vector<float>();
}

Module::Module(char letter, std::vector<float> params)
{
	l = letter;
	p = params;
}

char Module::letter() const
{
	return l;
}

float Module::param(int in) const
{
	if (in < p.size())
		return p[in];
	return -69696969.f;
}