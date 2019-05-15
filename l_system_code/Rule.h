/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

#pragma once

#include "Module.h";

class Rule
{
public:
	virtual bool satisfied(Module m) const {
		return false;
	};

};